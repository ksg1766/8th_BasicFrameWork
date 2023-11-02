#include "..\Public\Renderer.h"
#include "GameObject.h"
#include "Transform.h"
#include "Model.h"
#include "Shader.h"
#include "VIBuffer_Instance.h"

CRenderer::CRenderer(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: CComponent(pDevice, pContext, ComponentType::Renderer)
{
}

HRESULT CRenderer::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CRenderer::Initialize(void * pArg)
{
	return S_OK;
}

HRESULT CRenderer::Add_RenderGroup(RENDERGROUP eRenderGroup, CGameObject * pGameObject)
{
	if (eRenderGroup >= RG_END)
		return S_OK;

	m_RenderObjects[eRenderGroup].push_back(pGameObject);

	Safe_AddRef(pGameObject);

	return S_OK;
}

HRESULT CRenderer::Draw_RenderObjects()
{
	ClearInstanceData();

	if (FAILED(Render_Priority()))
		return S_OK;
	if (FAILED(Render_NonLight()))
		return S_OK;
	if (FAILED(Render_NonBlend()))
		return S_OK;
	if (FAILED(Render_NonBlend_Instance()))
		return S_OK;
	if (FAILED(Render_Blend()))
		return S_OK;
	if (FAILED(Render_Blend_Instance()))
		return S_OK;
	if (FAILED(Render_UI()))
		return S_OK;

	return S_OK;
}

HRESULT CRenderer::Render_Priority()
{
	for (auto& pGameObject : m_RenderObjects[RG_PRIORITY])
	{
		if (nullptr != pGameObject)
			pGameObject->Render();

		Safe_Release(pGameObject);
	}
	m_RenderObjects[RG_PRIORITY].clear();

	return S_OK;
}

HRESULT CRenderer::Render_NonLight()
{
	for (auto& pGameObject : m_RenderObjects[RG_NONLIGHT])
	{
		if (nullptr != pGameObject)
			pGameObject->Render();

		Safe_Release(pGameObject);
	}
	m_RenderObjects[RG_NONLIGHT].clear();
	
	return S_OK;
}

HRESULT CRenderer::Render_NonBlend()
{
	for (auto& pGameObject : m_RenderObjects[RG_NONBLEND])
	{
		if (nullptr != pGameObject)
			pGameObject->Render();

		Safe_Release(pGameObject);
	}
	m_RenderObjects[RG_NONBLEND].clear();

	return S_OK;
}

HRESULT CRenderer::Render_NonBlend_Instance()
{
	map<InstanceID, vector<CGameObject*>> cache;

	for (auto& pGameObject : m_RenderObjects[RG_NONBLEND_INSTANCE])
	{
		if (nullptr == pGameObject->GetModel())
			continue;

		const _int iPassIndex = pGameObject->GetShader()->GetPassIndex();
		const _int instanceId = pGameObject->GetModel()->GetInstanceID();
		InstanceID ID(iPassIndex, instanceId);
		cache[ID].push_back(pGameObject);

		Safe_Release(pGameObject);
	}

	for (auto& mapIter : cache)
	{
		InstancedTweenDesc* tweenDesc = new InstancedTweenDesc;

		vector<CGameObject*>& vecInstances = mapIter.second;

		const InstanceID instanceId = mapIter.first;

		CGameObject*& pChief = vecInstances[0];

		for (_int i = 0; i < vecInstances.size(); i++)
		{
			CGameObject*& pGameObject = vecInstances[i];
			InstancingData data;
			data.matWorld = pGameObject->GetTransform()->WorldMatrix();

			AddInstanceData(instanceId, data);
		}

		if(pChief->GetModel()->IsAnimModel())
		{// INSTANCING
			InstancedTweenDesc* tweenDesc = new InstancedTweenDesc;
			for (_int i = 0; i < vecInstances.size(); i++)
			{
				CGameObject*& pGameObject = vecInstances[i];
				tweenDesc->tweens[i] = pGameObject->GetModel()->GetTweenDesc();	// 소켓 아이템의 경우 어떻게 할지.(굳이 인스턴싱이 필요 없을 듯 함 근데.)
			}

			pChief->GetModel()->PushTweenData(*tweenDesc);
		}

		Safe_Delete(tweenDesc);

		pChief->RenderInstance();	// BindShaderResource 호출을 위함.
		CVIBuffer_Instance*& buffer = m_InstanceBuffers[instanceId];
		pChief->GetModel()->RenderInstancing(buffer);
	}
	
	m_RenderObjects[RG_NONBLEND_INSTANCE].clear();

	return S_OK;
}

HRESULT CRenderer::Render_Blend()
{
	for (auto& pGameObject : m_RenderObjects[RG_BLEND])
	{
		if (nullptr != pGameObject)
			pGameObject->Render();

		Safe_Release(pGameObject);
	}
	m_RenderObjects[RG_BLEND].clear();

	return S_OK;
}

HRESULT CRenderer::Render_Blend_Instance()
{
	for (auto& pGameObject : m_RenderObjects[RG_BLEND_INSTANCE])
	{
		if (nullptr != pGameObject)
			pGameObject->RenderInstance();

		Safe_Release(pGameObject);
	}
	m_RenderObjects[RG_BLEND_INSTANCE].clear();

	return S_OK;
}

HRESULT CRenderer::Render_UI()
{
	for (auto& pGameObject : m_RenderObjects[RG_UI])
	{
		if (nullptr != pGameObject)
			pGameObject->Render();

		Safe_Release(pGameObject);
	}
	m_RenderObjects[RG_UI].clear();


	return S_OK;
}

void CRenderer::AddInstanceData(InstanceID instanceId, InstancingData& data)
{
	if (m_InstanceBuffers.find(instanceId) == m_InstanceBuffers.end())
		m_InstanceBuffers[instanceId] = CVIBuffer_Instance::Create(m_pDevice, m_pContext);

	m_InstanceBuffers[instanceId]->AddData(data);
}

void CRenderer::ClearInstanceData()
{
	for (auto& pair : m_InstanceBuffers)
	{
		CVIBuffer_Instance* pBuffer = pair.second;
		pBuffer->ClearData();
	}
}

CRenderer * CRenderer::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
{
	CRenderer*	pInstance = new CRenderer(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CRenderer");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CComponent * CRenderer::Clone(CGameObject* pGameObject, void * pArg)
{
	AddRef();

	return this;
}

void CRenderer::Free()
{
	ClearInstanceData();
	Super::Free();
}