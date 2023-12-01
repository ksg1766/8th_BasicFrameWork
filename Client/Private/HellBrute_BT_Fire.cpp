#include "stdafx.h"
#include "HellBrute_BT_Fire.h"
#include "GameInstance.h"
#include "Layer.h"
#include "GameObject.h"
#include "MonsterController.h"

#include "Strife_Ammo_Default.h"

CHellBrute_BT_Fire::CHellBrute_BT_Fire()
{
}

void CHellBrute_BT_Fire::OnStart()
{
	Super::OnStart(0);
}

CBT_Node::BT_RETURN CHellBrute_BT_Fire::OnUpdate(const _float& fTimeDelta)
{
	ConditionalAbort(fTimeDelta);
	if (IsZeroHP())
		return BT_FAIL;

	if (IsInRange())
		return BT_FAIL;

	BLACKBOARD& hashBlackBoard = m_pBehaviorTree->GetBlackBoard();
	const auto& target = hashBlackBoard.find(TEXT("Target"));

	const Vec3& vTargetPos = GET_VALUE(CGameObject, target)->GetTransform()->GetPosition();

	CMonsterController* pController = static_cast<CMonsterController*>(m_pController);
	pController->Look(vTargetPos);
	//pController->GetAttackMessage();

	return BT_RUNNING;
}

void CHellBrute_BT_Fire::OnEnd()
{
	Super::OnEnd();
}

void CHellBrute_BT_Fire::ConditionalAbort(const _float& fTimeDelta)
{
}

_bool CHellBrute_BT_Fire::IsInRange()
{
	BLACKBOARD& hashBlackBoard = m_pBehaviorTree->GetBlackBoard();
	const auto& meleeRange = hashBlackBoard.find(TEXT("MeleeRange"));
	const auto& target = hashBlackBoard.find(TEXT("Target"));

	const Vec3& vTargetPos = GET_VALUE(CGameObject, target)->GetTransform()->GetPosition();
	const Vec3& vCurPos = m_pGameObject->GetTransform()->GetPosition();
	Vec3 vDist = vTargetPos - vCurPos;

	if (vDist.Length() < *GET_VALUE(_float, meleeRange))
		return true;
	else
		return false;
}

_bool CHellBrute_BT_Fire::IsInSight()
{
	map<LAYERTAG, class CLayer*>& mapLayers = m_pGameInstance->GetCurrentLevelLayers();
	const map<LAYERTAG, class CLayer*>::iterator& pPlayerLayer = mapLayers.find(LAYERTAG::PLAYER);

	BLACKBOARD& hashBlackBoard = m_pBehaviorTree->GetBlackBoard();
	const auto& tSight = hashBlackBoard.find(TEXT("Sight"));
	const auto& tTarget = hashBlackBoard.find(TEXT("Target"));

	CGameObject* pPlayer = pPlayerLayer->second->GetGameObjects().front();
	if ((pPlayer->GetTransform()->GetPosition() - m_pGameObject->GetTransform()->GetPosition()).Length() < *GET_VALUE(_float, tSight))	// 시야에 있다면
	{
		if (tTarget == hashBlackBoard.end())	// 타겟의 키값이 블랙보드에 없다면(이전에 없었으면 데이터도 없어야 함) 키값 추가해줌.
		{
			tagBlackBoardData<CGameObject*>* pTarget = new tagBlackBoardData<CGameObject*>(pPlayer);

			hashBlackBoard.emplace(TEXT("Target"), pTarget);
		}

		return true;
	}
	else // 시야에 없다면
	{
		BLACKBOARD& hashBlackBoard = m_pBehaviorTree->GetBlackBoard();

		if (tTarget != hashBlackBoard.end())	// 근데 키값이 있다면 제거
		{
			hashBlackBoard.erase(tTarget);
		}

		return false;
	}
}

_bool CHellBrute_BT_Fire::IsZeroHP()
{
	if(static_cast<CMonsterController*>(m_pController)->IsZeroHP())
		return true;

	return false;
}

void CHellBrute_BT_Fire::Fire()
{
	//CGameObject* pAmmo = nullptr;
	//CStrife_Ammo::AMMOPROPS tProps;

	//Vec3 vFireOffset;
	//Quaternion qRot = m_pTransform->GetRotationQuaternion();

	//tProps = { eAmmoType, 7, 0, /*10*/50, 70.f * m_pTransform->GetForward(), false, 5.f };
	//pAmmo = m_pGameInstance->CreateObject(L"Prototype_GameObject_Strife_Ammo_Default", LAYERTAG::IGNORECOLLISION, &tProps);
	//vFireOffset = m_pTransform->GetPosition() + 2.2f * m_pTransform->GetForward() - (m_bFireLR * 0.35f - 0.175f) * m_pTransform->GetRight() + 1.7f * m_pTransform->GetUp();
	//m_bFireLR = !m_bFireLR;

	//pAmmo->GetTransform()->Rotate(qRot);
	//pAmmo->GetTransform()->SetPosition(vFireOffset);
}

CHellBrute_BT_Fire* CHellBrute_BT_Fire::Create(CGameObject* pGameObject, CBehaviorTree* pBehaviorTree, const BEHAVEANIMS& tBehaveAnim, CMonoBehaviour* pController)
{
	CHellBrute_BT_Fire* pInstance = new CHellBrute_BT_Fire;

	if (FAILED(pInstance->Initialize(pGameObject, pBehaviorTree, tBehaveAnim, pController)))
	{
		MSG_BOX("Failed to Created : CHellBrute_BT_Fire");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CHellBrute_BT_Fire::Free()
{
	Super::Free();
}