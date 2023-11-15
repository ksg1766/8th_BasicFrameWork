#include "stdafx.h"
#include "Moloch_BT_TremorPulse.h"
#include "GameInstance.h"
#include "Layer.h"
#include "GameObject.h"
#include "BossController.h"

CMoloch_BT_TremorPulse::CMoloch_BT_TremorPulse()
{
}

void CMoloch_BT_TremorPulse::OnStart()
{
	Super::OnStart(0);
}

CBT_Node::BT_RETURN CMoloch_BT_TremorPulse::OnUpdate(const _float& fTimeDelta)
{
	if (m_fTimeSum > m_vecAnimIndexTime[0].second * 0.9f)
	{
		return BT_SUCCESS;
	}

	m_fTimeSum += fTimeDelta;

	return BT_RUNNING;
}

void CMoloch_BT_TremorPulse::OnEnd()
{
	Super::OnEnd();
}

void CMoloch_BT_TremorPulse::ConditionalAbort(const _float& fTimeDelta)
{
}

CMoloch_BT_TremorPulse* CMoloch_BT_TremorPulse::Create(CGameObject* pGameObject, CBehaviorTree* pBehaviorTree, const BEHAVEANIMS& tBehaveAnim, CMonoBehaviour* pController)
{
	CMoloch_BT_TremorPulse* pInstance = new CMoloch_BT_TremorPulse;

	if (FAILED(pInstance->Initialize(pGameObject, pBehaviorTree, tBehaveAnim, pController)))
	{
		MSG_BOX("Failed to Created : CMoloch_BT_TremorPulse");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CMoloch_BT_TremorPulse::Free()
{
	Super::Free();
}