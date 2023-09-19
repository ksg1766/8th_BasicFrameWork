#include "Camera.h"

CCamera::CCamera(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
    :Super(pDevice, pContext, ComponentType::Camera)
{
}

CCamera::CCamera(const CCamera& rhs)
    :Super(rhs)
{
}

HRESULT CCamera::Initialize_Prototype()
{
	return E_NOTIMPL;
}

HRESULT CCamera::Initialize(void* pArg)
{
    return S_OK;
}

void CCamera::Tick(const _float& fTimeDelta)
{
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
	__super::Free();
}
