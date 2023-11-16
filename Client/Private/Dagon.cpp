#include "stdafx.h"
#include "..\Public\Dagon.h"
#include "GameInstance.h"
#include "BossController.h"
#include "MonsterStats.h"

#include "BT_Composite.h"
#include "Dagon_BT_IF_Dead.h"

#include "Dagon_BT_Dead.h"
#include "Dagon_BT_Idle.h"
#include "Dagon_BT_Lightning.h"
#include "Dagon_BT_Orb.h"
#include "Dagon_BT_Tentacle.h"
#include "Dagon_BT_Wave.h"
#include "Dagon_BT_WaveLoop.h"
#include "Dagon_BT_WhirlPool.h"

CDagon::CDagon(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: Super(pDevice, pContext)
{
}

CDagon::CDagon(const CDagon& rhs)
	: Super(rhs)
{
}

HRESULT CDagon::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CDagon::Initialize(void* pArg)
{
	if (FAILED(Ready_FixedComponents()))
		return E_FAIL;

	if (FAILED(Ready_Scripts()))
		return E_FAIL;

	GetRigidBody()->GetSphereCollider()->SetRadius(7.f);
	GetRigidBody()->GetOBBCollider()->SetExtents(Vec3(4.f, 4.f, 4.f));

	GetTransform()->RotateYAxisFixed(Vec3(0.f, 180.f, 0.f));
	GetTransform()->Translate(Vec3(-147.f, 28.f, 237.f));

	return S_OK;
}

void CDagon::Tick(const _float& fTimeDelta)
{
	Super::Tick(fTimeDelta);
}

void CDagon::LateTick(const _float& fTimeDelta)
{
	Super::LateTick(fTimeDelta);

	GetRenderer()->Add_RenderGroup(CRenderer::RG_NONBLEND, this);
}

void CDagon::DebugRender()
{
	Super::DebugRender();
}

HRESULT CDagon::Render()
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

HRESULT CDagon::Ready_FixedComponents()
{
	/* Com_Shader */
	if (FAILED(Super::AddComponent(LEVEL_STATIC, ComponentType::Shader, TEXT("Prototype_Component_Shader_VtxTexFetchAnim"))))
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

	if (LEVEL_GAMEPLAY == m_pGameInstance->GetCurrentLevelIndex())
	{
	}

	return S_OK;
}

HRESULT CDagon::Ready_Scripts()
{
	if (LEVEL_GAMEPLAY == m_pGameInstance->GetCurrentLevelIndex())
	{
		/* Com_BossController */
		if (FAILED(Super::AddComponent(LEVEL_GAMEPLAY, ComponentType::Script, TEXT("Prototype_Component_BossController"))))
			return E_FAIL;

		m_pController = dynamic_cast<CBossController*>(m_vecScripts[0]);

		/* Com_MonsterStats */
		CMonsterStats::MONSTERSTAT stats = { 3000, 20 };
		if (FAILED(Super::AddComponent(LEVEL_GAMEPLAY, ComponentType::Script, TEXT("Prototype_Component_MonsterStats"), &stats)))
			return E_FAIL;

		m_pController->SetStats(dynamic_cast<CMonsterStats*>(m_vecScripts[1]));

		/* Com_BehaviorTree */
		if (FAILED(Super::AddComponent(LEVEL_GAMEPLAY, ComponentType::Script, TEXT("Prototype_Component_BehaviorTree"))))
			return E_FAIL;

		CBehaviorTree* pBehaviorTree = dynamic_cast<CBehaviorTree*>(m_vecScripts[2]);
		{
			CBT_Action::BEHAVEANIMS desc;

			desc.vecAnimations.clear();
			desc.vecAnimations.push_back(TEXT("WaterBoss_Impact_Stun"));
			CBT_Action* pDead = CDagon_BT_Dead::Create(this, pBehaviorTree, desc, m_pController);

			CBT_Decorator* pIfDead = CDagon_BT_IF_Dead::Create(this, pBehaviorTree, m_pController, CBT_Decorator::DecoratorType::IF);
			pIfDead->AddChild(pDead);

			desc.vecAnimations.clear();
			desc.vecAnimations.push_back(TEXT("WaterBoss_Idle"));
			CBT_Action* pIdle = CDagon_BT_Idle::Create(this, pBehaviorTree, desc, m_pController);

			//
			desc.vecAnimations.clear();
			desc.vecAnimations.push_back(TEXT("WaterBoss_Atk_CallLightning"));
			CBT_Action* pLightning = CDagon_BT_Lightning::Create(this, pBehaviorTree, desc, m_pController);

			desc.vecAnimations.clear();
			desc.vecAnimations.push_back(TEXT("WaterBoss_Atk_SummonOrb_L"));
			CBT_Action* pOrb = CDagon_BT_Orb::Create(this, pBehaviorTree, desc, m_pController);

			desc.vecAnimations.clear();
			desc.vecAnimations.push_back(TEXT("WaterBoss_Atk_WhirlPool"));
			CBT_Action* pPool = CDagon_BT_WhirlPool::Create(this, pBehaviorTree, desc, m_pController);

			desc.vecAnimations.clear();
			desc.vecAnimations.push_back(TEXT("WaterBoss_TidalWave_Loop"));
			CBT_Action* pWaveLoop = CDagon_BT_WaveLoop::Create(this, pBehaviorTree, desc, m_pController);

			desc.vecAnimations.clear();
			desc.vecAnimations.push_back(TEXT("WaterBoss_TidalWave"));
			CBT_Action* pWave = CDagon_BT_Wave::Create(this, pBehaviorTree, desc, m_pController);

			CBT_Composite* pWaveSequence = CBT_Composite::Create(this, pBehaviorTree, m_pController, CBT_Composite::CompositeType::SEQUENCE);
			pWaveSequence->AddChild(pWaveLoop);
			pWaveSequence->AddChild(pWave);

			desc.vecAnimations.clear();
			desc.vecAnimations.push_back(TEXT("WaterBoss_Atk_Tentacle_Pummel"));
			CBT_Action* pTentacle = CDagon_BT_Tentacle::Create(this, pBehaviorTree, desc, m_pController);

			CBT_Composite* pAttack = CBT_Composite::Create(this, pBehaviorTree, m_pController, CBT_Composite::CompositeType::SELECTOR);
			pAttack->AddChild(pLightning);
			pAttack->AddChild(pOrb);
			pAttack->AddChild(pPool);
			pAttack->AddChild(pWaveSequence);
			pAttack->AddChild(pTentacle);

			CBT_Composite* pRun = CBT_Composite::Create(this, pBehaviorTree, m_pController, CBT_Composite::CompositeType::SEQUENCE);
			pRun->AddChild(pIdle);
			pRun->AddChild(pAttack);

			CBT_Composite* pRoot = CBT_Composite::Create(this, pBehaviorTree, m_pController, CBT_Composite::CompositeType::SELECTOR);
			pRoot->AddChild(pIfDead);
			pRoot->AddChild(pRun);
			pBehaviorTree->SetRoot(pRoot);
		}
	}

	return S_OK;
}

void CDagon::OnCollisionEnter(CGameObject* pOther)
{
	m_pController->OnCollisionEnter(pOther);
}

void CDagon::OnCollisionStay(CGameObject* pOther)
{
	m_pController->OnCollisionStay(pOther);
}

void CDagon::OnCollisionExit(CGameObject* pOther)
{
	m_pController->OnCollisionExit(pOther);
}

HRESULT CDagon::Bind_ShaderResources()
{
	/* 셰이더 전역변수로 던져야 할 값들을 던지자. */
	if (FAILED(GetTransform()->Bind_ShaderResources(GetShader(), "g_WorldMatrix")) ||
		FAILED(m_pGameInstance->Bind_TransformToShader(GetShader(), "g_ViewMatrix", CPipeLine::D3DTS_VIEW)) ||
		FAILED(m_pGameInstance->Bind_TransformToShader(GetShader(), "g_ProjMatrix", CPipeLine::D3DTS_PROJ)) ||
		FAILED(GetShader()->Bind_RawValue("g_vCamPosition", &static_cast<const _float4&>(m_pGameInstance->Get_CamPosition_Float4()), sizeof(_float4))))
	{
		return E_FAIL;
	}

	return S_OK;
}

CDagon* CDagon::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CDagon* pInstance = new CDagon(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CDagon");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CDagon::Clone(void* pArg)
{
	CDagon* pInstance = new CDagon(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CDagon");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CDagon::Free()
{
	Super::Free();
}
