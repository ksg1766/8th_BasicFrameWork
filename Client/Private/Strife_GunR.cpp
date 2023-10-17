#include "stdafx.h"
#include "..\Public\Strife_GunR.h"
#include "GameInstance.h"

CStrife_GunR::CStrife_GunR(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: Super(pDevice, pContext)
{
}

CStrife_GunR::CStrife_GunR(const CStrife_GunR& rhs)
	: Super(rhs)
{
}

HRESULT CStrife_GunR::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CStrife_GunR::Initialize(void* pArg)
{
	if (FAILED(Ready_FixedComponents()))
		return E_FAIL;

	if (FAILED(Ready_Scripts()))
		return E_FAIL;	

	return S_OK;
}

void CStrife_GunR::Tick(const _float& fTimeDelta)
{

	Super::Tick(fTimeDelta);
}

void CStrife_GunR::LateTick(const _float& fTimeDelta)
{
	Super::LateTick(fTimeDelta);

	GetRenderer()->Add_RenderGroup(CRenderer::RG_NONBLEND, this);
}

void CStrife_GunR::DebugRender()
{
	Super::DebugRender();
}

HRESULT CStrife_GunR::Render()
{
	if (nullptr == GetModel() || nullptr == GetShader())
		return E_FAIL;

	if (FAILED(Bind_ShaderResources()))
		return E_FAIL;

	GetModel()->Render();

#ifdef _DEBUG
	DebugRender();
#endif

	return S_OK;
}

HRESULT CStrife_GunR::Ready_FixedComponents()
{
	/* Com_Shader */
	if (FAILED(Super::AddComponent(LEVEL_STATIC, ComponentType::Shader, TEXT("Prototype_Component_Shader_VTFSocket"))))
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

	/* Com_RigidBody */	// ���̶� �ʿ����

	return S_OK;
}

HRESULT CStrife_GunR::Ready_Scripts()
{
	return S_OK;
}

HRESULT CStrife_GunR::Bind_ShaderResources()
{
	/* ���̴� ���������� ������ �� ������ ������. */
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	if (FAILED(GetTransform()->Bind_ShaderResources(GetShader(), "g_WorldMatrix")) || 
		FAILED(pGameInstance->Bind_TransformToShader(GetShader(), "g_ViewMatrix", CPipeLine::D3DTS_VIEW)) ||
		FAILED(pGameInstance->Bind_TransformToShader(GetShader(), "g_ProjMatrix", CPipeLine::D3DTS_PROJ)) ||
		FAILED(GetShader()->Bind_RawValue("g_vCamPosition", &static_cast<const _float4&>(pGameInstance->Get_CamPosition_Float4()), sizeof(_float4))))
	{
		RELEASE_INSTANCE(CGameInstance);
		return E_FAIL;
	}


	_float4 vLightDir = _float4(1.f, -1.f, 1.f, 0.f);
	if (FAILED(GetShader()->Bind_RawValue("g_vLightDir", &vLightDir, sizeof(_float4))))
		return E_FAIL;

	_float4	vDiffuse = _float4(1.f, 1.f, 1.f, 1.f);
	if (FAILED(GetShader()->Bind_RawValue("g_vLightDiffuse", &vDiffuse, sizeof(_float4))))
		return E_FAIL;

	_float4 vAmbient = _float4(1.f, 1.f, 1.f, 1.f);
	if (FAILED(GetShader()->Bind_RawValue("g_vLightAmbient", &vAmbient, sizeof(_float4))))
		return E_FAIL;

	_float4 vSpecular = _float4(1.f, 1.f, 1.f, 1.f);
	if (FAILED(GetShader()->Bind_RawValue("g_vLightSpecular", &vSpecular, sizeof(_float4))))
		return E_FAIL;

	RELEASE_INSTANCE(CGameInstance);
	return S_OK;
}

CStrife_GunR* CStrife_GunR::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CStrife_GunR* pInstance = new CStrife_GunR(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CStrife_GunR");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CStrife_GunR::Clone(void* pArg)
{
	CStrife_GunR* pInstance = new CStrife_GunR(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CStrife_GunR");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CStrife_GunR::Free()
{
	Super::Free();
}