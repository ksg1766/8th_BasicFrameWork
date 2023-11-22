#include "stdafx.h"
#include "Moloch_BT_FullDash2.h"
#include "GameInstance.h"
#include "GameObject.h"
#include "BossController.h"
#include "Moloch_MotionTrail.h"

CMoloch_BT_FullDash2::CMoloch_BT_FullDash2()
{
}

void CMoloch_BT_FullDash2::OnStart()
{
	Super::OnStart(0);

	m_vTargetPos = GetTarget()->GetTransform()->GetPosition();

	CBossController* pController = static_cast<CBossController*>(m_pController);
	pController->Look(m_vTargetPos);
}

CBT_Node::BT_RETURN CMoloch_BT_FullDash2::OnUpdate(const _float& fTimeDelta)
{
	if (IsZeroHP())
		return BT_FAIL;

	if (m_fTimeSum > m_vecAnimIndexTime[0].second * 0.9f)
	{
		//CBossController* pController = static_cast<CBossController*>(m_pController);
		//pController->GetAttackMessage(0);

		return BT_SUCCESS;
	}

	_float fDistance = Vec3::DistanceSquared(m_vTargetPos, m_pGameObject->GetTransform()->GetPosition());
	if (m_fTimeSum < m_vecAnimIndexTime[0].second * 0.4f)
	{
		if (fDistance > 4.f)
		{
			CBossController* pController = static_cast<CBossController*>(m_pController);
			pController->GetMaxSpeedMessage();
			pController->GetTranslateMessage(m_pGameObject->GetTransform()->GetForward());
		}

		if (4 == m_iFrameCounter++)
		{
			CMoloch_MotionTrail::MOTIONTRAIL_DESC desc{ m_pModel, &m_pModel->GetTweenDesc(), m_pGameObject->GetTransform()->WorldMatrix(), 0.18f };
			m_pGameInstance->CreateObject(TEXT("Prototype_GameObject_Moloch_MotionTrail"), LAYERTAG::IGNORECOLLISION, &desc);
			m_iFrameCounter = 0;
		}
	}

	m_fTimeSum += fTimeDelta;

	return BT_RUNNING;
}

void CMoloch_BT_FullDash2::OnEnd()
{
	CBossController* pController = static_cast<CBossController*>(m_pController);
	pController->GetDashMessage(false);

	static_cast<CRigidDynamic*>(m_pGameObject->GetRigidBody())->IsKinematic(true);
	Super::OnEnd();
}

void CMoloch_BT_FullDash2::ConditionalAbort(const _float& fTimeDelta)
{
}

_bool CMoloch_BT_FullDash2::IsZeroHP()
{
	if (static_cast<CBossController*>(m_pController)->IsZeroHP())
		return true;

	return false;
}

CGameObject* CMoloch_BT_FullDash2::GetTarget()
{
	BLACKBOARD& hashBlackBoard = m_pBehaviorTree->GetBlackBoard();
	const auto& target = hashBlackBoard.find(TEXT("Target"));
	CGameObject* pPlayer = GET_VALUE(CGameObject, target);

	return pPlayer;
}

CMoloch_BT_FullDash2* CMoloch_BT_FullDash2::Create(CGameObject* pGameObject, CBehaviorTree* pBehaviorTree, const BEHAVEANIMS& tBehaveAnim, CMonoBehaviour* pController)
{
	CMoloch_BT_FullDash2* pInstance = new CMoloch_BT_FullDash2;

	if (FAILED(pInstance->Initialize(pGameObject, pBehaviorTree, tBehaveAnim, pController)))
	{
		MSG_BOX("Failed to Created : CMoloch_BT_FullDash2");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CMoloch_BT_FullDash2::Free()
{
	Super::Free();
}