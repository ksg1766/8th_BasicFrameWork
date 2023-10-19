#include "stdafx.h"
#include "GameObject.h"
#include "GameInstance.h"
#include "P_Strife_State_Jump.h"
#include "PlayerController.h"

CP_Strife_State_Jump::CP_Strife_State_Jump()
	:Super()
{
	m_strStateName = TEXT("Jump");
	m_vecTransition.push_back(TEXT("Idle"));
	m_vecTransition.push_back(TEXT("Run"));
	m_vecTransition.push_back(TEXT("Aim"));
	m_vecTransition.push_back(TEXT("Dash"));
	m_vecTransition.push_back(TEXT("Impact"));
}

HRESULT CP_Strife_State_Jump::Enter(_int i)
{
	m_iCurrAnimation = i;
	Super::Enter(m_vecAnimIndexTime[i].first);

	return S_OK;
}

void CP_Strife_State_Jump::Tick(const _float& fTimeDelta)
{
	m_fTimeSum += fTimeDelta;
	Input(fTimeDelta);
}

const wstring& CP_Strife_State_Jump::LateTick(const _float& fTimeDelta)
{
	return Transition();
}

void CP_Strife_State_Jump::Exit()
{
}

const wstring& CP_Strife_State_Jump::Transition()
{
	CPlayerController* pController = static_cast<CPlayerController*>(m_pController);

	/*if (pController->Aim())
		return m_vecTransition[Trans::AIM];*/

	if (Anims::JUMP == m_iCurrAnimation)
	{
		if (m_fTimeSum > m_vecAnimIndexTime[m_iCurrAnimation].second)
		{
			//Enter(Anims::FALL);
			Enter(Anims::LAND);
			return m_strStateName;
		}

		if (pController->IsJump())
		{
			Enter(Anims::JUMP_DOUBLE);
			return m_strStateName;
		}
	}
	else if (Anims::JUMP_DOUBLE == m_iCurrAnimation)
	{
		if (m_fTimeSum > m_vecAnimIndexTime[m_iCurrAnimation].second)
		{
			//Enter(Anims::FALL);
			Enter(Anims::JUMP_LAND_HEAVY);
			return m_strStateName;
		}
	}
	else if (Anims::LAND == m_iCurrAnimation)
	{
		if (m_fTimeSum > m_vecAnimIndexTime[m_iCurrAnimation].second * 0.6f)
		{
			return m_vecTransition[Trans::IDLE];
		}
	}
	else if (Anims::JUMP_LAND_HEAVY == m_iCurrAnimation)
	{
		if (m_fTimeSum > m_vecAnimIndexTime[m_iCurrAnimation].second * 0.6f)
		{
			return m_vecTransition[Trans::IDLE];
		}
	}

	return m_strStateName;
}

void CP_Strife_State_Jump::Input(const _float& fTimeDelta)
{
	CPlayerController* pController = static_cast<CPlayerController*>(m_pController);

	if (KEY_PRESSING(KEY::W) || KEY_DOWN(KEY::W)) pController->GetMoveMessage(Vec3::UnitZ);
	if (KEY_PRESSING(KEY::A) || KEY_DOWN(KEY::A)) pController->GetMoveMessage(-Vec3::UnitX);
	if (KEY_PRESSING(KEY::S) || KEY_DOWN(KEY::S)) pController->GetMoveMessage(-Vec3::UnitZ);
	if (KEY_PRESSING(KEY::D) || KEY_DOWN(KEY::D)) pController->GetMoveMessage(Vec3::UnitX);
}

CP_Strife_State_Jump* CP_Strife_State_Jump::Create(CGameObject* pGameObject, const STATEANIMS& tStateAnim, CMonoBehaviour* pController)
{
	CP_Strife_State_Jump* pInstance = new CP_Strife_State_Jump();

	if (FAILED(pInstance->Initialize(pGameObject, tStateAnim, pController)))
	{
		MSG_BOX("Failed to Created : CP_Strife_State_Jump");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CP_Strife_State_Jump::Free()
{
	Super::Free();
}