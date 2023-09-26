#include "stdafx.h"
#include "..\Public\FloorTiles_A.h"
#include "GameInstance.h"

CFloorTiles_A::CFloorTiles_A(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: Super(pDevice, pContext)
{
}

CFloorTiles_A::CFloorTiles_A(const Super& rhs)
	: Super(rhs)
{
}

HRESULT CFloorTiles_A::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CFloorTiles_A::Initialize(void* pArg)
{
	if (FAILED(Ready_FixedComponents()))
		return E_FAIL;

	if (FAILED(Ready_Scripts()))
		return E_FAIL;

	//GetTransform()->SetScale(Vec3(.05f, .05f, .05f));

	return S_OK;
}

void CFloorTiles_A::Tick(const _float& fTimeDelta)
{
	Super::Tick(fTimeDelta);
}

void CFloorTiles_A::LateTick(const _float& fTimeDelta)
{
	Super::LateTick(fTimeDelta);

	GetRenderer()->Add_RenderGroup(CRenderer::RG_NONBLEND, this);
}

void CFloorTiles_A::DebugRender()
{
}

HRESULT CFloorTiles_A::Render()
{
	if (nullptr == GetModel() || nullptr == GetShader())
		return E_FAIL;

	if (FAILED(Bind_ShaderResources()))
		return E_FAIL;

	GetShader()->Begin(0);

	GetModel()->Render();

#ifdef _DEBUG
	Super::DebugRender();
#endif

	return S_OK;
}

HRESULT CFloorTiles_A::Ready_FixedComponents()
{
	/* Com_Model */
	if (FAILED(Super::AddComponent(LEVEL_STATIC, ComponentType::Model, TEXT("Prototype_Component_Model"))))
		return E_FAIL;

	if (FAILED(GetModel()->InitializeWithFile(TEXT("../Bin/Resources/Models/FloorTiles_A/FloorTiles_A.fbx"))))
		return E_FAIL;

	/* Com_Transform */
	if (FAILED(Super::AddComponent(LEVEL_STATIC, ComponentType::Transform, TEXT("Prototype_Component_Transform"))))
		return E_FAIL;

	/* Com_Renderer */
	if (FAILED(Super::AddComponent(LEVEL_STATIC, ComponentType::Renderer, TEXT("Prototype_Component_Renderer"))))
		return E_FAIL;

	/* Com_Shader */
	if (FAILED(Super::AddComponent(LEVEL_STATIC, ComponentType::Shader, TEXT("Prototype_Component_Shader_VtxNorTex"))))
		return E_FAIL;

	/* Com_RigidBody */
	if (FAILED(Super::AddComponent(LEVEL_STATIC, ComponentType::RigidBody, TEXT("Prototype_Component_RigidStatic")))
		|| FAILED(GetRigidBody()->InitializeCollider()))
		return E_FAIL;

	return S_OK;
}

HRESULT CFloorTiles_A::Ready_Scripts()
{
	return S_OK;
}

HRESULT CFloorTiles_A::Bind_ShaderResources()
{
	/* 셰이더 전역변수로 던져야 할 값들을 던지자. */
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	if (FAILED(GetTransform()->Bind_ShaderResources(GetShader(), "g_WorldMatrix")) ||
		FAILED(pGameInstance->Bind_TransformToShader(GetShader(), "g_ViewMatrix", CPipeLine::D3DTS_VIEW)) ||
		FAILED(pGameInstance->Bind_TransformToShader(GetShader(), "g_ProjMatrix", CPipeLine::D3DTS_PROJ)) ||
		FAILED(GetShader()->Bind_RawValue("g_vCamPosition", &static_cast<const _float4&>(pGameInstance->Get_CamPosition_Float4()), sizeof(_float4)))/* ||
		FAILED(GetTexture()->Bind_ShaderResource(GetShader(), "g_DiffuseTexture", 2))*/)
	{
		RELEASE_INSTANCE(CGameInstance);
		return E_FAIL;
	}

	RELEASE_INSTANCE(CGameInstance);
	return S_OK;

	//const LIGHT_DESC* pLightDesc = pGameInstance->Get_LightDesc(0);
	/*if (nullptr == pLightDesc)
		return E_FAIL;*/

	//_uint		iPassIndex = 0;

	//if (LIGHT_DESC::LIGHT_DIRECTIONAL == pLightDesc->eLightType)
	//{
	_float4 vLightDir = _float4(1.f, -1.f, 1.f, 0.f);
		if (FAILED(GetShader()->Bind_RawValue("g_vLightDir", &vLightDir, sizeof(_float4))))
			return E_FAIL;
	//	iPassIndex = 0;
	//}
	/*else
	{
		if (FAILED(m_pShaderCom->Bind_RawValue("g_vLightPos", &pLightDesc->vLightPos, sizeof(_float4))))
			return E_FAIL;
		if (FAILED(m_pShaderCom->Bind_RawValue("g_fLightRange", &pLightDesc->fLightRange, sizeof(_float))))
			return E_FAIL;
		iPassIndex = 1;
	}*/

	_float4	vDiffuse = _float4(1.f, 1.f, 1.f, 1.f);
	if (FAILED(GetShader()->Bind_RawValue("g_vLightDiffuse", &vDiffuse, sizeof(_float4))))
		return E_FAIL;

	_float4 vAmbient = _float4(1.f, 1.f, 1.f, 1.f);
	if (FAILED(GetShader()->Bind_RawValue("g_vLightAmbient", &vAmbient, sizeof(_float4))))
		return E_FAIL;

	_float4 vSpecular = _float4(1.f, 1.f, 1.f, 1.f);
	if (FAILED(GetShader()->Bind_RawValue("g_vLightSpecular", &vSpecular, sizeof(_float4))))
		return E_FAIL;

	return S_OK;
}

CFloorTiles_A* CFloorTiles_A::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CFloorTiles_A* pInstance = new CFloorTiles_A(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CFloorTiles_A");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CFloorTiles_A::Clone(void* pArg)
{
	CFloorTiles_A* pInstance = new CFloorTiles_A(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CFloorTiles_A");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CFloorTiles_A::Free()
{
	Super::Free();
}
