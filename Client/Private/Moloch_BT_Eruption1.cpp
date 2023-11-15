#include "stdafx.h"
#include "Moloch_BT_Eruption1.h"
#include "GameInstance.h"
#include "Layer.h"
#include "GameObject.h"
#include "BossController.h"

CMoloch_BT_Eruption1::CMoloch_BT_Eruption1()
{
}

void CMoloch_BT_Eruption1::OnStart()
{
	Super::OnStart(0);
}

CBT_Node::BT_RETURN CMoloch_BT_Eruption1::OnUpdate(const _float& fTimeDelta)
{
	if (IsZeroHP())
		return BT_FAIL;

	if (m_fTimeSum > m_vecAnimIndexTime[0].second * 0.9f)
	{
		return BT_SUCCESS;
	}

	m_fTimeSum += fTimeDelta;

	return BT_RUNNING;
}

void CMoloch_BT_Eruption1::OnEnd()
{
	Super::OnEnd();
}

void CMoloch_BT_Eruption1::ConditionalAbort(const _float& fTimeDelta)
{
}

_bool CMoloch_BT_Eruption1::IsZeroHP()
{
	if (static_cast<CBossController*>(m_pController)->IsZeroHP())
		return true;

	return false;
}

CMoloch_BT_Eruption1* CMoloch_BT_Eruption1::Create(CGameObject* pGameObject, CBehaviorTree* pBehaviorTree, const BEHAVEANIMS& tBehaveAnim, CMonoBehaviour* pController)
{
	CMoloch_BT_Eruption1* pInstance = new CMoloch_BT_Eruption1;

	if (FAILED(pInstance->Initialize(pGameObject, pBehaviorTree, tBehaveAnim, pController)))
	{
		MSG_BOX("Failed to Created : CMoloch_BT_Eruption1");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CMoloch_BT_Eruption1::Free()
{
	Super::Free();
}