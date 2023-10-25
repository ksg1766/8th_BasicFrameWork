#include "stdafx.h"
#include "..\Public\HellHound.h"
#include "GameInstance.h"
#include "BT_Composite.h"
#include "BT_Decorator.h"
#include "HellHound_BT_Spawn.h"
#include "HellHound_BT_Dead.h"
#include "HellHound_BT_Attack.h"
#include "HellHound_BT_StepBack.h"
#include "HellHound_BT_Wait.h"
#include "HellHound_BT_Chase.h"
#include "HellHound_BT_Sleep.h"
#include "HellHound_BT_Idle.h"
#include "HellHound_BT_Roaming.h"
#include "HellHound_BT_Hit.h"

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
	if (LEVEL_GAMEPLAY == m_pGameInstance->GetCurrentLevelIndex())
	{
		/* Com_MonsterController */
		if (FAILED(Super::AddComponent(LEVEL_GAMEPLAY, ComponentType::Script, TEXT("Prototype_Component_MonsterController"))))
			return E_FAIL;

		CMonoBehaviour* pController = m_vecScripts.back();

		/* Com_StateMachine */
		if (FAILED(Super::AddComponent(LEVEL_GAMEPLAY, ComponentType::Script, TEXT("Prototype_Component_BehaviorTree"))))
			return E_FAIL;

		CBehaviorTree* pBehaviorTree = dynamic_cast<CBehaviorTree*>(m_vecScripts.back());
		{
			CBT_Action::BEHAVEANIMS desc;
			
			desc.vecAnimations.clear();
			desc.vecAnimations.push_back(TEXT("HellHound_Death"));
			CBT_Action* pDead = CHellHound_BT_Dead::Create(this, pBehaviorTree, desc, pController);

			CBT_Decorator* pIfDead = CBT_Decorator::Create(this, pBehaviorTree, pController, CBT_Decorator::DecoratorType::IF);//죽었는가
			pIfDead->AddChild(pDead);

			desc.vecAnimations.clear();
			desc.vecAnimations.push_back(TEXT("HellHound_Atk_Bite"));
			CBT_Action* pAttack = CHellHound_BT_Attack::Create(this, pBehaviorTree, desc, pController);

			CBT_Decorator* pAtkCool = CBT_Decorator::Create(this, pBehaviorTree, pController, CBT_Decorator::DecoratorType::IF);//공격쿨
			if (FAILED(pAtkCool->AddChild(pAttack))) return E_FAIL;

			desc.vecAnimations.clear();
			desc.vecAnimations.push_back(TEXT("HellHound_Evade_B"));
			CBT_Action* pStepBack = CHellHound_BT_StepBack::Create(this, pBehaviorTree, desc, pController);

			CBT_Decorator* pStpBckCool = CBT_Decorator::Create(this, pBehaviorTree, pController, CBT_Decorator::DecoratorType::IF);//스텝백쿨
			if (FAILED(pStpBckCool->AddChild(pStepBack))) return E_FAIL;

			desc.vecAnimations.clear();
			desc.vecAnimations.push_back(TEXT("HellHound_Taunt_01"));
			CBT_Action* pWait = CHellHound_BT_Wait::Create(this, pBehaviorTree, desc, pController);

			CBT_Composite* pSuccessAtkRng = CBT_Composite::Create(this, pBehaviorTree, pController, CBT_Composite::CompositeType::SELECTOR);//사거리 내에 있을 경우
			if (FAILED(pSuccessAtkRng->AddChild(pAtkCool))) return E_FAIL;
			if (FAILED(pSuccessAtkRng->AddChild(pStpBckCool))) return E_FAIL;
			if (FAILED(pSuccessAtkRng->AddChild(pWait))) return E_FAIL;

			CBT_Decorator* pIfAtkRng = CBT_Decorator::Create(this, pBehaviorTree, pController, CBT_Decorator::DecoratorType::IF);//사거리
			if (FAILED(pIfAtkRng->AddChild(pSuccessAtkRng))) return E_FAIL;

			desc.vecAnimations.clear();
			desc.vecAnimations.push_back(TEXT("HellHound_Run_F"));
			CBT_Action* pChase = CHellHound_BT_Chase::Create(this, pBehaviorTree, desc, pController);

			CBT_Composite* pInSightTrue = CBT_Composite::Create(this, pBehaviorTree, pController, CBT_Composite::CompositeType::SELECTOR);//시야 내에 있을 경우
			if (FAILED(pInSightTrue->AddChild(pIfAtkRng))) return E_FAIL;
			if (FAILED(pInSightTrue->AddChild(pChase))) return E_FAIL;

			CBT_Decorator* pIfInSight = CBT_Decorator::Create(this, pBehaviorTree, pController, CBT_Decorator::DecoratorType::IF);//If 시야내 or Hit (==Awake)
			if (FAILED(pIfInSight->AddChild(pInSightTrue))) return E_FAIL;

			//desc.vecAnimations.clear();
			//desc.vecAnimations.push_back(TEXT("HellHound_Sleeping_Idle"));
			//CBT_Action* pSleep = CHellHound_BT_Sleep::Create(this, pBehaviorTree, desc, pController);

			//CBT_Decorator* pIfSleep = CBT_Decorator::Create(this, pBehaviorTree, pController, CBT_Decorator::DecoratorType::IF);//if 자는가? // 아 맞다 자는거 보류;;
			//if (FAILED(pIfSleep->AddChild(pSleep))) return E_FAIL;

			desc.vecAnimations.clear();
			desc.vecAnimations.push_back(TEXT("HellHound_Idle"));
			CBT_Action* pIdle = CHellHound_BT_Idle::Create(this, pBehaviorTree, desc, pController);

			desc.vecAnimations.clear();
			desc.vecAnimations.push_back(TEXT("HellHound_Walk_F"));
			desc.vecAnimations.push_back(TEXT("HellHound_Turn_90_L"));
			desc.vecAnimations.push_back(TEXT("HellHound_Turn_90_R"));
			CBT_Action* pRoaming = CHellHound_BT_Roaming::Create(this, pBehaviorTree, desc, pController);

			CBT_Composite* pPeace = CBT_Composite::Create(this, pBehaviorTree, pController, CBT_Composite::CompositeType::SEQUENCE); //평화로운 상태
			if (FAILED(pPeace->AddChild(pIdle))) return E_FAIL;
			if (FAILED(pPeace->AddChild(pRoaming))) return E_FAIL;
			
			// Spawn, Hit도 보류
			//desc.vecAnimations.push_back(TEXT("HellHound_Turn_90_R"));
			//desc.vecAnimations.push_back(TEXT("HellHound_Imapact_F"));

			CBT_Composite* pRoot = CBT_Composite::Create(this, pBehaviorTree, pController, CBT_Composite::CompositeType::SELECTOR);
			if (FAILED(pPeace->AddChild(pIfDead))) return E_FAIL;
			if (FAILED(pPeace->AddChild(pIfInSight))) return E_FAIL;
			if (FAILED(pPeace->AddChild(pPeace))) return E_FAIL;
			pBehaviorTree->SetRoot(pRoot);
		}
	}

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
