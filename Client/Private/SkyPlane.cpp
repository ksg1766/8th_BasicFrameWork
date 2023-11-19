#include "stdafx.h"
#include "..\Public\SkyPlane.h"
#include "GameInstance.h"
#include "VIBuffer_Point_Instance.h"
#include "Layer.h"

CSkyPlane::CSkyPlane(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: Super(pDevice, pContext)
{
}

CSkyPlane::CSkyPlane(const CSkyPlane& rhs)
	: Super(rhs)
{
}

HRESULT CSkyPlane::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CSkyPlane::Initialize(void* pArg)
{
	if (FAILED(Ready_FixedComponents()))
		return E_FAIL;

	if (FAILED(Ready_Scripts(pArg)))
		return E_FAIL;

	GetTransform()->SetScale(Vec3(1280.f, 1280.f, 1280.f));
	GetTransform()->Rotate(Vec3(0.f, 0.f, 180.f));
	GetTransform()->Translate(Vec3(0.f, 1280.f, 0.f));

	return S_OK;
}

void CSkyPlane::Tick(const _float& fTimeDelta)
{
	Super::Tick(fTimeDelta);
}

void CSkyPlane::LateTick(const _float& fTimeDelta)
{
	Super::LateTick(fTimeDelta);

	GetRenderer()->Add_RenderGroup(CRenderer::RG_NONBLEND, this);
}

void CSkyPlane::DebugRender()
{
	Super::DebugRender();
}

HRESULT CSkyPlane::Render()
{
	//GetTransform()->SetPosition(m_pGameInstance->Get_CamPosition_Vector());

	if (nullptr == GetFixedComponent(ComponentType::Terrain) || nullptr == GetShader())
		return E_FAIL;

	if (FAILED(Bind_ShaderResources()))
		return E_FAIL;

	GetShader()->Begin();

	static_cast<CTerrain*>(GetFixedComponent(ComponentType::Terrain))->Render();

#ifdef _DEBUG
	DebugRender();
#endif

	return S_OK;
}

HRESULT CSkyPlane::Ready_FixedComponents()
{
	/* Com_Shader */
	if (FAILED(Super::AddComponent(LEVEL_STATIC, ComponentType::Shader, TEXT("Prototype_Component_Shader_VtxPosTex"))))
		return E_FAIL;
	GetShader()->SetPassIndex(4);

	/* Com_Terrain */
	if (FAILED(Super::AddComponent(LEVEL_STATIC, ComponentType::Terrain, TEXT("Prototype_Component_Terrain"))) ||
		FAILED(static_cast<CTerrain*>(GetFixedComponent(ComponentType::Terrain))->InitializeNorTex(1280.f, 1280.f, 1280.f, 1280.f)))
		return E_FAIL;
	
	/* Com_Transform */
	if (FAILED(Super::AddComponent(LEVEL_STATIC, ComponentType::Transform, TEXT("Prototype_Component_Transform"))))
			return E_FAIL;

	/* Com_Renderer */
	if (FAILED(Super::AddComponent(LEVEL_STATIC, ComponentType::Renderer, TEXT("Prototype_Component_Renderer"))))
		return E_FAIL;

	/* Com_Texture */
	if (FAILED(Super::AddComponent(LEVEL_STATIC, ComponentType::Texture, TEXT("Prototype_Component_Texture_SkyPlane"))))
		return E_FAIL;

	return S_OK;
}

HRESULT CSkyPlane::Ready_Scripts(void* pArg)
{
	if (LEVEL_GAMEPLAY == m_pGameInstance->GetCurrentLevelIndex())
	{

	}

	return S_OK;
}

HRESULT CSkyPlane::Bind_ShaderResources()
{
	/* 셰이더 전역변수로 던져야 할 값들을 던지자. */
	if (FAILED(GetTransform()->Bind_ShaderResources(GetShader(), "g_WorldMatrix")) ||
		FAILED(m_pGameInstance->Bind_TransformToShader(GetShader(), "g_ViewMatrix", CPipeLine::D3DTS_VIEW)) ||
		FAILED(m_pGameInstance->Bind_TransformToShader(GetShader(), "g_ProjMatrix", CPipeLine::D3DTS_PROJ)))
	{
		return E_FAIL;
	}

	if (FAILED(GetTexture()->Bind_ShaderResource(GetShader(), "g_Texture", 0)))
		return E_FAIL;


	return S_OK;
}

CSkyPlane* CSkyPlane::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CSkyPlane* pInstance = new CSkyPlane(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CSkyPlane");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CSkyPlane::Clone(void* pArg)
{
	CSkyPlane* pInstance = new CSkyPlane(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CSkyPlane");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CSkyPlane::Free()
{
	Super::Free();
}
