#include "stdafx.h"
#include "HellHound_BT_Wait.h"
#include "GameInstance.h"
#include "GameObject.h"
#include "MonoBehaviour.h"

CHellHound_BT_Wait::CHellHound_BT_Wait()
{
}

void CHellHound_BT_Wait::OnStart()
{
	Super::OnStart(0);
}

CBT_Node::BT_RETURN CHellHound_BT_Wait::OnUpdate(const _float& fTimeDelta)
{
	// 어택 쿨 도는한 여기서 머물러있도록
	BLACKBOARD& hashBlackBoard = m_pBehaviorTree->GetBlackBoard();
	const auto& tAttackCoolDown = hashBlackBoard.find(TEXT("AttackCoolDown"));
	
	if (tAttackCoolDown == hashBlackBoard.end())
	{
		return BT_SUCCESS;
	}

	RunAttackCoolDown(fTimeDelta);
	RunStepBackCoolDown(fTimeDelta);

	return BT_RUNNING;
}

void CHellHound_BT_Wait::OnEnd()
{
	Super::OnEnd();
}

void CHellHound_BT_Wait::ConditionalAbort(const _float& fTimeDelta)
{
}

void CHellHound_BT_Wait::RunAttackCoolDown(const _float& fTimeDelta)
{
	BLACKBOARD& hashBlackBoard = m_pBehaviorTree->GetBlackBoard();
	const auto& tAttackCoolDown = hashBlackBoard.find(TEXT("AttackCoolDown"));

	if (0.f > *GET_VALUE(_float, tAttackCoolDown))
	{
		hashBlackBoard.erase(tAttackCoolDown);
	}
	else
	{
		SET_VALUE(_float, tAttackCoolDown, *GET_VALUE(_float, tAttackCoolDown) - fTimeDelta);
	}
}

void CHellHound_BT_Wait::RunStepBackCoolDown(const _float& fTimeDelta)
{
	BLACKBOARD& hashBlackBoard = m_pBehaviorTree->GetBlackBoard();
	const auto& tStepBackCoolDown = hashBlackBoard.find(TEXT("StepBackCoolDown"));

	if (tStepBackCoolDown == hashBlackBoard.end())
	{
		return;
		//__debugbreak();
	}

	if (0.f > *GET_VALUE(_float, tStepBackCoolDown))
	{
		hashBlackBoard.erase(tStepBackCoolDown);
	}
	else
	{
		SET_VALUE(_float, tStepBackCoolDown, *GET_VALUE(_float, tStepBackCoolDown) - fTimeDelta);
	}
}

CHellHound_BT_Wait* CHellHound_BT_Wait::Create(CGameObject* pGameObject, CBehaviorTree* pBehaviorTree, const BEHAVEANIMS& tBehaveAnim, CMonoBehaviour* pController)
{
	CHellHound_BT_Wait* pInstance = new CHellHound_BT_Wait;

	if (FAILED(pInstance->Initialize(pGameObject, pBehaviorTree, tBehaveAnim, pController)))
	{
		MSG_BOX("Failed to Created : CHellHound_BT_Wait");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CHellHound_BT_Wait::Free()
{
	Super::Free();
}