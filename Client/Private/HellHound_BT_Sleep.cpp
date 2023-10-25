#include "stdafx.h"
#include "HellHound_BT_Sleep.h"
#include "GameInstance.h"
#include "GameObject.h"
#include "MonoBehaviour.h"

CHellHound_BT_Sleep::CHellHound_BT_Sleep()
{
}

CHellHound_BT_Sleep::CHellHound_BT_Sleep(const CHellHound_BT_Sleep& rhs)
	:Super(rhs)
{
}

void CHellHound_BT_Sleep::OnStart()
{
	Super::OnStart();
}

CBT_Node::BT_RETURN CHellHound_BT_Sleep::OnUpdate(const _float& fTimeDelta)
{
	return BT_RETURN();
}

void CHellHound_BT_Sleep::OnEnd()
{
	Super::OnEnd();
}

CHellHound_BT_Sleep* CHellHound_BT_Sleep::Create(CGameObject* pGameObject, CBehaviorTree* pBehaviorTree, const BEHAVEANIMS& tBehaveAnim, CMonoBehaviour* pController)
{
	CHellHound_BT_Sleep* pInstance = new CHellHound_BT_Sleep;

	if (FAILED(pInstance->Initialize(pGameObject, pBehaviorTree, tBehaveAnim, pController)))
	{
		MSG_BOX("Failed to Created : CHellHound_BT_Sleep");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CHellHound_BT_Sleep::Free()
{
	Super::Free();
}