#include "stdafx.h"
#include "Moloch_BT_Dash.h"
#include "GameInstance.h"
#include "Layer.h"
#include "GameObject.h"
#include "BossController.h"

CMoloch_BT_Dash::CMoloch_BT_Dash()
{
}

void CMoloch_BT_Dash::OnStart()
{
	Super::OnStart(0);

	const Vec3& vTargetPos = GetOrAddTarget()->GetTransform()->GetPosition();

	CBossController* pController = static_cast<CBossController*>(m_pController);
	pController->Look(vTargetPos);

	static_cast<CRigidDynamic*>(m_pGameObject->GetRigidBody())->IsKinematic(false);
	pController->GetDashMessage(true);
}

CBT_Node::BT_RETURN CMoloch_BT_Dash::OnUpdate(const _float& fTimeDelta)
{
	if (IsZeroHP())
		return BT_FAIL;

	if (m_fTimeSum > m_vecAnimIndexTime[0].second * 0.9f)
	{
		//CBossController* pController = static_cast<CBossController*>(m_pController);
		//pController->GetAttackMessage(0);

		return BT_SUCCESS;
	}

	m_fTimeSum += fTimeDelta;

	return BT_RUNNING;
}

void CMoloch_BT_Dash::OnEnd()
{
	CBossController* pController = static_cast<CBossController*>(m_pController);
	pController->GetDashMessage(false);

	static_cast<CRigidDynamic*>(m_pGameObject->GetRigidBody())->IsKinematic(true);
	Super::OnEnd();
}

void CMoloch_BT_Dash::ConditionalAbort(const _float& fTimeDelta)
{
}

_bool CMoloch_BT_Dash::IsZeroHP()
{
	if (static_cast<CBossController*>(m_pController)->IsZeroHP())
		return true;

	return false;
}

CGameObject* CMoloch_BT_Dash::GetOrAddTarget()
{
	BLACKBOARD& hashBlackBoard = m_pBehaviorTree->GetBlackBoard();
	const auto& target = hashBlackBoard.find(TEXT("Target"));

	if (target == hashBlackBoard.end())	// 타겟의 키값이 블랙보드에 없다면(이전에 없었으면 데이터도 없어야 함) 키값 추가해줌.
	{
		map<LAYERTAG, class CLayer*>& mapLayers = m_pGameInstance->GetCurrentLevelLayers();
		const map<LAYERTAG, class CLayer*>::iterator& pPlayerLayer = mapLayers.find(LAYERTAG::PLAYER);
		CGameObject* pPlayer = pPlayerLayer->second->GetGameObjects().front();
		tagBlackBoardData<CGameObject*>* pTarget = new tagBlackBoardData<CGameObject*>(pPlayer);

		hashBlackBoard.emplace(TEXT("Target"), pTarget);
		return pPlayer;
	}
	else
	{
		CGameObject* pPlayer = GET_VALUE(CGameObject, target);
		return pPlayer;
	}
}

CMoloch_BT_Dash* CMoloch_BT_Dash::Create(CGameObject* pGameObject, CBehaviorTree* pBehaviorTree, const BEHAVEANIMS& tBehaveAnim, CMonoBehaviour* pController)
{
	CMoloch_BT_Dash* pInstance = new CMoloch_BT_Dash;

	if (FAILED(pInstance->Initialize(pGameObject, pBehaviorTree, tBehaveAnim, pController)))
	{
		MSG_BOX("Failed to Created : CMoloch_BT_Dash");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CMoloch_BT_Dash::Free()
{
	Super::Free();
}