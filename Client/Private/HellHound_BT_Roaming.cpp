#include "stdafx.h"
#include "HellHound_BT_Roaming.h"
#include "GameInstance.h"
#include "GameObject.h"
#include "MonsterController.h"

CHellHound_BT_Roaming::CHellHound_BT_Roaming()
{
}

void CHellHound_BT_Roaming::OnStart()
{
	Super::OnStart(0);
	SetRoamingPoint();
}

CBT_Node::BT_RETURN CHellHound_BT_Roaming::OnUpdate(const _float& fTimeDelta)
{
	BLACKBOARD& hashBlackBoard = m_pBehaviorTree->GetBlackBoard();
	const auto& tRoamingPoint = hashBlackBoard.find(TEXT("RoamingPoint"));

	const Vec3& vRoamingPoint = *GET_VALUE(Vec3, tRoamingPoint);
	const Vec3& vCurrentPos = m_pGameObject->GetTransform()->GetPosition();

	Vec3 vRoamingDir = vRoamingPoint - vCurrentPos;
	vRoamingDir.Normalize();

	if ((vCurrentPos - vRoamingPoint).Length() < 1.f)
		return BT_SUCCESS;

	CMonsterController* pController = static_cast<CMonsterController*>(m_pController);
	pController->GetMoveMessage(vRoamingDir);

	return BT_RUNNING;
}

void CHellHound_BT_Roaming::OnEnd()
{
	Super::OnEnd();

	BLACKBOARD& hashBlackBoard = m_pBehaviorTree->GetBlackBoard();
	const auto& tRoamingPoint = hashBlackBoard.find(TEXT("RoamingPoint"));

	hashBlackBoard.erase(tRoamingPoint);
}

void CHellHound_BT_Roaming::ConditionalAbort(const _float& fTimeDelta)
{
}

void CHellHound_BT_Roaming::SetRoamingPoint()
{
	BLACKBOARD& hashBlackBoard = m_pBehaviorTree->GetBlackBoard();
	const auto& tRoamingPoint = hashBlackBoard.find(TEXT("RoamingPoint"));

	if (tRoamingPoint == hashBlackBoard.end())
	{
		Vec3 vRandPos(rand() % 3, 0.f, (rand() * 7) % 3);

		CMonsterController* pController = static_cast<CMonsterController*>(m_pController);
		/*while (!pController->Walkable(vRandPos))
		{
			vRandPos.x = (_float)(((_int)vRandPos.x * (_int)pow(-1, rand())) % 3);
			vRandPos.z = (_float)(((_int)vRandPos.z * (_int)pow(-1, rand())) % 3);
		}*/ // 뭐지... 일단 주석

		tagBlackBoardData<Vec3>* tStepBackCool = new tagBlackBoardData<Vec3>(Vec3(m_pGameObject->GetTransform()->GetPosition()) + vRandPos);
		hashBlackBoard.emplace(TEXT("RoamingPoint"), tStepBackCool);
	}
	else
	{
		__debugbreak();
	}
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