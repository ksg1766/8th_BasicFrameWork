#include "stdafx.h"
#include "PlayerController.h"
#include "GameInstance.h"
#include "GameObject.h"
#include "Strife_Ammo_Default.h"

constexpr auto EPSILON = 0.03f;

CPlayerController::CPlayerController(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:Super(pDevice, pContext)
	, m_vLinearSpeed(Vec3(7.f, 7.f, 7.f))
	, m_vMaxLinearSpeed(Vec3(20.f, 20.f, 20.f))
	, m_vAngularSpeed(Vec3(0.f, 360.f, 0.f))
	, m_vMaxAngularSpeed(Vec3(0.f, 540.f, 0.f))
{
}

CPlayerController::CPlayerController(const CPlayerController& rhs)
	:Super(rhs)
	, m_vLinearSpeed(rhs.m_vLinearSpeed)
	, m_vMaxLinearSpeed(rhs.m_vMaxLinearSpeed)
	, m_vAngularSpeed(rhs.m_vAngularSpeed)
	, m_vMaxAngularSpeed(rhs.m_vMaxAngularSpeed)
{
}

HRESULT CPlayerController::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CPlayerController::Initialize(void* pArg)
{
	m_pTransform = GetTransform();
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
	Move(fTimeDelta);
}

void CPlayerController::LateTick(const _float& fTimeDelta)
{
}

void CPlayerController::DebugRender()
{
}

_bool CPlayerController::IsIdle()
{	// 루프 끝나면 Idle로 돌아오도록 해야함.
	// 쓸일 없는 함수여야 함.
	if(!IsRun() && !IsAim() && !IsJump() && !IsDash())
		return true;
}

_bool CPlayerController::IsRun()
{
	if (KEY_PRESSING(KEY::W) || KEY_DOWN(KEY::W) || KEY_PRESSING(KEY::A) || KEY_DOWN(KEY::A) ||
		KEY_PRESSING(KEY::S) || KEY_DOWN(KEY::S) || KEY_PRESSING(KEY::D) || KEY_DOWN(KEY::D))
		return true;

	return false;
}

_bool CPlayerController::IsAim()
{
	const POINT& p = CGameInstance::GetInstance()->GetMousePos();
	if (p.x > 1440 || p.x < 0 || p.y > 810 || p.y < 0)
		return false;

	if (MOUSE_DOWN(MOUSEKEYSTATE::DIM_LB) || MOUSE_PRESSING(MOUSEKEYSTATE::DIM_LB) ||
		MOUSE_DOWN(MOUSEKEYSTATE::DIM_RB) || MOUSE_PRESSING(MOUSEKEYSTATE::DIM_RB))
		return true;

	return false;
}

_bool CPlayerController::IsJump()
{
	if (KEY_DOWN(KEY::SPACE))
		return true;

	return false;
}

_bool CPlayerController::IsDash()
{
	if (KEY_DOWN(KEY::SHIFT))
		return true;

	return false;
}

void CPlayerController::Move(const _float& fTimeDelta)
{
	if (m_vNetTrans.Length() < EPSILON) return;

	m_vNetTrans.Normalize();

	const Vec3& vForward = m_pTransform->GetForward();

	Vec3 vSpeed = m_vLinearSpeed * m_vNetTrans;
	m_pTransform->Translate(fTimeDelta * vSpeed);

	_float fRadian = acos(vForward.Dot(m_vNetTrans));
	if (fabs(fRadian) > EPSILON)
	{
		const Vec3& vLeftRight = vForward.Cross(m_vNetTrans);
		Vec3 vRotateAmount(m_vAngularSpeed * fRadian);
		if (vLeftRight.y < 0)
			vRotateAmount.y = -vRotateAmount.y;

		m_pTransform->RotateYAxisFixed(fTimeDelta * vRotateAmount);
	}

	m_vNetTrans = Vec3::Zero;
}

void CPlayerController::Jump()
{
	m_pRigidBody->IsKinematic(false);
	m_pRigidBody->UseGravity(true);
	m_pRigidBody->ClearForce(ForceMode::FORCE);
	m_pRigidBody->ClearForce(ForceMode::IMPULSE);
	m_pRigidBody->AddForce(10.f * Vec3::UnitY, ForceMode::IMPULSE);
}

void CPlayerController::Land()
{
	m_pRigidBody->ClearForce(ForceMode::ACCELERATION);
	m_pRigidBody->ClearForce(ForceMode::VELOCITY_CHANGE);
	m_pRigidBody->IsKinematic(true);
	m_pRigidBody->UseGravity(false);
}

void CPlayerController::Dash(const Vec3& vDir)
{
	m_pRigidBody->ClearForce(ForceMode::FORCE);
	m_pRigidBody->ClearForce(ForceMode::IMPULSE);
	m_pRigidBody->IsKinematic(false);
	m_pRigidBody->UseGravity(false);
	m_pRigidBody->AddForce(30.f * m_pTransform->GetForward(), ForceMode::IMPULSE);
}

void CPlayerController::DashEnd()
{
	m_pRigidBody->ClearForce(ForceMode::FORCE);
	m_pRigidBody->ClearForce(ForceMode::IMPULSE);
	m_pRigidBody->IsKinematic(true);
}

void CPlayerController::GetDashMessage(const _bool& IsDash)
{
	IsDash ? Dash(m_pTransform->GetForward()) : DashEnd();
}

void CPlayerController::Fire(const _float& fTimeDelta, CStrife_Ammo::AmmoType eAmmoType)
{
	CGameObject* pAmmo = nullptr;
	
	switch (eAmmoType)
	{
	case CStrife_Ammo::AmmoType::DEFAULT:
		CStrife_Ammo_Default::AMMOPROPS tProps{ eAmmoType, 7, 0, 1, 50.f * m_pTransform->GetForward(), false, 5.f };
		pAmmo = m_pGameInstance->CreateObject(L"Prototype_GameObject_Strife_Ammo_Default", LAYERTAG::EQUIPMENT, &tProps);
		break;

	/*case CStrife_Ammo::AmmoType::CHARGE:
		CStrife_Ammo::AMMOPROPS tPros{ eAmmoType, 4, 0, 1, 10.f * m_pTransform->GetForward(), false, 5.f };
		pInstance->CreateObject(L"Prototype_GameObject_Strife_Ammo_Charge", LAYERTAG::EQUIPMENT, &tProps);
		break;

	case CStrife_Ammo::AmmoType::STATIC:
		CStrife_Ammo::AMMOPROPS tPros{ eAmmoType, 4, 0, 1, 10.f * m_pTransform->GetForward(), false, 5.f };
		pInstance->CreateObject(L"Prototype_GameObject_Strife_Ammo_Static", LAYERTAG::EQUIPMENT, &tProps);
		break;

	case CStrife_Ammo::AmmoType::GRAVITY:
		CStrife_Ammo::AMMOPROPS tPros{ eAmmoType, 4, 0, 1, 10.f * m_pTransform->GetForward(), false, 5.f };
		pInstance->CreateObject(L"Prototype_GameObject_Strife_Ammo_Gravity", LAYERTAG::EQUIPMENT, &tProps);
		break;

	case CStrife_Ammo::AmmoType::NATURE:
		CStrife_Ammo::AMMOPROPS tPros{ eAmmoType, 4, 0, 1, 10.f * m_pTransform->GetForward(), false, 5.f };
		pInstance->CreateObject(L"Prototype_GameObject_Strife_Ammo_Nature", LAYERTAG::EQUIPMENT, &tProps);
		break;

	case CStrife_Ammo::AmmoType::BEAM:
		CStrife_Ammo::AMMOPROPS tPros{ eAmmoType, 4, 0, 1, 10.f * m_pTransform->GetForward(), false, 5.f };
		pInstance->CreateObject(L"Prototype_GameObject_Strife_Ammo_Beam", LAYERTAG::EQUIPMENT, &tProps);
		break;

	case CStrife_Ammo::AmmoType::LAVA:
		CStrife_Ammo::AMMOPROPS tPros{ eAmmoType, 4, 0, 1, 10.f * m_pTransform->GetForward(), false, 5.f };
		pInstance->CreateObject(L"Prototype_GameObject_Strife_Ammo_Lava", LAYERTAG::EQUIPMENT, &tProps);
		break;*/
	}

	Vec3 vFireOffset = m_pTransform->GetPosition() + 2.f * m_pTransform->GetForward() - (m_bFireLR * 0.35f - 0.175f) * m_pTransform->GetRight() + 1.7f * m_pTransform->GetUp();
	Quaternion qRot = m_pTransform->GetRotationQuaternion();

	pAmmo->GetTransform()->SetScale(Vec3(0.28f, 3.6f, 1.f));
	pAmmo->GetTransform()->Rotate(Vec3(90.f, 0.0f, 0.f));
	pAmmo->GetTransform()->Rotate(qRot);
	pAmmo->GetTransform()->SetPosition(vFireOffset);

	m_bFireLR = !m_bFireLR;
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
