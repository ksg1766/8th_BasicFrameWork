#include "stdafx.h"
#include "Dagon_BT_Lightning.h"
#include "GameInstance.h"
#include "GameObject.h"
#include "BossController.h"

CDagon_BT_Lightning::CDagon_BT_Lightning()
{
}

void CDagon_BT_Lightning::OnStart()
{
	Super::OnStart(0);
	HitOrMiss();
}

CBT_Node::BT_RETURN CDagon_BT_Lightning::OnUpdate(const _float& fTimeDelta)
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

void CDagon_BT_Lightning::OnEnd()
{
	Super::OnEnd();
}

void CDagon_BT_Lightning::ConditionalAbort(const _float& fTimeDelta)
{
}

_bool CDagon_BT_Lightning::IsZeroHP()
{
	if (static_cast<CBossController*>(m_pController)->IsZeroHP())
		return true;

	return false;
}

void CDagon_BT_Lightning::HitOrMiss()
{
	random_device		RandomDevice;

	mt19937_64							RandomNumber(RandomDevice());
	uniform_int_distribution<_int>		iRandom(0, 20);

	if (4 >= iRandom(RandomNumber))
		m_bHitOrMiss = true;
	else
		m_bHitOrMiss = false;
}

CDagon_BT_Lightning* CDagon_BT_Lightning::Create(CGameObject* pGameObject, CBehaviorTree* pBehaviorTree, const BEHAVEANIMS& tBehaveAnim, CMonoBehaviour* pController)
{
	CDagon_BT_Lightning* pInstance = new CDagon_BT_Lightning;

	if (FAILED(pInstance->Initialize(pGameObject, pBehaviorTree, tBehaveAnim, pController)))
	{
		MSG_BOX("Failed to Created : CDagon_BT_Lightning");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CDagon_BT_Lightning::Free()
{
	Super::Free();
}