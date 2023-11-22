#include "stdafx.h"
#include "Dagon_BT_Tentacle.h"
#include "GameInstance.h"
#include "GameObject.h"
#include "BossController.h"

CDagon_BT_Tentacle::CDagon_BT_Tentacle()
{
}

void CDagon_BT_Tentacle::OnStart()
{
	Super::OnStart(0);
}

CBT_Node::BT_RETURN CDagon_BT_Tentacle::OnUpdate(const _float& fTimeDelta)
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

void CDagon_BT_Tentacle::OnEnd()
{
	Super::OnEnd();
}

void CDagon_BT_Tentacle::ConditionalAbort(const _float& fTimeDelta)
{
}

_bool CDagon_BT_Tentacle::IsZeroHP()
{
	if (static_cast<CBossController*>(m_pController)->IsZeroHP())
		return true;

	return false;
}

CDagon_BT_Tentacle* CDagon_BT_Tentacle::Create(CGameObject* pGameObject, CBehaviorTree* pBehaviorTree, const BEHAVEANIMS& tBehaveAnim, CMonoBehaviour* pController)
{
	CDagon_BT_Tentacle* pInstance = new CDagon_BT_Tentacle;

	if (FAILED(pInstance->Initialize(pGameObject, pBehaviorTree, tBehaveAnim, pController)))
	{
		MSG_BOX("Failed to Created : CDagon_BT_Tentacle");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CDagon_BT_Tentacle::Free()
{
	Super::Free();
}