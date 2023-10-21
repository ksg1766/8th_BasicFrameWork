#include "stdafx.h"
#include "..\Public\P_Strife.h"
#include "GameInstance.h"
#include "StateMachine.h"
#include "P_Strife_State_Idle.h"
#include "P_Strife_State_Run.h"
#include "P_Strife_State_Aim.h"
#include "P_Strife_State_Jump.h"
#include "P_Strife_State_Dash.h"
#include "P_Strife_State_Impact.h"

CP_Strife::CP_Strife(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: Super(pDevice, pContext)
{
}

CP_Strife::CP_Strife(const CP_Strife& rhs)
	: Super(rhs)
{
}

HRESULT CP_Strife::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CP_Strife::Initialize(void* pArg)
{
	if (FAILED(Ready_FixedComponents()))
		return E_FAIL;

	if (FAILED(Ready_Scripts()))
		return E_FAIL;
	
	if (FAILED(Ready_Parts()))
		return E_FAIL;

	//GetRigidBody()->GetSphereCollider()->SetRadius(10.f);
	//GetRigidBody()->GetOBBCollider()->SetExtents(Vec3(5.f, 5.f, 5.f));
	//GetTransform()->SetScale(Vec3(2.f, 2.f, 2.f));

	return S_OK;
}

void CP_Strife::Tick(const _float& fTimeDelta)
{
	Super::Tick(fTimeDelta);
}

void CP_Strife::LateTick(const _float& fTimeDelta)
{
	Super::LateTick(fTimeDelta);

	GetRenderer()->Add_RenderGroup(CRenderer::RG_NONBLEND, this);
}

void CP_Strife::DebugRender()
{
	Super::DebugRender();
}

HRESULT CP_Strife::Render()
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

HRESULT CP_Strife::Ready_FixedComponents()
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
		/* Com_NavMeshAgent */
		if (FAILED(Super::AddComponent(LEVEL_GAMEPLAY, ComponentType::NavMeshAgent, TEXT("Prototype_Component_NavMeshAgent"))))
			return E_FAIL;
	}

	return S_OK;
}

HRESULT CP_Strife::Ready_Scripts()
{
	if (LEVEL_GAMEPLAY == m_pGameInstance->GetCurrentLevelIndex())
	{
		///* Com_PlayerController */
		if (FAILED(Super::AddComponent(LEVEL_GAMEPLAY, ComponentType::Script, TEXT("Prototype_Component_PlayerController"))))
			return E_FAIL;
	
		CMonoBehaviour* pController = m_vecScripts.back();

		///* Com_StateMachine */
		if (FAILED(Super::AddComponent(LEVEL_GAMEPLAY, ComponentType::Script, TEXT("Prototype_Component_StateMachine"))))
			return E_FAIL;
		
		CStateMachine* pStateMachine = dynamic_cast<CStateMachine*>(m_vecScripts.back());
		{
			CState::STATEANIMS desc;
			desc.vecAnimations.push_back(TEXT("Strife_Idle"));
			CState* pState = CP_Strife_State_Idle::Create(this, desc, pController);
			if (FAILED(pStateMachine->AddState(pState))) return E_FAIL;

			desc.vecAnimations.clear();
			desc.vecAnimations.push_back(TEXT("Strife_Run"));
			desc.vecAnimations.push_back(TEXT("Strife_Run_B"));
			pState = CP_Strife_State_Run::Create(this, desc, pController);
			if (FAILED(pStateMachine->AddState(pState))) return E_FAIL;

			desc.vecAnimations.clear();
			desc.vecAnimations.push_back(TEXT("Strife_Aim_Idle"));
			desc.vecAnimations.push_back(TEXT("Strife_Aim_Walk"));
			desc.vecAnimations.push_back(TEXT("Strife_Aim_Walk_Back"));
			pState = CP_Strife_State_Aim::Create(this, desc, pController);
			if (FAILED(pStateMachine->AddState(pState))) return E_FAIL;

			desc.vecAnimations.clear();
			desc.vecAnimations.push_back(TEXT("Strife_Jump"));
			desc.vecAnimations.push_back(TEXT("Strife_Land"));
			desc.vecAnimations.push_back(TEXT("Strife_Jump_Double"));
			desc.vecAnimations.push_back(TEXT("Strife_Jump_Land_Heavy"));
			desc.vecAnimations.push_back(TEXT("Strife_Fall"));
			pState = CP_Strife_State_Jump::Create(this, desc, pController);
			if (FAILED(pStateMachine->AddState(pState))) return E_FAIL;

			desc.vecAnimations.clear();
			desc.vecAnimations.push_back(TEXT("Strife_Dash"));
			desc.vecAnimations.push_back(TEXT("Strife_Dash_End"));
			desc.vecAnimations.push_back(TEXT("Strife_Dash_Back"));
			desc.vecAnimations.push_back(TEXT("Strife_Dash_Back_End"));
			pState = CP_Strife_State_Dash::Create(this, desc, pController);
			if (FAILED(pStateMachine->AddState(pState))) return E_FAIL;

			desc.vecAnimations.clear();
			desc.vecAnimations.push_back(TEXT("Strife_Impact_FromFront"));
			desc.vecAnimations.push_back(TEXT("Strife_Impact_FromBack"));
			desc.vecAnimations.push_back(TEXT("Strife_Impact_FromLeft"));
			desc.vecAnimations.push_back(TEXT("Strife_Impact_FromRight"));
			pState = CP_Strife_State_Impact::Create(this, desc, pController);
			if (FAILED(pStateMachine->AddState(pState))) return E_FAIL;
		}
	}

	return S_OK;
}

HRESULT CP_Strife::Ready_Parts()
{
	CGameObject* pGameObject = m_pGameInstance->CreateObject(TEXT("Prototype_GameObject_Strife_GunL"), LAYERTAG::IGNORECOLLISION);
	if (nullptr == pGameObject)	return E_FAIL;
	m_vecParts.push_back(pGameObject);
	GetModel()->EquipParts(0, pGameObject->GetModel());

	pGameObject = m_pGameInstance->CreateObject(TEXT("Prototype_GameObject_Strife_GunR"), LAYERTAG::IGNORECOLLISION);
	if (nullptr == pGameObject)	return E_FAIL;
	m_vecParts.push_back(pGameObject);
	GetModel()->EquipParts(1, pGameObject->GetModel());

	return S_OK;
}

HRESULT CP_Strife::Bind_ShaderResources()
{
	/* 셰이더 전역변수로 던져야 할 값들을 던지자. */
	if (FAILED(GetTransform()->Bind_ShaderResources(GetShader(), "g_WorldMatrix")) ||
		FAILED(m_pGameInstance->Bind_TransformToShader(GetShader(), "g_ViewMatrix", CPipeLine::D3DTS_VIEW)) ||
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

CP_Strife* CP_Strife::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CP_Strife* pInstance = new CP_Strife(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CP_Strife");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CP_Strife::Clone(void* pArg)
{
	CP_Strife* pInstance = new CP_Strife(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CP_Strife");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CP_Strife::Free()
{
	Super::Free();
}
