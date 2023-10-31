#include "stdafx.h"
#include "HellHound_BT_Attack.h"
#include "GameInstance.h"
#include "GameObject.h"
#include "MonsterController.h"
// Temp
#include "HellHound.h"

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
	if (IsZeroHP())
		return BT_FAIL;

	if (m_fTimeSum > m_vecAnimIndexTime[0].second * 0.9f)
	{
		CMonsterController* pController = static_cast<CMonsterController*>(m_pController);
		pController->GetAttackMessage();
		
		return BT_SUCCESS;
	}
	else if (m_fTimeSum > m_vecAnimIndexTime[0].second * 0.3f && m_fTimeSum < m_vecAnimIndexTime[0].second * 0.6f)
	{
		CMonsterController* pController = static_cast<CMonsterController*>(m_pController);

		BLACKBOARD& hashBlackBoard = m_pBehaviorTree->GetBlackBoard();
		const auto& target = hashBlackBoard.find(TEXT("Target"));

		const Vec3& vTargetPos = GET_VALUE(CGameObject, target)->GetTransform()->GetPosition();
		const Vec3& vCurPos = m_pGameObject->GetTransform()->GetPosition();

		pController->GetMoveMessage(vTargetPos - vCurPos);
	}

	m_fTimeSum += fTimeDelta;

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

_bool CHellHound_BT_Attack::IsInRange()
{
	BLACKBOARD& hashBlackBoard = m_pBehaviorTree->GetBlackBoard();
	const auto& attackRange = hashBlackBoard.find(TEXT("AttackRange"));
	const auto& target = hashBlackBoard.find(TEXT("Target"));

	const Vec3& vTargetPos = GET_VALUE(CGameObject, target)->GetTransform()->GetPosition();
	const Vec3& vCurPos = m_pGameObject->GetTransform()->GetPosition();
	Vec3 vDist = vTargetPos - vCurPos;

	if (vDist.Length() < *GET_VALUE(_float, attackRange))
		return true;
	else
		return false;
}

_bool CHellHound_BT_Attack::IsZeroHP()
{
	// Temp
	if (dynamic_cast<CHellHound*>(m_pGameObject)->m_IsZeroHP)
		return true;
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