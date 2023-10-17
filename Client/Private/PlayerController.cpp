#include "stdafx.h"
#include "PlayerController.h"
#include "GameInstance.h"
#include "GameObject.h"

constexpr auto EPSILON = 0.03f;

CPlayerController::CPlayerController(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:Super(pDevice, pContext)
	, m_vLinearSpeed(Vec3(10.f, 10.f, 10.f))
	, m_vMaxLinearSpeed(Vec3(20.f, 20.f, 20.f))
	, m_vAngularSpeed(Vec3(0.f, 540.f, 0.f))
	, m_vMaxAngularSpeed(Vec3(0.f, 540.f, 0.f))
{
}

CPlayerController::CPlayerController(const CPlayerController& rhs)
	:Super(rhs)
{
}

HRESULT CPlayerController::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CPlayerController::Initialize(void* pArg)
{
	m_pTransform = m_pGameObject->GetTransform();
	m_pRigidBody = static_cast<CRigidDynamic*>(m_pGameObject->GetRigidBody());

	m_pRigidBody->FreezeRotation(Axis::X);
	m_pRigidBody->FreezeRotation(Axis::Z);

	m_pRigidBody->UseGravity(false);
	m_pRigidBody->IsKinematic(true);
	m_pRigidBody->SetMass(1.f);

	return S_OK;
}

void CPlayerController::Tick(const _float& fTimeDelta)
{

}

void CPlayerController::LateTick(const _float& fTimeDelta)
{
}

void CPlayerController::DebugRender()
{
}

_bool CPlayerController::Idle()
{	// 루프 끝나면 Idle로 돌아오도록 해야함.
	// 쓸일 없는 함수여야 함.
	if(!Run() && !Aim() && !Jump() && !Dash())
		return true;
}

_bool CPlayerController::Run()
{
	if (KEY_PRESSING(KEY::W) || KEY_DOWN(KEY::W) || KEY_PRESSING(KEY::A) || KEY_DOWN(KEY::A) ||
		KEY_PRESSING(KEY::S) || KEY_DOWN(KEY::S) || KEY_PRESSING(KEY::D) || KEY_DOWN(KEY::D))
		return true;

	return false;
}

_bool CPlayerController::Aim()
{
	const POINT& p = CGameInstance::GetInstance()->GetMousePos();
	if (p.x > 1280 || p.x < 0 || p.y > 720 || p.y < 0)
		return false;

	if (MOUSE_DOWN(MOUSEKEYSTATE::DIM_LB) || MOUSE_PRESSING(MOUSEKEYSTATE::DIM_LB) ||
		MOUSE_DOWN(MOUSEKEYSTATE::DIM_RB) || MOUSE_PRESSING(MOUSEKEYSTATE::DIM_RB))
		return true;

	return false;
}

_bool CPlayerController::Jump()
{
	if (KEY_DOWN(KEY::SPACE))
		return true;

	return false;
}

_bool CPlayerController::Dash()
{
	if (KEY_DOWN(KEY::SHIFT))
		return true;

	return false;
}

void CPlayerController::Move(const Vec3& vDir, const _float& fTimeDelta)
{
	const Vec3& vForward = m_pTransform->GetForward();
	const Vec3& vRotation = m_pTransform->GetRotation();

	_float fRadian = acos(vForward.Dot(vDir));

	if (fabs(fRadian) < EPSILON) return;

	const Vec3& vLeftRight = vForward.Cross(vDir);
	const _float& fRotY = m_pTransform->GetRotation().y;

	Vec3 vSpeed = m_vLinearSpeed * vDir;
	//Vec3 vRotateAmount(m_vAngularSpeed);
	Vec3 vRotateAmount(0.f, 360.f * fRadian, 0.f);
	if (vLeftRight.y < 0)
		vRotateAmount.y = -vRotateAmount.y;

	m_pRigidBody->AddForce(vSpeed, ForceMode::VELOCITY_CHANGE);
	//m_pTransform->Rotate(vRotateAmount);
	m_pRigidBody->AddTorque(vRotateAmount, ForceMode::VELOCITY_CHANGE);

	LimitAllAxisVelocity();
}

void CPlayerController::Input(const _float& fTimeDelta)
{
	if (KEY_PRESSING(KEY::SHIFT) && KEY_DOWN(KEY::K))
		m_pRigidBody->IsKinematic(!m_pRigidBody->IsKinematic());
	if (KEY_PRESSING(KEY::SHIFT) && KEY_DOWN(KEY::U))
		m_pRigidBody->UseGravity(!m_pRigidBody->UseGravity());
}

void CPlayerController::LimitAllAxisVelocity()
{
	Vec3 pCurLinearVelocity = m_pRigidBody->GetLinearVelocity();

	if (pCurLinearVelocity.x > m_vMaxLinearSpeed.x)
		m_pRigidBody->SetLinearAxisVelocity(Axis::X, m_vMaxLinearSpeed.x);
	if (pCurLinearVelocity.x < -m_vMaxLinearSpeed.x)
		m_pRigidBody->SetLinearAxisVelocity(Axis::X, -m_vMaxLinearSpeed.x);

	if (pCurLinearVelocity.y > m_vMaxLinearSpeed.y)
		m_pRigidBody->SetLinearAxisVelocity(Axis::Y, m_vMaxLinearSpeed.y);
	if (pCurLinearVelocity.y < -m_vMaxLinearSpeed.y)
		m_pRigidBody->SetLinearAxisVelocity(Axis::Y, -m_vMaxLinearSpeed.y);

	if (pCurLinearVelocity.z > m_vMaxLinearSpeed.z)
		m_pRigidBody->SetLinearAxisVelocity(Axis::Z, m_vMaxLinearSpeed.z);
	if (pCurLinearVelocity.z < -m_vMaxLinearSpeed.z)
		m_pRigidBody->SetLinearAxisVelocity(Axis::Z, -m_vMaxLinearSpeed.z);
}

CPlayerController* CPlayerController::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CPlayerController* pInstance = new CPlayerController(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CPlayerController");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CComponent* CPlayerController::Clone(CGameObject* pGameObject, void* pArg)
{
	CPlayerController* pInstance = new CPlayerController(*this);
	pInstance->m_pGameObject = pGameObject;

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed To Cloned : CPlayerController");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CPlayerController::Free()
{
	Super::Free();
}
