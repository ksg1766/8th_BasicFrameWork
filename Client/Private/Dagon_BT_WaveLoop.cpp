#include "stdafx.h"
#include "Dagon_BT_WaveLoop.h"
#include "GameInstance.h"
#include "GameObject.h"
#include "BossController.h"

CDagon_BT_WaveLoop::CDagon_BT_WaveLoop()
{
}

void CDagon_BT_WaveLoop::OnStart()
{
	Super::OnStart(0);
	HitOrMiss();
}

CBT_Node::BT_RETURN CDagon_BT_WaveLoop::OnUpdate(const _float& fTimeDelta)
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

void CDagon_BT_WaveLoop::OnEnd()
{
	Super::OnEnd();
}

void CDagon_BT_WaveLoop::ConditionalAbort(const _float& fTimeDelta)
{
}

_bool CDagon_BT_WaveLoop::IsZeroHP()
{
	if (static_cast<CBossController*>(m_pController)->IsZeroHP())
		return true;

	return false;
}

void CDagon_BT_WaveLoop::HitOrMiss()
{
	random_device		RandomDevice;

	mt19937_64							RandomNumber(RandomDevice());
	uniform_int_distribution<_int>		iRandom(0, 20);

	if (3 >= iRandom(RandomNumber))
		m_bHitOrMiss = true;
	else
		m_bHitOrMiss = false;
}

CDagon_BT_WaveLoop* CDagon_BT_WaveLoop::Create(CGameObject* pGameObject, CBehaviorTree* pBehaviorTree, const BEHAVEANIMS& tBehaveAnim, CMonoBehaviour* pController)
{
	CDagon_BT_WaveLoop* pInstance = new CDagon_BT_WaveLoop;

	if (FAILED(pInstance->Initialize(pGameObject, pBehaviorTree, tBehaveAnim, pController)))
	{
		MSG_BOX("Failed to Created : CDagon_BT_WaveLoop");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CDagon_BT_WaveLoop::Free()
{
	Super::Free();
}