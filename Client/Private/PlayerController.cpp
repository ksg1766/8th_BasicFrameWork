#include "stdafx.h"
#include "PlayerController.h"
#include "GameInstance.h"
#include "GameObject.h"

#define	EPSILON 0.03f

CPlayerController::CPlayerController(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:Super(pDevice, pContext)
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
	m_vLinearSpeed = Vec3(10.f, 10.f, 10.f);
	m_vMaxLinearSpeed = Vec3(20.f, 20.f, 20.f);
	
	m_vAngularSpeed = Vec3(0.f, 1080.f, 0.f);
	m_vMaxAngularSpeed = Vec3(0.f, 1800.f, 0.f);

	m_pRigidBody->FreezeRotation(Axis::X);
	m_pRigidBody->FreezeRotation(Axis::Z);

	m_pRigidBody->UseGravity(false);
	m_pRigidBody->IsKinematic(true);
	m_pRigidBody->SetMass(1.f);

	return S_OK;
}

void CPlayerController::Tick(const _float& fTimeDelta)
{
	Input(fTimeDelta);
}

void CPlayerController::LateTick(const _float& fTimeDelta)
{
}

void CPlayerController::DebugRender()
{
}

void CPlayerController::Input(const _float& fTimeDelta)
{
	CGameInstance* pGameInstance = CGameInstance::GetInstance();

	if (KEY_PRESSING(KEY::W) || KEY_DOWN(KEY::W))
	{
		m_pRigidBody->AddForce(Vec3(0.f, 0.f, m_vLinearSpeed.z), ForceMode::VELOCITY_CHANGE);

		const Vec3& vForward = m_pTransform->GetForward();
		const Vec3& vRotation = m_pTransform->GetRotation();

		_float fRadian = acos(vForward.Dot(Vec3::UnitZ));

		if (fabs(fRadian) > EPSILON)
		{
			Vec3 vLeftRight = vForward.Cross(Vec3::UnitZ);

			const _float& fRotY = m_pTransform->GetRotation().y;

			Vec3 vRotateAmount(m_vAngularSpeed);

			if (vLeftRight.y < 0) // Left
				vRotateAmount.y = -vRotateAmount.y;

			m_pRigidBody->AddTorque(vRotateAmount, ForceMode::VELOCITY_CHANGE);
		}
	}
	if (KEY_PRESSING(KEY::A) || KEY_DOWN(KEY::A))
	{
		m_pRigidBody->AddForce(Vec3(-m_vLinearSpeed.x, 0.f, 0.f), ForceMode::VELOCITY_CHANGE);

		const Vec3& vForward = m_pTransform->GetForward();
		const Vec3& vRotation = m_pTransform->GetRotation();

		_float fRadian = acos(vForward.Dot(-Vec3::UnitX));

		if (fabs(fRadian) > EPSILON)
		{
			Vec3 vLeftRight = vForward.Cross(-Vec3::UnitX);

			const _float& fRotY = m_pTransform->GetRotation().y;

			Vec3 vRotateAmount(m_vAngularSpeed);

			if (vLeftRight.y < 0) // Left
				vRotateAmount.y = -vRotateAmount.y;

			m_pRigidBody->AddTorque(vRotateAmount, ForceMode::VELOCITY_CHANGE);
		}
	}
	if (KEY_PRESSING(KEY::S) || KEY_DOWN(KEY::S))
	{
		m_pRigidBody->AddForce(Vec3(0.f, 0.f, -m_vLinearSpeed.z), ForceMode::VELOCITY_CHANGE);

		const Vec3& vForward = m_pTransform->GetForward();
		const Vec3& vRotation = m_pTransform->GetRotation();

		_float fRadian = acos(vForward.Dot(-Vec3::UnitZ));

		if (fabs(fRadian) > EPSILON)
		{
			Vec3 vLeftRight = vForward.Cross(-Vec3::UnitZ);

			const _float& fRotY = m_pTransform->GetRotation().y;

			Vec3 vRotateAmount(m_vAngularSpeed);

			if (vLeftRight.y < 0) // Left
				vRotateAmount.y = -vRotateAmount.y;
			
			m_pRigidBody->AddTorque(vRotateAmount, ForceMode::VELOCITY_CHANGE);
		}
	}
	if (KEY_PRESSING(KEY::D) || KEY_DOWN(KEY::D))
	{
		m_pRigidBody->AddForce(Vec3(m_vLinearSpeed.x, 0.f, 0.f), ForceMode::VELOCITY_CHANGE);

		const Vec3& vForward = m_pTransform->GetForward();
		const Vec3& vRotation = m_pTransform->GetRotation();

		_float fRadian = acos(vForward.Dot(Vec3::UnitX));

		if (fabs(fRadian) > EPSILON)
		{
			Vec3 vLeftRight = vForward.Cross(Vec3::UnitX);

			const _float& fRotY = m_pTransform->GetRotation().y;

			Vec3 vRotateAmount(m_vAngularSpeed);

			if (vLeftRight.y < 0) // Left
				vRotateAmount.y = -vRotateAmount.y;

			m_pRigidBody->AddTorque(vRotateAmount, ForceMode::VELOCITY_CHANGE);
		}
	}

	if (KEY_PRESSING(KEY::SHIFT) && KEY_DOWN(KEY::K))
		m_pRigidBody->IsKinematic(!m_pRigidBody->IsKinematic());
	if (KEY_PRESSING(KEY::SHIFT) && KEY_DOWN(KEY::U))
		m_pRigidBody->UseGravity(!m_pRigidBody->UseGravity());

	LimitAllAxisVelocity();
}

void CPlayerController::LimitAllAxisVelocity()
{
	Vec3 pCurLinearVelocity = m_pRigidBody->GetLinearVelocity();
	Vec3 pCurAngularVelocity = m_pRigidBody->GetAngularVelocity();

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

	/*if (pCurAngularVelocity.x > m_vMaxAngularSpeed.x)
		m_pRigidBody->SetAngularAxisVelocity(Axis::X, m_vMaxAngularSpeed.x);
	if (pCurAngularVelocity.x < -m_vMaxAngularSpeed.x)
		m_pRigidBody->SetAngularAxisVelocity(Axis::X, -m_vMaxAngularSpeed.x);

	if (pCurAngularVelocity.y > m_vMaxAngularSpeed.y)
		m_pRigidBody->SetAngularAxisVelocity(Axis::Y, m_vMaxAngularSpeed.y);
	if (pCurAngularVelocity.y < -m_vMaxAngularSpeed.y)
		m_pRigidBody->SetAngularAxisVelocity(Axis::Y, -m_vMaxAngularSpeed.y);

	if (pCurAngularVelocity.z > m_vMaxAngularSpeed.z)
		m_pRigidBody->SetAngularAxisVelocity(Axis::Z, m_vMaxAngularSpeed.z);
	if (pCurAngularVelocity.z < -m_vMaxAngularSpeed.z)
		m_pRigidBody->SetAngularAxisVelocity(Axis::Z, -m_vMaxAngularSpeed.z);*/
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
