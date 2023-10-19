#include "stdafx.h"
#include "..\Public\HellHound.h"
#include "GameInstance.h"

CHellHound::CHellHound(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: Super(pDevice, pContext)
{
}

CHellHound::CHellHound(const CHellHound& rhs)
	: Super(rhs)
{
}

HRESULT CHellHound::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CHellHound::Initialize(void* pArg)
{
	if (FAILED(Ready_FixedComponents()))
		return E_FAIL;

	if (FAILED(Ready_Scripts()))
		return E_FAIL;

	GetRigidBody()->GetSphereCollider()->SetRadius(1.f);
	GetRigidBody()->GetOBBCollider()->SetExtents(Vec3(1.f, 1.f, 0.8f));

	return S_OK;
}

void CHellHound::Tick(const _float& fTimeDelta)
{
	Super::Tick(fTimeDelta);

	if (KEY_DOWN(KEY::NUM_9))
	{
		GetModel()->SetNextAnimationIndex(-1 + GetModel()->GetCurAnimationIndex());
	}
	if (KEY_DOWN(KEY::NUM_0))
	{
		GetModel()->SetNextAnimationIndex(1 + GetModel()->GetCurAnimationIndex());
	}
}

void CHellHound::LateTick(const _float& fTimeDelta)
{
	Super::LateTick(fTimeDelta);

	GetRenderer()->Add_RenderGroup(CRenderer::RG_NONBLEND_INSTANCE, this);
}

void CHellHound::DebugRender()
{
	Super::DebugRender();
}

HRESULT CHellHound::Render()
{
	if (nullptr == GetModel() || nullptr == GetShader())
		return E_FAIL;

	if (FAILED(Bind_ShaderResources()))
		return E_FAIL;

#ifdef _DEBUG
	DebugRender();
#endif

	return S_OK;
}

HRESULT CHellHound::Ready_FixedComponents()
{
	/* Com_Shader */
	if (FAILED(Super::AddComponent(LEVEL_STATIC, ComponentType::Shader, TEXT("Prototype_Component_Shader_VtxAnimInstancing"))))
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
	if (FAILED(Super::AddComponent(LEVEL_STATIC, ComponentType::RigidBody, TEXT("Prototype_Component_RigidDynamic")))
		|| FAILED(GetRigidBody()->InitializeCollider()))
		return E_FAIL;

	return S_OK;
}

HRESULT CHellHound::Ready_Scripts()
{
	return S_OK;
}

HRESULT CHellHound::Bind_ShaderResources()
{
	/* 셰이더 전역변수로 던져야 할 값들을 던지자. */
	if (FAILED(m_pGameInstance->Bind_TransformToShader(GetShader(), "g_ViewMatrix", CPipeLine::D3DTS_VIEW)) ||
		FAILED(m_pGameInstance->Bind_TransformToShader(GetShader(), "g_ProjMatrix", CPipeLine::D3DTS_PROJ)) ||
		FAILED(GetShader()->Bind_RawValue("g_vCamPosition", &static_cast<const _float4&>(m_pGameInstance->Get_CamPosition_Float4()), sizeof(_float4))))
	{
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

	return S_OK;
}

CHellHound* CHellHound::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CHellHound* pInstance = new CHellHound(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CHellHound");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CHellHound::Clone(void* pArg)
{
	CHellHound* pInstance = new CHellHound(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CHellHound");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CHellHound::Free()
{
	Super::Free();
}
