#include "stdafx.h"
#include "HellHound_BT_Dead.h"
#include "GameInstance.h"
#include "GameObject.h"
#include "MonoBehaviour.h"
#include "DissolveManager.h"
//Temp
#include "HellHound.h"

CHellHound_BT_Dead::CHellHound_BT_Dead()
{
}

void CHellHound_BT_Dead::OnStart()
{
	Super::OnStart(0);

	static_cast<CRigidDynamic*>(m_pGameObject->GetRigidBody())->IsKinematic(false);
}

CBT_Node::BT_RETURN CHellHound_BT_Dead::OnUpdate(const _float& fTimeDelta)
{
	ConditionalAbort(fTimeDelta);

	if (!m_bDissolveFlag && m_fTimeSum > m_vecAnimIndexTime[0].second * 0.8f)
	{
		m_pModel->PauseAnimation(true);
		CDissolveManager::GetInstance()->AddDissolve(m_pGameObject);
		m_bDissolveFlag = true;
		//return BT_RUNNING;
	}

	m_fTimeSum += fTimeDelta;

	return BT_RUNNING;
	//return BT_FAIL;
}

void CHellHound_BT_Dead::OnEnd()
{
	Super::OnEnd();
}

void CHellHound_BT_Dead::ConditionalAbort(const _float& fTimeDelta)
{
}

CHellHound_BT_Dead* CHellHound_BT_Dead::Create(CGameObject* pGameObject, CBehaviorTree* pBehaviorTree, const BEHAVEANIMS& tBehaveAnim, CMonoBehaviour* pController)
{
	CHellHound_BT_Dead* pInstance = new CHellHound_BT_Dead;

	if (FAILED(pInstance->Initialize(pGameObject, pBehaviorTree, tBehaveAnim, pController)))
	{
		MSG_BOX("Failed to Created : CHellHound_BT_Dead");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CHellHound_BT_Dead::Free()
{
	Super::Free();
}