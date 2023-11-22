#include "stdafx.h"
#include "..\Public\Lightning.h"
#include "GameInstance.h"
#include "Layer.h"

CLightning::CLightning(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: Super(pDevice, pContext)
{
}

CLightning::CLightning(const CLightning& rhs)
	: Super(rhs)
{
}

HRESULT CLightning::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CLightning::Initialize(void* pArg)
{
	if (FAILED(Ready_FixedComponents()))
		return E_FAIL;

	if (FAILED(Ready_Scripts(pArg)))
		return E_FAIL;
	
	//GetTransform()->SetScale(Vec3(10.f, 10.f, 10.f));
	//GetTransform()->Rotate(Vec3(90.f, 0.0f, 0.f));

	return S_OK;
}

void CLightning::Tick(const _float& fTimeDelta)
{
	Super::Tick(fTimeDelta);

	m_fFrameTime += fTimeDelta;
	if (m_fFrameTime > 1000.0f)
	{
		m_fFrameTime = 0.f;
	}
}

void CLightning::LateTick(const _float& fTimeDelta)
{
	Super::LateTick(fTimeDelta);

	GetRenderer()->Add_RenderGroup(CRenderer::RG_BLEND, this);
}

void CLightning::DebugRender()
{
	Super::DebugRender();
}

HRESULT CLightning::Render()
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

HRESULT CLightning::Ready_FixedComponents()
{
	/* Com_Shader */
	if (FAILED(Super::AddComponent(LEVEL_STATIC, ComponentType::Shader, TEXT("Prototype_Component_Shader_Lightning"))))
		return E_FAIL;

	/* Com_VIBuffer */
	if (FAILED(Super::AddComponent(LEVEL_STATIC, ComponentType::Buffer, TEXT("Prototype_Component_VIBuffer_Point"))))
		return E_FAIL;
	
	/* Com_Transform */
	if (FAILED(Super::AddComponent(LEVEL_STATIC, ComponentType::Transform, TEXT("Prototype_Component_Transform"))))
		return E_FAIL;

	/* Com_Renderer */
	if (FAILED(Super::AddComponent(LEVEL_STATIC, ComponentType::Renderer, TEXT("Prototype_Component_Renderer"))))
		return E_FAIL;
	
	/* Com_Texture */
	if (FAILED(Super::AddComponent(LEVEL_STATIC, ComponentType::Texture, TEXT("Prototype_Component_Texture_Lightning"))))
		return E_FAIL;

	return S_OK;
}

HRESULT CLightning::Ready_Scripts(void* pArg)
{
	if (LEVEL_GAMEPLAY == m_pGameInstance->GetCurrentLevelIndex())
	{
	}

	return S_OK;
}

HRESULT CLightning::Bind_ShaderResources()
{
	_float4x4		WorldMatrix;
	XMStoreFloat4x4(&WorldMatrix, XMMatrixIdentity());

	/* 셰이더 전역변수로 던져야 할 값들을 던지자. */
	if (FAILED(GetTransform()->Bind_ShaderResources(GetShader(), "g_WorldMatrix")) ||
		FAILED(m_pGameInstance->Bind_TransformToShader(GetShader(), "g_ViewMatrix", CPipeLine::D3DTS_VIEW)) ||
		FAILED(m_pGameInstance->Bind_TransformToShader(GetShader(), "g_ProjMatrix", CPipeLine::D3DTS_PROJ)) ||
		FAILED(GetShader()->Bind_RawValue("g_vCamPosition", &static_cast<const _float4&>(m_pGameInstance->Get_CamPosition_Float4()), sizeof(_float4))) ||
		FAILED(GetTexture()->Bind_ShaderResource(GetShader(), "g_LightningTexture", 0)) ||
		FAILED(GetShader()->Bind_RawValue("g_fFrameTime", &m_fFrameTime, sizeof(_float))))
	{
		return E_FAIL;
	}

	return S_OK;
}

CLightning* CLightning::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CLightning* pInstance = new CLightning(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CLightning");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CLightning::Clone(void* pArg)
{
	CLightning* pInstance = new CLightning(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CLightning");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CLightning::Free()
{
	Super::Free();
}
