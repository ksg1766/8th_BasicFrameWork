#include "stdafx.h"
#include "HellHound_BT_Hit.h"
#include "GameInstance.h"
#include "GameObject.h"
#include "MonoBehaviour.h"

CHellHound_BT_Hit::CHellHound_BT_Hit()
{
}

void CHellHound_BT_Hit::OnStart()
{
	Super::OnStart();
}

CBT_Node::BT_RETURN CHellHound_BT_Hit::OnUpdate(const _float& fTimeDelta)
{
	return BT_RETURN();
}

void CHellHound_BT_Hit::OnEnd()
{
	Super::OnEnd();
}

CHellHound_BT_Hit* CHellHound_BT_Hit::Create(CGameObject* pGameObject, CBehaviorTree* pBehaviorTree, const BEHAVEANIMS& tBehaveAnim, CMonoBehaviour* pController)
{
	CHellHound_BT_Hit* pInstance = new CHellHound_BT_Hit;

	if (FAILED(pInstance->Initialize(pGameObject, pBehaviorTree, tBehaveAnim, pController)))
	{
		MSG_BOX("Failed to Created : CHellHound_BT_Hit");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CHellHound_BT_Hit::Free()
{
	Super::Free();
}