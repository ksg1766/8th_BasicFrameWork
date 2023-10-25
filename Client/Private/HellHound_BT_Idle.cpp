#include "stdafx.h"
#include "HellHound_BT_Idle.h"
#include "GameInstance.h"
#include "GameObject.h"
#include "MonoBehaviour.h"

CHellHound_BT_Idle::CHellHound_BT_Idle()
{
}

void CHellHound_BT_Idle::OnStart()
{
	Super::OnStart();
}

CBT_Node::BT_RETURN CHellHound_BT_Idle::OnUpdate(const Engine::_float& fTimeDelta)
{
	return BT_RETURN();
}

void CHellHound_BT_Idle::OnEnd()
{
	Super::OnEnd();
}

CHellHound_BT_Idle* CHellHound_BT_Idle::Create(CGameObject* pGameObject, CBehaviorTree* pBehaviorTree, const BEHAVEANIMS& tBehaveAnim, CMonoBehaviour* pController)
{
	CHellHound_BT_Idle* pInstance = new CHellHound_BT_Idle;

	if (FAILED(pInstance->Initialize(pGameObject, pBehaviorTree, tBehaveAnim, pController)))
	{
		MSG_BOX("Failed to Created : CHellHound_BT_Idle");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CHellHound_BT_Idle::Free()
{
	Super::Free();
}


