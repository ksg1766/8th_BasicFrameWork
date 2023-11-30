#include "stdafx.h"
#include "..\Public\Particle_Rain.h"
#include "GameInstance.h"
#include "VIBuffer_ParticleSystem.h"
#include "Shader.h"
#include "Utils.h"

CParticle_Rain::CParticle_Rain(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: Super(pDevice, pContext)
{
}

CParticle_Rain::CParticle_Rain(const CParticle_Rain& rhs)
	: Super(rhs)
{
}

HRESULT CParticle_Rain::Initialize_Prototype()
{
	srand(time(NULL));
	return S_OK;
}

HRESULT CParticle_Rain::Initialize(void* pArg)
{
	if (FAILED(Ready_FixedComponents()))
		return E_FAIL;

	if (FAILED(Ready_Scripts()))
		return E_FAIL;

	return S_OK;
}

void CParticle_Rain::Tick(const _float& fTimeDelta)
{
	Super::Tick(fTimeDelta);

	if (m_fLifeTime > 15.f)
		m_pGameInstance->DeleteObject(this);

	m_fTimeDelta = fTimeDelta;
	m_fLifeTime += m_fTimeDelta;
}

void CParticle_Rain::LateTick(const _float& fTimeDelta)
{
	Super::LateTick(fTimeDelta);

	GetRenderer()->Add_RenderGroup(CRenderer::RG_BLEND, this);
}

void CParticle_Rain::DebugRender()
{
	Super::DebugRender();
}

HRESULT CParticle_Rain::Render()
{
	if (nullptr == GetBuffer() || nullptr == GetShader())
		return E_FAIL;

	if (FAILED(Bind_ShaderResources()))
		return E_FAIL;

#ifdef _DEBUG
	DebugRender();
#endif

	if (FAILED(static_cast<CVIBuffer_ParticleSystem*>(GetBuffer())->Render(m_pShaderStream, GetShader())))
		return E_FAIL;

	return S_OK;
}

HRESULT CParticle_Rain::Ready_FixedComponents()
{
	/* Com_Shader */
	if (FAILED(Super::AddComponent(LEVEL_STATIC, ComponentType::Shader, TEXT("Prototype_Component_Shader_ParticleSystem_Draw"))))
		return E_FAIL;
	m_pShaderStream = static_cast<CShader*>(m_pGameInstance->Clone_Component(this, LEVEL_STATIC, TEXT("Prototype_Component_Shader_ParticleSystem_Stream")));
	if (nullptr == m_pShaderStream)
		return E_FAIL;

	/* Com_VIBuffer */
	if (FAILED(Super::AddComponent(LEVEL_STATIC, ComponentType::Buffer, TEXT("Prototype_Component_VIBuffer_ParticleSystem"))))
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
	if (FAILED(Super::AddComponent(LEVEL_GAMEPLAY, ComponentType::Texture, TEXT("Prototype_Component_Texture_Raindrop"))))
		return E_FAIL;

	Utils::CreateRandomTexture1DSRV(m_pDevice, &m_pRandomTexture);
	//m_pRandomTexture = static_cast<CTexture*>(m_pGameInstance->Clone_Component(this, LEVEL_STATIC, TEXT("Prototype_Component_Texture_Smoke_Tiled")));
	if (nullptr == m_pRandomTexture)
		return E_FAIL;

	return S_OK;
}

HRESULT CParticle_Rain::Ready_Scripts()
{
	if (LEVEL_GAMEPLAY == m_pGameInstance->GetCurrentLevelIndex())
	{

	}

	return S_OK;
}

HRESULT CParticle_Rain::Bind_ShaderResources()
{
	/* 셰이더 전역변수로 던져야 할 값들을 던지자. */
	if (FAILED(m_pGameInstance->Bind_TransformToShader(GetShader(), "g_ViewMatrix", CPipeLine::D3DTS_VIEW)) ||
		FAILED(m_pGameInstance->Bind_TransformToShader(GetShader(), "g_ProjMatrix", CPipeLine::D3DTS_PROJ)) ||
		FAILED(GetShader()->Bind_RawValue("g_vCamPosition", &static_cast<const _float4&>(m_pGameInstance->Get_CamPosition_Float4()), sizeof(_float4)))/* ||
		FAILED(m_pShaderStream->Bind_RawValue("g_vCamPosition", &static_cast<const _float4&>(m_pGameInstance->Get_CamPosition_Float4()), sizeof(_float4)))*/)
		return E_FAIL;

	Vec3 vEmitPosition = GetTransform()->GetPosition();
	if (FAILED(m_pShaderStream->Bind_RawValue("g_vEmitPosition", &vEmitPosition, sizeof(_float3))) ||
		//FAILED(m_pShaderStream->Bind_RawValue("g_vEmitDirection", &, sizeof(_float3))) ||
		FAILED(m_pShaderStream->Bind_RawValue("g_fGameTime", &m_fLifeTime, sizeof(_float))) ||
		FAILED(m_pShaderStream->Bind_RawValue("g_fTimeStep", &m_fTimeDelta, sizeof(_float))))
		return E_FAIL;

	if (FAILED(GetTexture()->Bind_ShaderResource(GetShader(), "g_Texture", 0)) ||
		FAILED(m_pShaderStream->Bind_Texture("g_RandomTexture", m_pRandomTexture)))
		return E_FAIL;

	return S_OK;
}

CParticle_Rain* CParticle_Rain::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CParticle_Rain* pInstance = new CParticle_Rain(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CParticle_Rain");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CParticle_Rain::Clone(void* pArg)
{
	CParticle_Rain* pInstance = new CParticle_Rain(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CParticle_Rain");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CParticle_Rain::Free()
{
	Safe_Release(m_pShaderStream);
	Safe_Release(m_pRandomTexture);
	Super::Free();
}
