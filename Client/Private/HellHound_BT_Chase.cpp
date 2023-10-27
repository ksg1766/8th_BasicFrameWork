#include "stdafx.h"
#include "HellHound_BT_Chase.h"
#include "GameInstance.h"
#include "GameObject.h"
#include "MonsterController.h"

CHellHound_BT_Chase::CHellHound_BT_Chase()
{
}

void CHellHound_BT_Chase::OnStart()
{
	Super::OnStart(0);
}

CBT_Node::BT_RETURN CHellHound_BT_Chase::OnUpdate(const Engine::_float& fTimeDelta)
{
	ConditionalAbort(fTimeDelta);
	if (IsInRange())
		return BT_SUCCESS;

	BLACKBOARD& hashBlackBoard = m_pBehaviorTree->GetBlackBoard();
	const auto& attackRange = hashBlackBoard.find(TEXT("AttackRange"));
	const auto& target = hashBlackBoard.find(TEXT("Target"));

	const Vec3& vTargetPos = GET_VALUE(CGameObject, target)->GetTransform()->GetPosition();
	const Vec3& vCurrentPos = m_pGameObject->GetTransform()->GetPosition();
	
	Vec3 vChaseDir = vTargetPos - vCurrentPos;
	vChaseDir.Normalize();

	CMonsterController* pController = static_cast<CMonsterController*>(m_pController);
	pController->GetChaseMessage();
	pController->GetMoveMessage(vChaseDir);

	return BT_RUNNING;
}

void CHellHound_BT_Chase::OnEnd()
{
	Super::OnEnd();
}

void CHellHound_BT_Chase::ConditionalAbort(const _float& fTimeDelta)
{
}

_bool CHellHound_BT_Chase::IsInRange()
{
	BLACKBOARD& hashBlackBoard = m_pBehaviorTree->GetBlackBoard();
	const auto& attackRange = hashBlackBoard.find(TEXT("AttackRange"));
	const auto& target = hashBlackBoard.find(TEXT("Target"));

	if ((GET_VALUE(CGameObject, target)->GetTransform()->GetPosition() - m_pGameObject->GetTransform()->GetPosition()).Length() < *GET_VALUE(_float, attackRange))
		return true;
	else
		return false;
}

CHellHound_BT_Chase* CHellHound_BT_Chase::Create(CGameObject* pGameObject, CBehaviorTree* pBehaviorTree, const BEHAVEANIMS& tBehaveAnim, CMonoBehaviour* pController)
{
	CHellHound_BT_Chase* pInstance = new CHellHound_BT_Chase;

	if (FAILED(pInstance->Initialize(pGameObject, pBehaviorTree, tBehaveAnim, pController)))
	{
		MSG_BOX("Failed to Created : CP_Strife_State_Dash");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CHellHound_BT_Chase::Free()
{
	Super::Free();
}


