#include "stdafx.h"
#include "Moloch_BT_FullDash1.h"
#include "GameInstance.h"
#include "Layer.h"
#include "GameObject.h"
#include "BossController.h"
#include "Moloch_MotionTrail.h"
#include "TremorCrystal.h"
#include "Particle.h"
#include "ParticleController.h"

CMoloch_BT_FullDash1::CMoloch_BT_FullDash1()
{
}

void CMoloch_BT_FullDash1::OnStart()
{
	Super::OnStart(0);

	m_bAttack = false;
	m_vTargetPos = GetTarget()->GetTransform()->GetPosition();

	CBossController* pController = static_cast<CBossController*>(m_pController);
	pController->Look(m_vTargetPos);
}

CBT_Node::BT_RETURN CMoloch_BT_FullDash1::OnUpdate(const _float& fTimeDelta)
{
	if (IsZeroHP())
		return BT_FAIL;

	if (m_fTimeSum > m_vecAnimIndexTime[0].second * 0.9f)
	{


		//CBossController* pController = static_cast<CBossController*>(m_pController);
		//pController->GetAttackMessage(0);

		return BT_SUCCESS;
	}

	if (!m_bAttack)
	{
		if (m_fTimeSum > m_vecAnimIndexTime[0].second * 0.5f)
		{
			CTransform* pTransform = m_pGameObject->GetTransform();
			const Vec3& vPos = pTransform->GetPosition();

			Vec3 vCreatePosition[4] = {
				vPos + 3.f * (pTransform->GetForward() + 0.75f * pTransform->GetRight()),
				vPos + 3.f * (pTransform->GetForward() - 0.75f * pTransform->GetRight()),
				vPos + 2.f * (pTransform->GetForward() + 1.1f * pTransform->GetRight()),
				vPos + 2.f * (pTransform->GetForward() - 1.1f * pTransform->GetRight())
			};

			CTremorCrystal::EFFECT_DESC desc;
			desc.fLifeTime = 10.f;

			CGameObject* pCrystal[4] = {
				m_pGameInstance->CreateObject(TEXT("Prototype_GameObject_TremorCrystal_A"), LAYERTAG::IGNORECOLLISION, &desc),
				m_pGameInstance->CreateObject(TEXT("Prototype_GameObject_TremorCrystal_A"), LAYERTAG::IGNORECOLLISION, &desc),
				m_pGameInstance->CreateObject(TEXT("Prototype_GameObject_TremorCrystal_A"), LAYERTAG::IGNORECOLLISION, &desc),
				m_pGameInstance->CreateObject(TEXT("Prototype_GameObject_TremorCrystal_A"), LAYERTAG::IGNORECOLLISION, &desc)
			};

			for (_int i = 0; i < 4; ++i)
			{
				pCrystal[i]->GetTransform()->Translate(vCreatePosition[i]);
			}

			CParticleController::PARTICLE_DESC tParticleDesc;
			tParticleDesc.eType = CParticleController::ParticleType::EXPLODE;
			tParticleDesc.vSpeedMax = _float3(5.f, 10.f, 5.f);
			tParticleDesc.vSpeedMin = _float3(-5.f, 7.f, -5.f);
			tParticleDesc.vColor = Color(1.f, 0.f, 0.1f, 1.f);

			for (_int i = 0; i < 4; ++i)
			{
				tParticleDesc.vCenter = pCrystal[i]->GetTransform()->GetPosition();
				for (_int i = 0; i < 15; ++i)
					m_pGameInstance->CreateObject(TEXT("Prototype_GameObject_Particle"), LAYERTAG::IGNORECOLLISION, &tParticleDesc);
			}

			m_bAttack = true;
		}
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

void CMoloch_BT_FullDash1::OnEnd()
{
	CBossController* pController = static_cast<CBossController*>(m_pController);
	pController->GetDashMessage(false);

	static_cast<CRigidDynamic*>(m_pGameObject->GetRigidBody())->IsKinematic(true);
	Super::OnEnd();
}

void CMoloch_BT_FullDash1::ConditionalAbort(const _float& fTimeDelta)
{
}

_bool CMoloch_BT_FullDash1::IsZeroHP()
{
	if (static_cast<CBossController*>(m_pController)->IsZeroHP())
		return true;

	return false;
}

CGameObject* CMoloch_BT_FullDash1::GetTarget()
{
	BLACKBOARD& hashBlackBoard = m_pBehaviorTree->GetBlackBoard();
	const auto& target = hashBlackBoard.find(TEXT("Target"));
	CGameObject* pPlayer = GET_VALUE(CGameObject, target);

	return pPlayer;
}

CMoloch_BT_FullDash1* CMoloch_BT_FullDash1::Create(CGameObject* pGameObject, CBehaviorTree* pBehaviorTree, const BEHAVEANIMS& tBehaveAnim, CMonoBehaviour* pController)
{
	CMoloch_BT_FullDash1* pInstance = new CMoloch_BT_FullDash1;

	if (FAILED(pInstance->Initialize(pGameObject, pBehaviorTree, tBehaveAnim, pController)))
	{
		MSG_BOX("Failed to Created : CMoloch_BT_FullDash1");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CMoloch_BT_FullDash1::Free()
{
	Super::Free();
}