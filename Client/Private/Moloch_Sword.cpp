#include "stdafx.h"
#include "..\Public\Moloch_Sword.h"
#include "GameInstance.h"
#include "Fire.h"

CMoloch_Sword::CMoloch_Sword(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: Super(pDevice, pContext)
{
}

CMoloch_Sword::CMoloch_Sword(const CMoloch_Sword& rhs)
	: Super(rhs)
{
}

HRESULT CMoloch_Sword::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CMoloch_Sword::Initialize(void* pArg)
{
	if (FAILED(Ready_FixedComponents()))
		return E_FAIL;

	if (FAILED(Ready_Scripts()))
		return E_FAIL;

	CFire* pFire = static_cast<CFire*>(m_pGameInstance->CreateObject(TEXT("Prototype_GameObject_Fire"), LAYERTAG::IGNORECOLLISION));

	m_vecFires.push_back(pFire);

	return S_OK;
}

void CMoloch_Sword::Tick(const _float& fTimeDelta)
{
	for (auto& iter : m_vecFires)
		iter->GetTransform()->SetPosition(GetTransform()->GetPosition());

	Super::Tick(fTimeDelta);
}

void CMoloch_Sword::LateTick(const _float& fTimeDelta)
{
	Super::LateTick(fTimeDelta);

	GetRenderer()->Add_RenderGroup(CRenderer::RG_NONBLEND, this);
}

void CMoloch_Sword::DebugRender()
{
	Super::DebugRender();
}

HRESULT CMoloch_Sword::Render()
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

HRESULT CMoloch_Sword::Ready_FixedComponents()
{
	/* Com_Shader */
	if (FAILED(Super::AddComponent(LEVEL_STATIC, ComponentType::Shader, TEXT("Prototype_Component_Shader_VTFSocket"))))
		return E_FAIL;

	/* Com_Model */
	if (FAILED(Super::AddComponent(LEVEL_STATIC, ComponentType::Model, TEXT("Prototype_Component_Model_") + GetObjectTag())))
		return E_FAIL;

	/* Com_Model */
	if (FAILED(Super::AddComponent(LEVEL_STATIC, ComponentType::Texture, TEXT("Prototype_Component_Texture_Moloch_Sword_emissive"))))
		return E_FAIL;

	/* Com_Transform */
	if (FAILED(Super::AddComponent(LEVEL_STATIC, ComponentType::Transform, TEXT("Prototype_Component_Transform"))))
		return E_FAIL;

	/* Com_Renderer */
	if (FAILED(Super::AddComponent(LEVEL_STATIC, ComponentType::Renderer, TEXT("Prototype_Component_Renderer"))))
		return E_FAIL;

	/* Com_RigidBody */	// 총이라 필요없음

	return S_OK;
}

HRESULT CMoloch_Sword::Ready_Scripts()
{
	return S_OK;
}

HRESULT CMoloch_Sword::Bind_ShaderResources()
{
	/* 셰이더 전역변수로 던져야 할 값들을 던지자. */
	if (FAILED(GetTransform()->Bind_ShaderResources(GetShader(), "g_WorldMatrix")) || 
		FAILED(m_pGameInstance->Bind_TransformToShader(GetShader(), "g_ViewMatrix", CPipeLine::D3DTS_VIEW)) ||
		FAILED(m_pGameInstance->Bind_TransformToShader(GetShader(), "g_ProjMatrix", CPipeLine::D3DTS_PROJ)))
	{
		return E_FAIL;
	}

	if (FAILED(GetTexture()->Bind_ShaderResource(GetShader(), "g_EmissiveTexture", 0)))
		return E_FAIL;

	return S_OK;
}

CMoloch_Sword* CMoloch_Sword::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CMoloch_Sword* pInstance = new CMoloch_Sword(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CMoloch_Sword");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CMoloch_Sword::Clone(void* pArg)
{
	CMoloch_Sword* pInstance = new CMoloch_Sword(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CMoloch_Sword");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CMoloch_Sword::Free()
{
	Super::Free();
}
