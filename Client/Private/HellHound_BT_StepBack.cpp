#include "stdafx.h"
#include "HellHound_BT_StepBack.h"
#include "GameInstance.h"
#include "GameObject.h"
#include "MonsterController.h"

CHellHound_BT_StepBack::CHellHound_BT_StepBack()
{
}

void CHellHound_BT_StepBack::OnStart()
{
	Super::OnStart(0);
}

CBT_Node::BT_RETURN CHellHound_BT_StepBack::OnUpdate(const _float& fTimeDelta)
{
	ConditionalAbort(fTimeDelta);

	if (m_fTimeSum > m_vecAnimIndexTime[0].second * 0.8f)
	{
		StartCoolDown();
		return BT_SUCCESS;
	}

	CMonsterController* pController = static_cast<CMonsterController*>(m_pController);
	pController->GetTranslateMessage(-m_pGameObject->GetTransform()->GetForward());
	
	m_fTimeSum += fTimeDelta;
	RunAttackCoolDown(fTimeDelta);

	return BT_RUNNING;

	// 스텝백으로 넘어온 이상 Attack이 쿨타임이란 뜻이니 여기랑 Wait 둘 다 마지막 부분에 쿨 계속 돌려주자
}

void CHellHound_BT_StepBack::OnEnd()
{
	Super::OnEnd();
}

void CHellHound_BT_StepBack::ConditionalAbort(const _float& fTimeDelta)
{
}

void CHellHound_BT_StepBack::StartCoolDown()
{
	BLACKBOARD& hashBlackBoard = m_pBehaviorTree->GetBlackBoard();
	const auto& tStepBackCoolDown = hashBlackBoard.find(TEXT("StepBackCoolDown"));

	if (tStepBackCoolDown == hashBlackBoard.end())
	{
		tagBlackBoardData<_float>* tStepBackCool = new tagBlackBoardData<_float>(3.f);
		hashBlackBoard.emplace(TEXT("StepBackCoolDown"), tStepBackCool);
	}
	else // 애초에 여기로 오면 잘못된 것.
	{
		__debugbreak();
		SET_VALUE(_float, tStepBackCoolDown, 3.f);
	}
}

void CHellHound_BT_StepBack::RunAttackCoolDown(const _float& fTimeDelta)
{
	BLACKBOARD& hashBlackBoard = m_pBehaviorTree->GetBlackBoard();
	const auto& tAttackCoolDown = hashBlackBoard.find(TEXT("AttackCoolDown"));

	if (tAttackCoolDown == hashBlackBoard.end())
	{
		return;
		//__debugbreak();
	}
	
	if (0.f > *GET_VALUE(_float, tAttackCoolDown))
	{
		hashBlackBoard.erase(tAttackCoolDown);
	}
	else
	{
		SET_VALUE(_float, tAttackCoolDown, *GET_VALUE(_float, tAttackCoolDown) - fTimeDelta);
	}
}

CHellHound_BT_StepBack* CHellHound_BT_StepBack::Create(CGameObject* pGameObject, CBehaviorTree* pBehaviorTree, const BEHAVEANIMS& tBehaveAnim, CMonoBehaviour* pController)
{
	CHellHound_BT_StepBack* pInstance = new CHellHound_BT_StepBack;

	if (FAILED(pInstance->Initialize(pGameObject, pBehaviorTree, tBehaveAnim, pController)))
	{
		MSG_BOX("Failed to Created : CHellHound_BT_StepBack");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CHellHound_BT_StepBack::Free()
{
	Super::Free();
}