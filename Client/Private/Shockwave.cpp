#include "stdafx.h"
#include "..\Public\Shockwave.h"
#include "GameInstance.h"
#include "Layer.h"

CShockwave::CShockwave(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: Super(pDevice, pContext)
{
}

CShockwave::CShockwave(const CShockwave& rhs)
	: Super(rhs)
{
}

HRESULT CShockwave::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CShockwave::Initialize(void* pArg)
{
	if (FAILED(Ready_FixedComponents()))
		return E_FAIL;

	if (FAILED(Ready_Scripts(pArg)))
		return E_FAIL;
	
	GetTransform()->SetScale(Vec3(10.f, 10.f, 10.f));
	GetTransform()->Rotate(Vec3(90.f, 0.0f, 0.f));

	return S_OK;
}

void CShockwave::Tick(const _float& fTimeDelta)
{
	Super::Tick(fTimeDelta);

	map<LAYERTAG, CLayer*>& mapLayers = m_pGameInstance->GetCurrentLevelLayers();
	map<LAYERTAG, CLayer*>::iterator iter = mapLayers.find(LAYERTAG::PLAYER);
	
	CLayer* pLayer = iter->second;
	CGameObject*& pObject = pLayer->GetGameObjects().front();
	GetTransform()->SetPosition(pObject->GetTransform()->GetPosition());
}

void CShockwave::LateTick(const _float& fTimeDelta)
{
	Super::LateTick(fTimeDelta);

	GetRenderer()->Add_RenderGroup(CRenderer::RG_DISTORTION, this);
}

void CShockwave::DebugRender()
{
	Super::DebugRender();
}

HRESULT CShockwave::Render()
{
	if (nullptr == GetBuffer() || nullptr == GetShader())
		return E_FAIL;

	if (FAILED(Bind_ShaderResources()))
		return E_FAIL;

	if (FAILED(GetShader()->Begin()))
		return E_FAIL;

	if(FAILED(GetBuffer()->Render()))
		return E_FAIL;

#ifdef _DEBUG
	DebugRender();
#endif

	return S_OK;
}

HRESULT CShockwave::Ready_FixedComponents()
{
	/* Com_Shader */
	if (FAILED(Super::AddComponent(LEVEL_STATIC, ComponentType::Shader, TEXT("Prototype_Component_Shader_Shockwave"))))
		return E_FAIL;

	/* Com_VIBuffer */
	if (FAILED(Super::AddComponent(LEVEL_STATIC, ComponentType::Buffer, TEXT("Prototype_Component_VIBuffer_Rect"))))
		return E_FAIL;
	
	/* Com_Transform */
	if (FAILED(Super::AddComponent(LEVEL_STATIC, ComponentType::Transform, TEXT("Prototype_Component_Transform"))))
		return E_FAIL;

	/* Com_Renderer */
	if (FAILED(Super::AddComponent(LEVEL_STATIC, ComponentType::Renderer, TEXT("Prototype_Component_Renderer"))))
		return E_FAIL;

	/* Com_Texture */
	if (FAILED(Super::AddComponent(LEVEL_GAMEPLAY, ComponentType::Texture, TEXT("Prototype_Component_Texture_Shockwave"))))
		return E_FAIL;

	return S_OK;
}

HRESULT CShockwave::Ready_Scripts(void* pArg)
{
	if (LEVEL_GAMEPLAY == m_pGameInstance->GetCurrentLevelIndex())
	{
	}

	return S_OK;
}

HRESULT CShockwave::Bind_ShaderResources()
{
	_float4x4		WorldMatrix;
	XMStoreFloat4x4(&WorldMatrix, XMMatrixIdentity());

	/* 셰이더 전역변수로 던져야 할 값들을 던지자. */
	if (FAILED(GetTransform()->Bind_ShaderResources(GetShader(), "g_WorldMatrix")) ||
		FAILED(m_pGameInstance->Bind_TransformToShader(GetShader(), "g_ViewMatrix", CPipeLine::D3DTS_VIEW)) ||
		FAILED(m_pGameInstance->Bind_TransformToShader(GetShader(), "g_ProjMatrix", CPipeLine::D3DTS_PROJ)))
	{
		return E_FAIL;
	}

	if (FAILED(GetTexture()->Bind_ShaderResource(GetShader(), "g_DistortionTexture", 0)))
		return E_FAIL;

	return S_OK;
}

CShockwave* CShockwave::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CShockwave* pInstance = new CShockwave(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CShockwave");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CShockwave::Clone(void* pArg)
{
	CShockwave* pInstance = new CShockwave(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CShockwave");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CShockwave::Free()
{
	Super::Free();
}
