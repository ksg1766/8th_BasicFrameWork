#include "stdafx.h"
#include "Moloch_BT_Idle.h"
#include "GameInstance.h"
#include "Layer.h"
#include "GameObject.h"
#include "MonsterController.h"

CMoloch_BT_Idle::CMoloch_BT_Idle()
{
}

void CMoloch_BT_Idle::OnStart()
{
	Super::OnStart(0);
	StartIdleCoolDown();
}

CBT_Node::BT_RETURN CMoloch_BT_Idle::OnUpdate(const _float& fTimeDelta)
{
	// TODO: 이걸 시퀀스 단계에서 판단할 수 있도록 해야 함. // Abort 반환 결과를 받아서 정지시키도록.
	if (IsZeroHP())
		return BT_FAIL;

	BLACKBOARD& hashBlackBoard = m_pBehaviorTree->GetBlackBoard();
	const auto& tIdleCoolDown = hashBlackBoard.find(TEXT("IdleCoolDown"));

	if (tIdleCoolDown == hashBlackBoard.end())
		return BT_SUCCESS;

	RunIdleCoolDown(fTimeDelta);

	return BT_RUNNING;
}

void CMoloch_BT_Idle::OnEnd()
{
	//AbortIdleCoolDown();
	Super::OnEnd();
}

void CMoloch_BT_Idle::ConditionalAbort(const _float& fTimeDelta)
{
}

void CMoloch_BT_Idle::StartIdleCoolDown()
{
	BLACKBOARD& hashBlackBoard = m_pBehaviorTree->GetBlackBoard();
	const auto& tIdleCoolDown = hashBlackBoard.find(TEXT("IdleCoolDown"));

	if (tIdleCoolDown == hashBlackBoard.end())
	{
		tagBlackBoardData<_float>* tStepBackCool = new tagBlackBoardData<_float>(2.f);
		hashBlackBoard.emplace(TEXT("IdleCoolDown"), tStepBackCool);
	}
	else
	{
		__debugbreak();
		//SET_VALUE(_float, tIdleCoolDown, 2.f);
	}
}

void CMoloch_BT_Idle::AbortIdleCoolDown()
{
	BLACKBOARD& hashBlackBoard = m_pBehaviorTree->GetBlackBoard();
	const auto& tIdleCoolDown = hashBlackBoard.find(TEXT("IdleCoolDown"));

	if (tIdleCoolDown != hashBlackBoard.end())
	{
		hashBlackBoard.erase(tIdleCoolDown);
	}
}

void CMoloch_BT_Idle::RunIdleCoolDown(const _float& fTimeDelta)
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

_bool CMoloch_BT_Idle::IsZeroHP()
{
	if (static_cast<CMonsterController*>(m_pController)->IsZeroHP())
		return true;

	return false;
}

CMoloch_BT_Idle* CMoloch_BT_Idle::Create(CGameObject* pGameObject, CBehaviorTree* pBehaviorTree, const BEHAVEANIMS& tBehaveAnim, CMonoBehaviour* pController)
{
	CMoloch_BT_Idle* pInstance = new CMoloch_BT_Idle;

	if (FAILED(pInstance->Initialize(pGameObject, pBehaviorTree, tBehaveAnim, pController)))
	{
		MSG_BOX("Failed to Created : CMoloch_BT_Idle");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CMoloch_BT_Idle::Free()
{
	Super::Free();
}


