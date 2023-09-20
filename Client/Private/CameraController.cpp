#include "stdafx.h"
#include "CameraController.h"
#include "GameInstance.h"
#include "GameObject.h"
#include "Transform.h"

CCameraController::CCameraController(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:Super(pDevice, pContext)
{
}

CCameraController::CCameraController(const CCameraController& rhs)
	:Super(rhs)
{
}

HRESULT CCameraController::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CCameraController::Initialize(void* pArg)
{
	m_vLinearSpeed = Vec3(100.f, 100.f, 100.f);
	m_vMaxLinearSpeed = Vec3(200.f, 200.f, 200.f);
	
	m_vAngularSpeed = Vec3(90.f, 90.f, 90.f);
	m_vMaxAngularSpeed = Vec3(180.f, 180.f, 180.f);

	

	return S_OK;
}

void CCameraController::Tick(const _float& fTimeDelta)
{
	// Input(fTimeDelta);
}

void CCameraController::LateTick(const _float& fTimeDelta)
{
}

void CCameraController::DebugRender()
{
}

void CCameraController::Input(const _float& fTimeDelta)
{
	CGameInstance* pGameInstance = CGameInstance::GetInstance();

	if (KEY_PRESSING(KEY::W) || KEY_DOWN(KEY::W))
		;

	if (KEY_PRESSING(KEY::A) || KEY_DOWN(KEY::A))
		;

	if (KEY_PRESSING(KEY::S) || KEY_DOWN(KEY::S))
		;

	if (KEY_PRESSING(KEY::D) || KEY_DOWN(KEY::D))
		;

}

CCameraController* CCameraController::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CCameraController* pInstance = new CCameraController(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CCameraController");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CComponent* CCameraController::Clone(CGameObject* pGameObject, void* pArg)
{
	CCameraController* pInstance = new CCameraController(*this);
	pInstance->m_pGameObject = pGameObject;

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed To Cloned : CCameraController");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CCameraController::Free()
{
}
