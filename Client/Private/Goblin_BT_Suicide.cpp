#include "stdafx.h"
#include "Goblin_BT_Suicide.h"
#include "GameInstance.h"
#include "GameObject.h"
#include "MonsterController.h"

CGoblin_BT_Suicide::CGoblin_BT_Suicide()
{
}

void CGoblin_BT_Suicide::OnStart()
{
	Super::OnStart(0);
}

CBT_Node::BT_RETURN CGoblin_BT_Suicide::OnUpdate(const _float& fTimeDelta)
{
	ConditionalAbort(fTimeDelta);
	if (IsZeroHP())
		return BT_FAIL;

	if (m_fTimeSum > m_vecAnimIndexTime[0].second * 0.9f)
	{
		CMonsterController* pController = static_cast<CMonsterController*>(m_pController);
		pController->GetAttackMessage();
		
		m_pGameInstance->DeleteObject(m_pGameObject);

		m_pGameInstance->CreateObject(TEXT("Prototype_GameObject_Bubble"), LAYERTAG::IGNORECOLLISION)
			->GetTransform()->Translate(m_pGameObject->GetTransform()->GetPosition());
		m_pGameInstance->CreateObject(TEXT("Prototype_GameObject_Wave_Ring"), LAYERTAG::IGNORECOLLISION)
			->GetTransform()->Translate(m_pGameObject->GetTransform()->GetPosition());

		return BT_SUCCESS;
	}

	m_fTimeSum += fTimeDelta;

	return BT_RUNNING;
}

void CGoblin_BT_Suicide::OnEnd()
{
	Super::OnEnd();
}

void CGoblin_BT_Suicide::ConditionalAbort(const _float& fTimeDelta)
{
}

_bool CGoblin_BT_Suicide::IsZeroHP()
{
	if(static_cast<CMonsterController*>(m_pController)->IsZeroHP())
		return true;

	return false;
}

CGoblin_BT_Suicide* CGoblin_BT_Suicide::Create(CGameObject* pGameObject, CBehaviorTree* pBehaviorTree, const BEHAVEANIMS& tBehaveAnim, CMonoBehaviour* pController)
{
	CGoblin_BT_Suicide* pInstance = new CGoblin_BT_Suicide;

	if (FAILED(pInstance->Initialize(pGameObject, pBehaviorTree, tBehaveAnim, pController)))
	{
		MSG_BOX("Failed to Created : CGoblin_BT_Suicide");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CGoblin_BT_Suicide::Free()
{
	Super::Free();
}