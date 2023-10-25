#include "stdafx.h"
#include "HellHound_BT_StepBack.h"
#include "GameInstance.h"
#include "GameObject.h"
#include "MonoBehaviour.h"

CHellHound_BT_StepBack::CHellHound_BT_StepBack()
{
}

void CHellHound_BT_StepBack::OnStart()
{
	Super::OnStart();
}

CBT_Node::BT_RETURN CHellHound_BT_StepBack::OnUpdate(const _float& fTimeDelta)
{
	return BT_RETURN();
}

void CHellHound_BT_StepBack::OnEnd()
{
	Super::OnEnd();
}

CHellHound_BT_StepBack* CHellHound_BT_StepBack::Create(CGameObject* pGameObject, CBehaviorTree* pBehaviorTree, const BEHAVEANIMS& tBehaveAnim, CMonoBehaviour* pController)
{
	CHellHound_BT_StepBack* pInstance = new CHellHound_BT_StepBack;

	if (FAILED(pInstance->Initialize(pGameObject, pBehaviorTree, tBehaveAnim, pController)))
	{
		MSG_BOX("Failed to Created : CHellHound_BT_StepBack");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CHellHound_BT_StepBack::Free()
{
	Super::Free();
}