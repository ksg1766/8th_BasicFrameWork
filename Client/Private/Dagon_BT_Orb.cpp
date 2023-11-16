#include "stdafx.h"
#include "Dagon_BT_Orb.h"
#include "GameInstance.h"
#include "GameObject.h"
#include "BossController.h"

CDagon_BT_Orb::CDagon_BT_Orb()
{
}

void CDagon_BT_Orb::OnStart()
{
	Super::OnStart(0);
	HitOrMiss();
}

CBT_Node::BT_RETURN CDagon_BT_Orb::OnUpdate(const _float& fTimeDelta)
{
	if (IsZeroHP())
		return BT_FAIL;

	if (!m_bHitOrMiss)
		return BT_FAIL;

	if (m_fTimeSum > m_vecAnimIndexTime[0].second * 0.9f)
	{
		return BT_SUCCESS;
	}

	m_fTimeSum += fTimeDelta;

	return BT_RUNNING;
}

void CDagon_BT_Orb::OnEnd()
{
	Super::OnEnd();
}

void CDagon_BT_Orb::ConditionalAbort(const _float& fTimeDelta)
{
}

_bool CDagon_BT_Orb::IsZeroHP()
{
	if (static_cast<CBossController*>(m_pController)->IsZeroHP())
		return true;

	return false;
}

void CDagon_BT_Orb::HitOrMiss()
{
	random_device		RandomDevice;

	mt19937_64							RandomNumber(RandomDevice());
	uniform_int_distribution<_int>		iRandom(0, 20);

	if (4 >= iRandom(RandomNumber))
		m_bHitOrMiss = true;
	else
		m_bHitOrMiss = false;
}

CDagon_BT_Orb* CDagon_BT_Orb::Create(CGameObject* pGameObject, CBehaviorTree* pBehaviorTree, const BEHAVEANIMS& tBehaveAnim, CMonoBehaviour* pController)
{
	CDagon_BT_Orb* pInstance = new CDagon_BT_Orb;

	if (FAILED(pInstance->Initialize(pGameObject, pBehaviorTree, tBehaveAnim, pController)))
	{
		MSG_BOX("Failed to Created : CDagon_BT_Orb");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CDagon_BT_Orb::Free()
{
	Super::Free();
}