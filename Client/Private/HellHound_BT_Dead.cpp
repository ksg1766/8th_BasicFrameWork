#include "stdafx.h"
#include "HellHound_BT_Dead.h"
#include "GameInstance.h"
#include "GameObject.h"
#include "MonoBehaviour.h"

CHellHound_BT_Dead::CHellHound_BT_Dead()
{
}

void CHellHound_BT_Dead::OnStart()
{
	Super::OnStart(0);
}

CBT_Node::BT_RETURN CHellHound_BT_Dead::OnUpdate(const _float& fTimeDelta)
{
	return BT_RETURN();
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