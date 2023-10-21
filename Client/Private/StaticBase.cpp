#include "stdafx.h"
#include "..\Public\StaticBase.h"
#include "GameInstance.h"

CStaticBase::CStaticBase(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: Super(pDevice, pContext)
{
}

CStaticBase::CStaticBase(const CStaticBase& rhs)
	: Super(rhs)
{
}

HRESULT CStaticBase::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CStaticBase::Initialize(void* pArg)
{
	if (FAILED(Ready_FixedComponents()))
		return E_FAIL;

	if (FAILED(Ready_Scripts()))
		return E_FAIL;

	GetRigidBody()->GetSphereCollider()->SetRadius(10.f);
	GetRigidBody()->GetOBBCollider()->SetExtents(Vec3(5.f, 5.f, 5.f));

	return S_OK;
}

void CStaticBase::Tick(const _float& fTimeDelta)
{
	Super::Tick(fTimeDelta);
}

void CStaticBase::LateTick(const _float& fTimeDelta)
{
	Super::LateTick(fTimeDelta);

	GetRenderer()->Add_RenderGroup(CRenderer::RG_NONBLEND, this);
}

void CStaticBase::DebugRender()
{
}

HRESULT CStaticBase::Render()
{
	if (nullptr == GetModel() || nullptr == GetShader())
		return E_FAIL;

	if (FAILED(Bind_ShaderResources()))
		return E_FAIL;

	_uint		iNumMeshes = GetModel()->GetNumMeshes();

	GetShader()->Begin();

	GetModel()->Render();

#ifdef _DEBUG
	Super::DebugRender();
#endif

	return S_OK;
}

HRESULT CStaticBase::Ready_FixedComponents()
{
	/* Com_Shader */
	if (FAILED(Super::AddComponent(LEVEL_STATIC, ComponentType::Shader, TEXT("Prototype_Component_Shader_VtxMesh"))))
		return E_FAIL;

	/* Com_Model */
	if (FAILED(Super::AddComponent(LEVEL_STATIC, ComponentType::Model, TEXT("Prototype_Component_Model_") + GetObjectTag())))
		return E_FAIL;

	/* Com_Transform */
	if (FAILED(Super::AddComponent(LEVEL_STATIC, ComponentType::Transform, TEXT("Prototype_Component_Transform"))))
		return E_FAIL;

	/* Com_Renderer */
	if (FAILED(Super::AddComponent(LEVEL_STATIC, ComponentType::Renderer, TEXT("Prototype_Component_Renderer"))))
		return E_FAIL;

	/* Com_RigidBody */
	if (FAILED(Super::AddComponent(LEVEL_STATIC, ComponentType::RigidBody, TEXT("Prototype_Component_RigidStatic")))
		|| FAILED(GetRigidBody()->InitializeCollider()))
		return E_FAIL;

	return S_OK;
}

HRESULT CStaticBase::Ready_Scripts()
{
	return S_OK;
}

HRESULT CStaticBase::Bind_ShaderResources()
{
	/* 셰이더 전역변수로 던져야 할 값들을 던지자. */
	if (FAILED(GetTransform()->Bind_ShaderResources(GetShader(), "g_WorldMatrix")) ||
		FAILED(m_pGameInstance->Bind_TransformToShader(GetShader(), "g_ViewMatrix", CPipeLine::D3DTS_VIEW)) ||
		FAILED(m_pGameInstance->Bind_TransformToShader(GetShader(), "g_ProjMatrix", CPipeLine::D3DTS_PROJ)) ||
		FAILED(GetShader()->Bind_RawValue("g_vCamPosition", &static_cast<const _float4&>(m_pGameInstance->Get_CamPosition_Float4()), sizeof(_float4))))
	{
		return E_FAIL;
	}

	// Dissolve Test
	/*if (FAILED(GetShader()->Bind_RawValue("g_fDissolveAmount", &m_fDissolveAmount, sizeof(_float))))
		return E_FAIL;*/

	//
	//return S_OK;

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

CStaticBase* CStaticBase::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CStaticBase* pInstance = new CStaticBase(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CStaticBase");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CStaticBase::Clone(void* pArg)
{
	CStaticBase* pInstance = new CStaticBase(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CStaticBase");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CStaticBase::Free()
{
	Super::Free();
}
