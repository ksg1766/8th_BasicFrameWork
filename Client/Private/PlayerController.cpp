#include "stdafx.h"
#include "PlayerController.h"
#include "GameInstance.h"
#include "GameObject.h"
#include "RigidDynamic.h"

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
	m_pRigidBody = static_cast<CRigidDynamic*>(m_pGameObject->GetRigidBody());
	m_vSpeed = Vec3(10.f, 10.f, 10.f);
	m_vMaxSpeed = Vec3(10.f, 10.f, 10.f);
	
	m_pRigidBody->FreezeRotation(Axis::X);
	m_pRigidBody->FreezeRotation(Axis::Z);

	m_pRigidBody->IsKinematic(true);

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

void CPlayerController::Move(const _float& fTimeDelta)
{
	CGameInstance* pGameInstance = CGameInstance::GetInstance();

	if (KEY_PRESSING(KEY::W) || KEY_DOWN(KEY::W))
		m_pRigidBody->AddForce(Vec3(0.f, 0.f, m_vSpeed.z), ForceMode::VELOCITY_CHANGE);

	if (KEY_PRESSING(KEY::A) || KEY_DOWN(KEY::A))
		m_pRigidBody->AddForce(Vec3(-m_vSpeed.x, 0.f, 0.f), ForceMode::VELOCITY_CHANGE);

	if (KEY_PRESSING(KEY::S) || KEY_DOWN(KEY::S))
		m_pRigidBody->AddForce(Vec3(0.f, 0.f, -m_vSpeed.z), ForceMode::VELOCITY_CHANGE);

	if (KEY_PRESSING(KEY::D) || KEY_DOWN(KEY::D))
		m_pRigidBody->AddForce(Vec3(m_vSpeed.x, 0.f, 0.f), ForceMode::VELOCITY_CHANGE);

	LimitAllAxisVelocity();
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

void CPlayerController::LimitAllAxisVelocity()
{
	Vec3 pCurLinearVelocity = m_pRigidBody->GetLinearVelocity();

	if (fabs(pCurLinearVelocity.x) > fabs(m_vMaxSpeed.x))
		m_pRigidBody->SetLinearAxisVelocity(Axis::X, m_vMaxSpeed.x);
	if (fabs(pCurLinearVelocity.y) > fabs(m_vMaxSpeed.y))
		m_pRigidBody->SetLinearAxisVelocity(Axis::Y, m_vMaxSpeed.y);
	if (fabs(pCurLinearVelocity.z) > fabs(m_vMaxSpeed.z))
		m_pRigidBody->SetLinearAxisVelocity(Axis::Z, m_vMaxSpeed.z);
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
}
