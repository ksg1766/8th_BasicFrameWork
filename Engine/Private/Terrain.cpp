#include "..\Public\Terrain.h"

CTerrain::CTerrain(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: Super(pDevice, pContext, ComponentType::Terrain)
{
}

CTerrain::CTerrain(const CTerrain& rhs)
	: Super(rhs)
{
	
}

HRESULT CTerrain::Initialize_Prototype()
{
	

	return S_OK;
}

HRESULT CTerrain::Initialize(void * pArg)
{
	return S_OK;
}

CTerrain* CTerrain::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
{
	CTerrain*	pInstance = new CTerrain(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CTerrain");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CComponent * CTerrain::Clone(void * pArg)
{
	CTerrain*	pInstance = new CTerrain(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CTerrain");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CTerrain::Free()
{

}