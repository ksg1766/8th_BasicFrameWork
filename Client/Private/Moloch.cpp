#include "stdafx.h"
#include "..\Public\Moloch.h"
#include "GameInstance.h"
#include "BossController.h"
#include "MonsterStats.h"

#include "BT_Composite.h"

CMoloch::CMoloch(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: Super(pDevice, pContext)
{
}

CMoloch::CMoloch(const CMoloch& rhs)
	: Super(rhs)
{
}

HRESULT CMoloch::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CMoloch::Initialize(void* pArg)
{
	if (FAILED(Ready_FixedComponents()))
		return E_FAIL;

	if (FAILED(Ready_Scripts()))
		return E_FAIL;

	if (FAILED(Ready_Parts()))
		return E_FAIL;

	GetRigidBody()->GetSphereCollider()->SetRadius(2.5f);
	GetRigidBody()->GetOBBCollider()->SetExtents(Vec3(2.f, 2.5f, 2.f));

	return S_OK;
}

void CMoloch::Tick(const _float& fTimeDelta)
{
	Super::Tick(fTimeDelta);
}

void CMoloch::LateTick(const _float& fTimeDelta)
{
	Super::LateTick(fTimeDelta);

	GetRenderer()->Add_RenderGroup(CRenderer::RG_NONBLEND, this);
}

void CMoloch::DebugRender()
{
	Super::DebugRender();
}

HRESULT CMoloch::Render()
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

HRESULT CMoloch::Ready_Parts()
{
	CGameObject* pGameObject = m_pGameInstance->CreateObject(TEXT("Prototype_GameObject_Moloch_Sword"), LAYERTAG::IGNORECOLLISION);
	if (nullptr == pGameObject)	return E_FAIL;
	m_vecParts.push_back(pGameObject);
	GetModel()->EquipParts(0, pGameObject->GetModel());

	return S_OK;
}

HRESULT CMoloch::Ready_FixedComponents()
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
		CNavMeshAgent::NAVIGATION_DESC pNaviDesc;
		pNaviDesc.iCurrentIndex = 10;

		if (FAILED(Super::AddComponent(LEVEL_GAMEPLAY, ComponentType::NavMeshAgent, TEXT("Prototype_Component_NavMeshAgent"), &pNaviDesc)))
			return E_FAIL;
	}

	return S_OK;
}

HRESULT CMoloch::Ready_Scripts()
{
	if (LEVEL_GAMEPLAY == m_pGameInstance->GetCurrentLevelIndex())
	{
		/* Com_BossController */
		//if (FAILED(Super::AddComponent(LEVEL_GAMEPLAY, ComponentType::Script, TEXT("Prototype_Component_BossController"))))
		//	return E_FAIL;

		//m_pController = dynamic_cast<CBossController*>(m_vecScripts[0]);

		///* Com_MonsterStats */
		//CMonsterStats::MONSTERSTAT stats = { 2000, 20 };
		//if (FAILED(Super::AddComponent(LEVEL_GAMEPLAY, ComponentType::Script, TEXT("Prototype_Component_MonsterStats"), &stats)))
		//	return E_FAIL;

		//m_pController->SetStats(dynamic_cast<CMonsterStats*>(m_vecScripts[1]));

		///* Com_BehaviorTree */
		//if (FAILED(Super::AddComponent(LEVEL_GAMEPLAY, ComponentType::Script, TEXT("Prototype_Component_BehaviorTree"))))
		//	return E_FAIL;

		//CBehaviorTree* pBehaviorTree = dynamic_cast<CBehaviorTree*>(m_vecScripts[2]);
		//{


		//	BLACKBOARD& hashBlackBoard = pBehaviorTree->GetBlackBoard();
		//	hashBlackBoard.emplace(TEXT("Sight"), new tagBlackBoardData<_float>(30.f));
		//}
	}

	return S_OK;
}

void CMoloch::OnCollisionEnter(CGameObject* pOther)
{
	m_pController->OnCollisionEnter(pOther);
}

void CMoloch::OnCollisionStay(CGameObject* pOther)
{
	m_pController->OnCollisionStay(pOther);
}

void CMoloch::OnCollisionExit(CGameObject* pOther)
{
	m_pController->OnCollisionExit(pOther);
}

HRESULT CMoloch::Bind_ShaderResources()
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

CMoloch* CMoloch::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CMoloch* pInstance = new CMoloch(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CMoloch");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CMoloch::Clone(void* pArg)
{
	CMoloch* pInstance = new CMoloch(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CMoloch");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CMoloch::Free()
{
	Super::Free();
}
