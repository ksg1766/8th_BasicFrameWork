#include "stdafx.h"
#include "HellHound_BT_Roaming.h"
#include "GameInstance.h"
#include "GameObject.h"
#include "MonoBehaviour.h"

CHellHound_BT_Roaming::CHellHound_BT_Roaming()
{
}

void CHellHound_BT_Roaming::OnStart()
{
	Super::OnStart();
}

CBT_Node::BT_RETURN CHellHound_BT_Roaming::OnUpdate(const _float& fTimeDelta)
{
	return BT_RETURN();
}

void CHellHound_BT_Roaming::OnEnd()
{
	Super::OnEnd();
}

CHellHound_BT_Roaming* CHellHound_BT_Roaming::Create(CGameObject* pGameObject, CBehaviorTree* pBehaviorTree, const BEHAVEANIMS& tBehaveAnim, CMonoBehaviour* pController)
{
	CHellHound_BT_Roaming* pInstance = new CHellHound_BT_Roaming;

	if (FAILED(pInstance->Initialize(pGameObject, pBehaviorTree, tBehaveAnim, pController)))
	{
		MSG_BOX("Failed to Created : CHellHound_BT_Roaming");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CHellHound_BT_Roaming::Free()
{
	Super::Free();
}