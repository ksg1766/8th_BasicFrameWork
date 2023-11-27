#include "stdafx.h"
#include "..\Public\SwordTrail.h"
#include "GameInstance.h"

CSwordTrail::CSwordTrail(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: Super(pDevice, pContext)
{
}

CSwordTrail::CSwordTrail(const CSwordTrail& rhs)
	: Super(rhs)
{
}

HRESULT CSwordTrail::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CSwordTrail::Initialize(void* pArg)
{
	if (FAILED(Ready_FixedComponents()))
		return E_FAIL;

	if (FAILED(Ready_Scripts()))
		return E_FAIL;

	SWORDTRAIL_DESC* pDesc = reinterpret_cast<SWORDTRAIL_DESC*>(pArg);

	m_pModel = pDesc->pModel;
	m_fLifeTime = pDesc->fLifeTime;
	GetTransform()->Set_WorldMatrix(pDesc->matCurWorld);
	m_matPreWorld = pDesc->matPreWorld;
	//m_tPrePrePreTweenDesc = pDesc->pPrePrePreTweenDesc;
	//m_tPrePreTweenDesc = pDesc->pPrePreTweenDesc;
	m_tPreTweenDesc = pDesc->pPreTweenDesc;
	m_tCurTweenDesc = pDesc->pCurTweenDesc;
	m_pOffsetTop = pDesc->pMatOffsetTop;
	m_pOffsetBottom = pDesc->pMatOffsetBottom;
	m_iBoneIndex = pDesc->iBoneIndex;

	return S_OK;
}

void CSwordTrail::Tick(const _float& fTimeDelta)
{
	m_fLifeTime -= fTimeDelta;

	if (m_fLifeTime < 0.f)
	{
		m_pGameInstance->DeleteObject(this);
		return;
	}

	Super::Tick(fTimeDelta);
}

void CSwordTrail::LateTick(const _float& fTimeDelta)
{
	Super::LateTick(fTimeDelta);

	GetRenderer()->Add_RenderGroup(CRenderer::RG_BLEND, this);
}

void CSwordTrail::DebugRender()
{
	Super::DebugRender();
}

HRESULT CSwordTrail::Render()
{
	if (nullptr == GetBuffer() || nullptr == GetShader())
		return E_FAIL;

	if (FAILED(Bind_ShaderResources()))
		return E_FAIL;

	if (FAILED(GetShader()->Begin()))
		return E_FAIL;

	if (FAILED(GetBuffer()->Render()))
		return E_FAIL;

#ifdef _DEBUG
	DebugRender();
#endif

	return S_OK;
}

HRESULT CSwordTrail::Ready_FixedComponents()
{
	/* Com_Shader */
	if (FAILED(Super::AddComponent(LEVEL_STATIC, ComponentType::Shader, TEXT("Prototype_Component_Shader_SwordTrail"))))
		return E_FAIL;

	/* Com_VIBuffer */
	//if (FAILED(Super::AddComponent(LEVEL_STATIC, ComponentType::Buffer, TEXT("Prototype_Component_VIBuffer_VIBuffer_Point"))))
	if (FAILED(Super::AddComponent(LEVEL_STATIC, ComponentType::Buffer, TEXT("Prototype_Component_VIBuffer_Dot"))))
		return E_FAIL;
	
	/* Com_Transform */
	if (FAILED(Super::AddComponent(LEVEL_STATIC, ComponentType::Transform, TEXT("Prototype_Component_Transform"))))
		return E_FAIL;

	/* Com_Renderer */
	if (FAILED(Super::AddComponent(LEVEL_STATIC, ComponentType::Renderer, TEXT("Prototype_Component_Renderer"))))
		return E_FAIL;

	/* Com_Texture */
	if (FAILED(Super::AddComponent(LEVEL_GAMEPLAY, ComponentType::Texture, TEXT("Prototype_Component_Texture_SwordTrail_Gradient"))))
		return E_FAIL;

	m_pAlphaTexture = static_cast<CTexture*>(m_pGameInstance->Clone_Component(this, LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_SwordTrail_Mask")));
	if (!m_pAlphaTexture) return E_FAIL;

	return S_OK;
}

HRESULT CSwordTrail::Ready_Scripts()
{


	return S_OK;
}

HRESULT CSwordTrail::Bind_ShaderResources()
{
	if (FAILED(GetTransform()->Bind_ShaderResources(GetShader(), "g_WorldMatrix")) ||
		FAILED(m_pGameInstance->Bind_TransformToShader(GetShader(), "g_ViewMatrix", CPipeLine::D3DTS_VIEW)) ||
		FAILED(m_pGameInstance->Bind_TransformToShader(GetShader(), "g_ProjMatrix", CPipeLine::D3DTS_PROJ)) ||
		FAILED(GetTexture()->Bind_ShaderResource(GetShader(), "g_DiffuseTexture", 0)) ||
		FAILED(m_pAlphaTexture->Bind_ShaderResource(GetShader(), "g_AlphaTexture", 0)))
		return E_FAIL;

	if (FAILED(GetShader()->Bind_Matrix("g_preWorldMatrix", &m_matPreWorld)) ||
		//FAILED(GetShader()->Bind_RawValue("g_preTweenframes", &m_tPrePrePreTweenDesc, sizeof(TWEENDESC))) ||
		//FAILED(GetShader()->Bind_RawValue("g_preTweenframes", &m_tPrePreTweenDesc, sizeof(TWEENDESC))) ||
		FAILED(GetShader()->Bind_RawValue("g_preTweenframes", &m_tPreTweenDesc, sizeof(TWEENDESC))) ||
		FAILED(GetShader()->Bind_RawValue("g_Tweenframes", &m_tCurTweenDesc, sizeof(TWEENDESC))) ||
		FAILED(GetShader()->Bind_Texture("g_TransformMap", m_pModel->GetSRV())) ||
		FAILED(GetShader()->Bind_Matrix("g_matOffsetTop", m_pOffsetTop)) ||
		FAILED(GetShader()->Bind_Matrix("g_matOffsetBottom", m_pOffsetBottom)) ||
		FAILED(GetShader()->Bind_RawValue("g_iSocketBoneIndex", &m_iBoneIndex, sizeof(_int)))
		)
		return E_FAIL;

	return S_OK;
}

CSwordTrail* CSwordTrail::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CSwordTrail* pInstance = new CSwordTrail(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CSwordTrail");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CSwordTrail::Clone(void* pArg)
{
	CSwordTrail* pInstance = new CSwordTrail(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CSwordTrail");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CSwordTrail::Free()
{
	Super::Free();
}
