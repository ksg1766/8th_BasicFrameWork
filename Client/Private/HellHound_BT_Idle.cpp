#include "stdafx.h"
#include "HellHound_BT_Idle.h"
#include "GameInstance.h"
#include "GameObject.h"
#include "MonoBehaviour.h"

CHellHound_BT_Idle::CHellHound_BT_Idle()
{
}

void CHellHound_BT_Idle::OnStart()
{
	Super::OnStart(0);
	StartIdleCoolDown();
}

CBT_Node::BT_RETURN CHellHound_BT_Idle::OnUpdate(const _float& fTimeDelta)
{
	ConditionalAbort(fTimeDelta);
	// 맞거나 시야안에 플레이어 있으면 FAIL;
	// 시야 체크는 sequence를 상속해서 재정의 하지 않는 한 여기 클래스에 함수를 파줘야 할 듯.
	BLACKBOARD& hashBlackBoard = m_pBehaviorTree->GetBlackBoard();
	const auto& tIdleCoolDown = hashBlackBoard.find(TEXT("IdleCoolDown"));

	if (tIdleCoolDown == hashBlackBoard.end())
		return BT_SUCCESS;

	RunIdleCoolDown(fTimeDelta);

	return BT_RUNNING;
}

void CHellHound_BT_Idle::OnEnd()
{
	Super::OnEnd();
}

void CHellHound_BT_Idle::ConditionalAbort(const _float& fTimeDelta)
{
}

void CHellHound_BT_Idle::StartIdleCoolDown()
{
	BLACKBOARD& hashBlackBoard = m_pBehaviorTree->GetBlackBoard();
	const auto& tIdleCoolDown = hashBlackBoard.find(TEXT("IdleCoolDown"));

	if (tIdleCoolDown == hashBlackBoard.end())
	{
		tagBlackBoardData<_float>* tStepBackCool = new tagBlackBoardData<_float>(3.f);
		hashBlackBoard.emplace(TEXT("IdleCoolDown"), tStepBackCool);
	}
	else
	{
		__debugbreak();
		SET_VALUE(_float, tIdleCoolDown, 3.f);
	}
}

void CHellHound_BT_Idle::RunIdleCoolDown(const _float& fTimeDelta)
{
	BLACKBOARD& hashBlackBoard = m_pBehaviorTree->GetBlackBoard();
	const auto& tIdleCoolDown = hashBlackBoard.find(TEXT("IdleCoolDown"));

	if (0.f > *GET_VALUE(_float, tIdleCoolDown))
	{
		hashBlackBoard.erase(tIdleCoolDown);
	}
	else
	{
		SET_VALUE(_float, tIdleCoolDown, *GET_VALUE(_float, tIdleCoolDown) - fTimeDelta);
	}
}

CHellHound_BT_Idle* CHellHound_BT_Idle::Create(CGameObject* pGameObject, CBehaviorTree* pBehaviorTree, const BEHAVEANIMS& tBehaveAnim, CMonoBehaviour* pController)
{
	CHellHound_BT_Idle* pInstance = new CHellHound_BT_Idle;

	if (FAILED(pInstance->Initialize(pGameObject, pBehaviorTree, tBehaveAnim, pController)))
	{
		MSG_BOX("Failed to Created : CHellHound_BT_Idle");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CHellHound_BT_Idle::Free()
{
	Super::Free();
}


