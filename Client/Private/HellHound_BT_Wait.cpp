#include "stdafx.h"
#include "HellHound_BT_Wait.h"
#include "GameInstance.h"
#include "GameObject.h"
#include "MonoBehaviour.h"

CHellHound_BT_Wait::CHellHound_BT_Wait()
{
}

void CHellHound_BT_Wait::OnStart()
{
	Super::OnStart();
}

CBT_Node::BT_RETURN CHellHound_BT_Wait::OnUpdate(const _float& fTimeDelta)
{
	return BT_RETURN();
}

void CHellHound_BT_Wait::OnEnd()
{
	Super::OnEnd();
}

CHellHound_BT_Wait* CHellHound_BT_Wait::Create(CGameObject* pGameObject, CBehaviorTree* pBehaviorTree, const BEHAVEANIMS& tBehaveAnim, CMonoBehaviour* pController)
{
	CHellHound_BT_Wait* pInstance = new CHellHound_BT_Wait;

	if (FAILED(pInstance->Initialize(pGameObject, pBehaviorTree, tBehaveAnim, pController)))
	{
		MSG_BOX("Failed to Created : CHellHound_BT_Wait");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CHellHound_BT_Wait::Free()
{
	Super::Free();
}