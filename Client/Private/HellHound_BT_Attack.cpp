#include "stdafx.h"
#include "HellHound_BT_Attack.h"
#include "GameInstance.h"
#include "GameObject.h"
#include "MonsterController.h"

CHellHound_BT_Attack::CHellHound_BT_Attack()
{
}

void CHellHound_BT_Attack::OnStart()
{
	Super::OnStart(0);
}

CBT_Node::BT_RETURN CHellHound_BT_Attack::OnUpdate(const _float& fTimeDelta)
{
	ConditionalAbort(fTimeDelta);

	if (m_fTimeSum > m_vecAnimIndexTime[0].second * 0.8f)
	{
		CMonsterController* pController = static_cast<CMonsterController*>(m_pController);
		pController->GetAttackMessage();
		StartCoolDown();
		return BT_SUCCESS;
	}
	
	m_fTimeSum += fTimeDelta;
	RunStepBackCoolDown(fTimeDelta);

	return BT_RUNNING;
	// BT_FAIL 은 중간에 맞았을 때 해주자
}

void CHellHound_BT_Attack::OnEnd()
{
	Super::OnEnd();
}

void CHellHound_BT_Attack::ConditionalAbort(const _float& fTimeDelta)
{
}

void CHellHound_BT_Attack::StartCoolDown()
{
	BLACKBOARD& hashBlackBoard = m_pBehaviorTree->GetBlackBoard();
	const auto& tAttackCoolDown = hashBlackBoard.find(TEXT("AttackCoolDown"));

	if (tAttackCoolDown == hashBlackBoard.end())
	{
		tagBlackBoardData<_float>* tAttackCool = new tagBlackBoardData<_float>(3.f);
		hashBlackBoard.emplace(TEXT("AttackCoolDown"), tAttackCool);
	}
	else // 애초에 여기로 오면 잘못된 것.
	{
		__debugbreak();
		SET_VALUE(_float, tAttackCoolDown, 3.f);
	}
}

void CHellHound_BT_Attack::RunStepBackCoolDown(const _float& fTimeDelta)
{
	BLACKBOARD& hashBlackBoard = m_pBehaviorTree->GetBlackBoard();
	const auto& tStepBackCoolDown = hashBlackBoard.find(TEXT("StepBackCoolDown"));

	if (tStepBackCoolDown == hashBlackBoard.end())
	{
		return;
		//	__debugbreak();
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


CHellHound_BT_Attack* CHellHound_BT_Attack::Create(CGameObject* pGameObject, CBehaviorTree* pBehaviorTree, const BEHAVEANIMS& tBehaveAnim, CMonoBehaviour* pController)
{
	CHellHound_BT_Attack* pInstance = new CHellHound_BT_Attack;

	if (FAILED(pInstance->Initialize(pGameObject, pBehaviorTree, tBehaveAnim, pController)))
	{
		MSG_BOX("Failed to Created : CHellHound_BT_Attack");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CHellHound_BT_Attack::Free()
{
	Super::Free();
}