#include "stdafx.h"
#include "..\Public\BasicTerrain.h"
#include "GameInstance.h"

CBasicTerrain::CBasicTerrain(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: Super(pDevice, pContext)
{
}

CBasicTerrain::CBasicTerrain(const CBasicTerrain& rhs)
	: Super(rhs)
{
}

HRESULT CBasicTerrain::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CBasicTerrain::Initialize(void* pArg)
{
	if (FAILED(Ready_Components(pArg)))
		return E_FAIL;

	return S_OK;
}

void CBasicTerrain::Tick(const _float& fTimeDelta)
{
}

void CBasicTerrain::LateTick(const _float& fTimeDelta)
{
	GetRenderer()->Add_RenderGroup(CRenderer::RG_NONBLEND, this);
}

void CBasicTerrain::DebugRender()
{
}

HRESULT CBasicTerrain::Render()
{
	if (FAILED(Bind_ShaderResources()))
		return E_FAIL;

	GetShader()->Begin(0);

	static_cast<CTerrain*>(GetFixedComponent(ComponentType::Terrain))->Render();

	return S_OK;
}

HRESULT CBasicTerrain::Ready_Components(void* pArg)
{
	/* Com_Renderer */
	if (FAILED(Super::AddComponent(LEVEL_STATIC, ComponentType::Renderer, TEXT("Prototype_Component_Renderer"))))
		return E_FAIL;

	/* Com_Shader */
	if (FAILED(Super::AddComponent(LEVEL_STATIC, ComponentType::Shader, TEXT("Prototype_Component_Shader_VtxNorTex"))))
		return E_FAIL;

	/* Com_Terrain */
	if (FAILED(Super::AddComponent(LEVEL_STATIC, ComponentType::Terrain, TEXT("Prototype_Component_Terrain")))
		|| FAILED(dynamic_cast<CTerrain*>(GetFixedComponent(ComponentType::Terrain))->InitializeWithHeightMap(TEXT("../Bin/Resources/Textures/Terrain/Height1.bmp"))))
		return E_FAIL;

	/* Com_Texture*/
	if (FAILED(Super::AddComponent(LEVEL_GAMETOOL, ComponentType::Texture, TEXT("Prototype_Component_Texture_Terrain"))))
		return E_FAIL;

	/* Com_Transform */
	if (FAILED(Super::AddComponent(LEVEL_STATIC, ComponentType::Transform, TEXT("Prototype_Component_Transform"))))
		return E_FAIL;

	return S_OK;
}

HRESULT CBasicTerrain::Bind_ShaderResources()
{
	/* 셰이더 전역변수로 던져야 할 값들을 던지자. */
	//if (FAILED(m_pShaderCom->Bind_Matrix("g_WorldMatrix", &IdentityMatrix)))
	//	return E_FAIL;

	_float4x4		ViewMatrix, ProjMatrix;

	XMStoreFloat4x4(&ViewMatrix, XMMatrixLookAtLH(XMVectorSet(0.f, 500.f, -500.f, 1.f), XMVectorSet(0.f, 0.f, 0.f, 1.f), XMVectorSet(0.0f, 1.f, 0.f, 0.f)));
	XMStoreFloat4x4(&ProjMatrix, XMMatrixPerspectiveFovLH(XMConvertToRadians(60.0f), g_iWinSizeX / g_iWinSizeY, 0.1f, 1000.f));

	if (FAILED(GetTransform()->Bind_ShaderResources(GetShader(), "g_WorldMatrix")))
		return E_FAIL;
	/*if (FAILED(GetShader()->Bind_Matrix("g_WorldMatrix", &GetTransform()->GetWorldMatrix())))
		return E_FAIL;*/
	if (FAILED(GetShader()->Bind_Matrix("g_ViewMatrix", &ViewMatrix)))
		return E_FAIL;
	if (FAILED(GetShader()->Bind_Matrix("g_ProjMatrix", &ProjMatrix)))
		return E_FAIL;

	if (FAILED(GetTexture()->Bind_ShaderResource(GetShader(), "g_Texture", 0)))
		return E_FAIL;

	return S_OK;
}

CBasicTerrain* CBasicTerrain::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CBasicTerrain* pInstance = new CBasicTerrain(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CBasicTerrain");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CBasicTerrain* CBasicTerrain::Clone(void* pArg)
{
	CBasicTerrain* pInstance = new CBasicTerrain(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CBasicTerrain");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CBasicTerrain::Free()
{
	Super::Free();
}
