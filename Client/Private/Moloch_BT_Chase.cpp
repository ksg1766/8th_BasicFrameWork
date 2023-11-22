#include "stdafx.h"
#include "Moloch_BT_Chase.h"
#include "GameInstance.h"
#include "Layer.h"
#include "GameObject.h"
#include "BossController.h"

CMoloch_BT_Chase::CMoloch_BT_Chase()
{
}

void CMoloch_BT_Chase::OnStart()
{
	Super::OnStart(0);
}

CBT_Node::BT_RETURN CMoloch_BT_Chase::OnUpdate(const _float& fTimeDelta)
{
	ConditionalAbort(fTimeDelta);

	if (IsInRange())
		return BT_SUCCESS;

	BLACKBOARD& hashBlackBoard = m_pBehaviorTree->GetBlackBoard();
	const auto& target = hashBlackBoard.find(TEXT("Target"));

	const Vec3& vTargetPos = GET_VALUE(CGameObject, target)->GetTransform()->GetPosition();
	const Vec3& vCurrentPos = m_pGameObject->GetTransform()->GetPosition();
	
	Vec3 vChaseDir = vTargetPos - vCurrentPos;
	vChaseDir.Normalize();

	CBossController* pController = static_cast<CBossController*>(m_pController);
	////pController->GetMoveMessage(vChaseDir);

	return BT_RUNNING;
}

void CMoloch_BT_Chase::OnEnd()
{
	Super::OnEnd();
}

void CMoloch_BT_Chase::ConditionalAbort(const _float& fTimeDelta)
{
}

_bool CMoloch_BT_Chase::IsInRange()
{
	BLACKBOARD& hashBlackBoard = m_pBehaviorTree->GetBlackBoard();
	const auto& attackRange = hashBlackBoard.find(TEXT("AttackRange"));
	const auto& target = hashBlackBoard.find(TEXT("Target"));

	if ((GET_VALUE(CGameObject, target)->GetTransform()->GetPosition() - m_pGameObject->GetTransform()->GetPosition()).Length() < *GET_VALUE(_float, attackRange))
		return true;
	else
		return false;
}

CMoloch_BT_Chase* CMoloch_BT_Chase::Create(CGameObject* pGameObject, CBehaviorTree* pBehaviorTree, const BEHAVEANIMS& tBehaveAnim, CMonoBehaviour* pController)
{
	CMoloch_BT_Chase* pInstance = new CMoloch_BT_Chase;

	if (FAILED(pInstance->Initialize(pGameObject, pBehaviorTree, tBehaveAnim, pController)))
	{
		MSG_BOX("Failed to Created : CP_Strife_State_Dash");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CMoloch_BT_Chase::Free()
{
	Super::Free();
}


