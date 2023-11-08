#include "..\Public\LightManager.h"
#include "Light.h"

IMPLEMENT_SINGLETON(CLightManager)

CLightManager::CLightManager()
{

}

const LIGHT_DESC* CLightManager::Get_LightDesc(_uint iLightIndex)
{
	if (iLightIndex >= m_vecLights.size())
		return nullptr;

	return m_vecLights[iLightIndex]->Get_LightDesc();
}

HRESULT CLightManager::Add_Light(const LIGHT_DESC & LightDesc)
{
	CLight*		pLight = CLight::Create(LightDesc);

	if (nullptr == pLight)
		return E_FAIL;

	m_vecLights.push_back(pLight);

	return S_OK;
}

HRESULT CLightManager::Render(CShader * pShader, CVIBuffer_Rect * pVIBuffer)
{
	for (auto& pLight : m_vecLights)
	{
		pLight->Render(pShader, pVIBuffer);
	}

	return S_OK;
}

void CLightManager::Free()
{
	for (auto& pLight : m_vecLights)
		Safe_Release(pLight);

	m_vecLights.clear();
}
