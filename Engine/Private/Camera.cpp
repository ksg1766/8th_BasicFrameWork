#include "ComponentManager.h"
#include "PipeLine.h"
#include "GameObject.h"

CCamera::CCamera(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
    :Super(pDevice, pContext, ComponentType::Camera)
	, m_pPipeLine(CPipeLine::GetInstance())
	, m_eMode(PROJECTION_MODE::PERSPECTIVE)
{
	Safe_AddRef(m_pPipeLine);
}

CCamera::CCamera(const CCamera& rhs)
    :Super(rhs)
	, m_pPipeLine(rhs.m_pPipeLine)
	, m_eMode(rhs.m_eMode)
{
	Safe_AddRef(m_pPipeLine);
}

HRESULT CCamera::Initialize_Prototype()
{
	m_vEye		= Vec3::Zero;
	m_vAt		= Vec3::UnitZ;
	m_fFovy		= XM_PI / 4.f;
	m_fAspect	= 1280.f / 720.f;
	m_fNear		= 0.1f;
	m_fFar		= 1000.0f;

	return S_OK;
}

HRESULT CCamera::Initialize(void* pArg)
{
	CAMERA_DESC* pCameraDesc = (CAMERA_DESC*)pArg;

	/* 내가 초기에 셋팅하고 싶은 카메라의 상태 설정. */
	/* 저장만했다. */

	m_pTransform = m_pGameObject->GetTransform();

	m_vEye		= pCameraDesc->vEye;
	m_vAt		= pCameraDesc->vAt;
	m_fFovy		= pCameraDesc->fFovy;
	m_fAspect	= pCameraDesc->fAspect;
	m_fNear		= pCameraDesc->fNear;
	m_fFar		= pCameraDesc->fFar;

    return S_OK;
}

void CCamera::Tick(const _float& fTimeDelta)
{
	/* 카메라 월드행렬의 역행렬 == 뷰스페이스 변환행렬. */
	m_pPipeLine->Set_Transform(CPipeLine::D3DTS_VIEW, m_pTransform->WorldMatrix().Invert());

	switch (m_eMode)
	{
	case PROJECTION_MODE::PERSPECTIVE:
		m_pPipeLine->Set_Transform(CPipeLine::D3DTS_PROJ, XMMatrixPerspectiveFovLH(m_fFovy, m_fAspect, m_fNear, m_fFar));
		break;
	case PROJECTION_MODE::ORTHOGRAPHIC:
		m_pPipeLine->Set_Transform(CPipeLine::D3DTS_PROJ, XMMatrixOrthographicLH(m_fFovy, m_fAspect, m_fNear, m_fFar));
		break;
	}
}

void CCamera::LateTick(const _float& fTimeDelta)
{
}

CCamera* CCamera::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CCamera* pInstance = new CCamera(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CCamera");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CComponent* CCamera::Clone(CGameObject* pGameObject, void* pArg)
{
	CCamera* pInstance = new CCamera(*this);
	pInstance->m_pGameObject = pGameObject;

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CCamera");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CCamera::Free()
{
	Super::Free();

	Safe_Release(m_pTransform);
	Safe_Release(m_pPipeLine);
}
