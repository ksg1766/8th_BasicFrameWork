#include "stdafx.h"
#include "MainCameraController.h"
#include "GameInstance.h"
#include "GameObject.h"
#include "Transform.h"
#include "Camera.h"

CMainCameraController::CMainCameraController(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:Super(pDevice, pContext)
{
}

CMainCameraController::CMainCameraController(const CMainCameraController& rhs)
	:Super(rhs)
{
}

HRESULT CMainCameraController::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CMainCameraController::Initialize(void* pArg)
{
	m_pTransform = static_cast<CTransform*>(m_pGameObject->GetTransform());
	m_pTransform->RotateYAxisFixed(Vec3(45.f, 0.f, 0.f));
	m_pTransform->Translate(Vec3(0.f, 200.f, -200.f));

	m_fLinearSpeed = 100.f;
	m_vAngularSpeed = Vec3(90.f, 90.f, 90.f);
	
	/*CCamera::CAMERA_DESC tDesc;
	::ZeroMemory(&tDesc, sizeof CCamera::CAMERA_DESC);

	tDesc.vEye = Vec4(m_pTransform->GetPosition(), 1.f);
	tDesc.vAt = _float4(0.f, 0.f, 0.f, 1.f);
	tDesc.fFovy = XMConvertToRadians(60.0f);
	tDesc.fAspect = g_iWinSizeX / (_float)g_iWinSizeY;
	tDesc.fNear = 0.1f;
	tDesc.fFar = 1000.0f;*/

	return S_OK;
}

void CMainCameraController::Tick(const _float& fTimeDelta)
{
	Input(fTimeDelta);
}

void CMainCameraController::LateTick(const _float& fTimeDelta)
{
}

void CMainCameraController::DebugRender()
{
}

void CMainCameraController::Input(const _float& fTimeDelta)
{
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	if (KEY_PRESSING(KEY::W) || KEY_DOWN(KEY::W))
		m_pTransform->Translate(fTimeDelta * m_fLinearSpeed * m_pTransform->GetForward());

	if (KEY_PRESSING(KEY::A) || KEY_DOWN(KEY::A))
		m_pTransform->Translate(-fTimeDelta * m_fLinearSpeed * m_pTransform->GetRight());

	if (KEY_PRESSING(KEY::S) || KEY_DOWN(KEY::S))
		m_pTransform->Translate(-fTimeDelta * m_fLinearSpeed * m_pTransform->GetForward());

	if (KEY_PRESSING(KEY::D) || KEY_DOWN(KEY::D))
		m_pTransform->Translate(fTimeDelta * m_fLinearSpeed * m_pTransform->GetRight());

	if (KEY_PRESSING(KEY::Q) || KEY_DOWN(KEY::Q))
		m_pTransform->Translate(fTimeDelta * m_fLinearSpeed * m_pTransform->GetUp());

	if (KEY_PRESSING(KEY::E) || KEY_DOWN(KEY::E))
		m_pTransform->Translate(-fTimeDelta * m_fLinearSpeed * m_pTransform->GetUp());

	const POINT& p = pGameInstance->GetMousePos();
	if (p.x > 1280 || p.x < 0 || p.y > 720 || p.y < 0)
		return;

	_long		dwMouseMove = 0;

	if (dwMouseMove = pGameInstance->Get_DIMouseMove(DIMS_X))
	{
		if (pGameInstance->Mouse_Pressing(DIM_LB))
			m_pTransform->RotateYAxisFixed(Vec3(0.f, -dwMouseMove * fTimeDelta * 4.f, 0.f));
	}

	if (dwMouseMove = pGameInstance->Get_DIMouseMove(DIMS_Y))
	{
		if (pGameInstance->Mouse_Pressing(DIM_LB))
			m_pTransform->RotateYAxisFixed(Vec3(-dwMouseMove * fTimeDelta * 4.f, 0.f, 0.f));
	}

	if (dwMouseMove = pGameInstance->Get_DIMouseMove(DIMS_X))
	{
		if (pGameInstance->Mouse_Pressing(DIM_RB))
			m_pTransform->Translate(dwMouseMove * fTimeDelta * -m_pTransform->GetRight() * 2.f);
	}

	if (dwMouseMove = pGameInstance->Get_DIMouseMove(DIMS_Y))
	{
		if (pGameInstance->Mouse_Pressing(DIM_RB))
			m_pTransform->Translate(dwMouseMove * fTimeDelta * m_pTransform->GetUp() * 2.f);
	}

	RELEASE_INSTANCE(CGameInstance);
}

CMainCameraController* CMainCameraController::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CMainCameraController* pInstance = new CMainCameraController(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CMainCameraController");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CComponent* CMainCameraController::Clone(CGameObject* pGameObject, void* pArg)
{
	CMainCameraController* pInstance = new CMainCameraController(*this);
	pInstance->m_pGameObject = pGameObject;

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed To Cloned : CMainCameraController");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CMainCameraController::Free()
{
}
