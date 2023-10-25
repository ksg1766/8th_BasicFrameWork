#include "stdafx.h"
#include "HellHound_BT_Chase.h"
#include "GameInstance.h"
#include "GameObject.h"
#include "MonoBehaviour.h"

CHellHound_BT_Chase::CHellHound_BT_Chase()
{
}

void CHellHound_BT_Chase::OnStart()
{
	Super::OnStart();
}

CBT_Node::BT_RETURN CHellHound_BT_Chase::OnUpdate(const Engine::_float& fTimeDelta)
{
	return BT_RETURN();
}

void CHellHound_BT_Chase::OnEnd()
{
	Super::OnEnd();
}

CHellHound_BT_Chase* CHellHound_BT_Chase::Create(CGameObject* pGameObject, CBehaviorTree* pBehaviorTree, const BEHAVEANIMS& tBehaveAnim, CMonoBehaviour* pController)
{
	CHellHound_BT_Chase* pInstance = new CHellHound_BT_Chase;

	if (FAILED(pInstance->Initialize(pGameObject, pBehaviorTree, tBehaveAnim, pController)))
	{
		MSG_BOX("Failed to Created : CP_Strife_State_Dash");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CHellHound_BT_Chase::Free()
{
	Super::Free();
}


