#include "stdafx.h"
#include "HellHound_BT_Attack.h"
#include "GameInstance.h"
#include "GameObject.h"
#include "MonoBehaviour.h"

CHellHound_BT_Attack::CHellHound_BT_Attack()
{
}

void CHellHound_BT_Attack::OnStart()
{
	Super::OnStart();
}

CBT_Node::BT_RETURN CHellHound_BT_Attack::OnUpdate(const _float& fTimeDelta)
{
	return BT_RETURN();
}

void CHellHound_BT_Attack::OnEnd()
{
	Super::OnEnd();
}

CHellHound_BT_Attack* CHellHound_BT_Attack::Create(CGameObject* pGameObject, CBehaviorTree* pBehaviorTree, const BEHAVEANIMS& tBehaveAnim, CMonoBehaviour* pController)
{
	CHellHound_BT_Attack* pInstance = new CHellHound_BT_Attack;

	if (FAILED(pInstance->Initialize(pGameObject, pBehaviorTree, tBehaveAnim, pController)))
	{
		MSG_BOX("Failed to Created : CHellHound_BT_Attack");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CHellHound_BT_Attack::Free()
{
	Super::Free();
}