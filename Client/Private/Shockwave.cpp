#include "stdafx.h"
#include "..\Public\Shockwave.h"
#include "GameInstance.h"

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

	return S_OK;
}

void CShockwave::Tick(const _float& fTimeDelta)
{
	Super::Tick(fTimeDelta);
}

void CShockwave::LateTick(const _float& fTimeDelta)
{
	Super::LateTick(fTimeDelta);

	GetRenderer()->Add_RenderGroup(CRenderer::RG_BLEND, this);
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

#ifdef _DEBUG
	DebugRender();
#endif

	return S_OK;
}

HRESULT CShockwave::Ready_FixedComponents()
{
	/* Com_Shader */
	if (FAILED(Super::AddComponent(LEVEL_STATIC, ComponentType::Shader, TEXT("Prototype_Component_Shader_Distortion"))))
		return E_FAIL;

	/* Com_VIBuffer */
	if (FAILED(Super::AddComponent(LEVEL_STATIC, ComponentType::Buffer, TEXT("Prototype_Component_VIBuffer_Point"))))
		return E_FAIL;
	
	/* Com_Transform */
	if (FAILED(Super::AddComponent(LEVEL_STATIC, ComponentType::Transform, TEXT("Prototype_Component_Transform"))))
		return E_FAIL;

	/* Com_RigidBody */
	/*if (FAILED(Super::AddComponent(LEVEL_STATIC, ComponentType::RigidBody, TEXT("Prototype_Component_RigidDynamic")))
		|| FAILED(GetRigidBody()->InitializeCollider()))
		return E_FAIL;*/

	/* Com_Renderer */
	if (FAILED(Super::AddComponent(LEVEL_STATIC, ComponentType::Renderer, TEXT("Prototype_Component_Renderer"))))
		return E_FAIL;

	/* Com_Texture */
	if (FAILED(Super::AddComponent(LEVEL_GAMEPLAY, ComponentType::Texture, TEXT("Prototype_Component_Texture_FlameMask"))))
		return E_FAIL;

	return S_OK;
}

HRESULT CShockwave::Ready_Scripts(void* pArg)
{
	if (LEVEL_GAMEPLAY == m_pGameInstance->GetCurrentLevelIndex())
	{
		if (FAILED(Super::AddComponent(LEVEL_GAMEPLAY, ComponentType::Script, TEXT("Prototype_Component_ShockwaveController"), pArg)))
			return E_FAIL;
	}

	return S_OK;
}

HRESULT CShockwave::Bind_ShaderResources()
{
	/* 셰이더 전역변수로 던져야 할 값들을 던지자. */
	if (FAILED(m_pGameInstance->Bind_TransformToShader(GetShader(), "g_ViewMatrix", CPipeLine::D3DTS_VIEW)) ||
		FAILED(m_pGameInstance->Bind_TransformToShader(GetShader(), "g_ProjMatrix", CPipeLine::D3DTS_PROJ)) ||
		FAILED(GetShader()->Bind_RawValue("g_vCamPosition", &static_cast<const _float4&>(m_pGameInstance->Get_CamPosition_Float4()), sizeof(_float4))))
	{
		return E_FAIL;
	}

	srand(time(NULL));
	if (FAILED(GetTexture()->Bind_ShaderResource(GetShader(), "g_Texture", rand() % 4)))
	//if (FAILED(GetTexture()->Bind_ShaderResources(GetShader(), "g_Textures")))
		return E_FAIL;

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
