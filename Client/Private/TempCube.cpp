#include "stdafx.h"
#include "..\Public\TempCube.h"
#include "GameInstance.h"

CTempCube::CTempCube(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: CGameObject(pDevice, pContext)
{
}

CTempCube::CTempCube(const CTempCube& rhs)
	: CGameObject(rhs)
{
}

HRESULT CTempCube::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CTempCube::Initialize(void * pArg)
{
	if (FAILED(Ready_Components()))
		return E_FAIL;
	
	return S_OK;
}

void CTempCube::Tick(const _float& fTimeDelta)
{
	// m_pTransformCom->Turn(XMVectorSet(0.f, 1.f, 0.f, 0.f), fTimeDelta);
}

void CTempCube::LateTick(const _float& fTimeDelta)
{
	GetRenderer()->Add_RenderGroup(CRenderer::RG_NONBLEND, this);
}

HRESULT CTempCube::Render()
{
	if (FAILED(Bind_ShaderResources()))
		return E_FAIL;

	GetShader()->Begin(0);

	GetBuffer()->Render();

#ifdef _DEBUG


#endif

	return S_OK;
}

HRESULT CTempCube::Ready_Components()
{
	/* Com_Transform */
	if (FAILED(Super::AddComponent(LEVEL_STATIC, ComponentType::Transform, TEXT("Prototype_Component_Transform"))))
		return E_FAIL;

	/* Com_Renderer */
	if (FAILED(Super::AddComponent(LEVEL_STATIC, ComponentType::Renderer, TEXT("Prototype_Component_Renderer"))))
		return E_FAIL;

	/* Com_Shader */
	if (FAILED(Super::AddComponent(LEVEL_STATIC, ComponentType::Shader, TEXT("Prototype_Component_Shader_VtxNorTex"))))
		return E_FAIL;

	/* Com_VIBuffer */
	if (FAILED(Super::AddComponent(LEVEL_STATIC, ComponentType::Buffer, TEXT("Prototype_Component_VIBuffer_Cube"))))
		return E_FAIL;

	/* For.Com_Texture */
	if (FAILED(Super::AddComponent(LEVEL_GAMEPLAY, ComponentType::Texture, TEXT("Prototype_Component_Texture_Sky"))))
		return E_FAIL;

	return S_OK;
}

HRESULT CTempCube::Bind_ShaderResources()
{
	_float4x4		ViewMatrix, ProjMatrix;

	XMStoreFloat4x4(&ViewMatrix, XMMatrixLookAtLH(XMVectorSet(0.f, 500.f, -500.f, 1.f), XMVectorSet(0.f, 0.f, 0.f, 1.f), XMVectorSet(0.0f, 1.f, 0.f, 0.f)));
	XMStoreFloat4x4(&ProjMatrix, XMMatrixPerspectiveFovLH(XMConvertToRadians(60.0f), g_iWinSizeX / g_iWinSizeY, 0.1f, 1000.f));

	if (FAILED(GetTransform()->Bind_ShaderResources(GetShader(), "g_WorldMatrix")))
		return E_FAIL;
	if (FAILED(GetShader()->Bind_Matrix("g_ViewMatrix", &ViewMatrix)))
		return E_FAIL;
	if (FAILED(GetShader()->Bind_Matrix("g_ProjMatrix", &ProjMatrix)))
		return E_FAIL;
	if (FAILED(GetTexture()->Bind_ShaderResource(GetShader(), "g_Texture", 0)))
		return E_FAIL;

	return S_OK;
}

CTempCube* CTempCube::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
{
	CTempCube*		pInstance = new CTempCube(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed To Created : CTempCube");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CTempCube::Clone(void * pArg)
{
	CTempCube*		pInstance = new CTempCube(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed To Cloned : CTempCube");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CTempCube::Free()
{
	Super::Free();
}
