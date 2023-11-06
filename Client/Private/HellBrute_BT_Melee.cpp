#include "stdafx.h"
#include "HellBrute_BT_Melee.h"
#include "GameInstance.h"
#include "GameObject.h"
#include "MonsterController.h"

CHellBrute_BT_Melee::CHellBrute_BT_Melee()
{
}

void CHellBrute_BT_Melee::OnStart()
{
	Super::OnStart(0);
}

CBT_Node::BT_RETURN CHellBrute_BT_Melee::OnUpdate(const _float& fTimeDelta)
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
	}

	m_fTimeSum += fTimeDelta;

	return BT_RUNNING;
}

void CHellBrute_BT_Melee::OnEnd()
{
	Super::OnEnd();
}

void CHellBrute_BT_Melee::ConditionalAbort(const _float& fTimeDelta)
{
}

_bool CHellBrute_BT_Melee::IsZeroHP()
{
	if(static_cast<CMonsterController*>(m_pController)->IsZeroHP())
		return true;

	return false;
}

CHellBrute_BT_Melee* CHellBrute_BT_Melee::Create(CGameObject* pGameObject, CBehaviorTree* pBehaviorTree, const BEHAVEANIMS& tBehaveAnim, CMonoBehaviour* pController)
{
	CHellBrute_BT_Melee* pInstance = new CHellBrute_BT_Melee;

	if (FAILED(pInstance->Initialize(pGameObject, pBehaviorTree, tBehaveAnim, pController)))
	{
		MSG_BOX("Failed to Created : CHellBrute_BT_Melee");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CHellBrute_BT_Melee::Free()
{
	Super::Free();
}