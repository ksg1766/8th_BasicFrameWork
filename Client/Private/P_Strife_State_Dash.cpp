#include "stdafx.h"
#include "GameObject.h"
#include "GameInstance.h"
#include "P_Strife_State_Dash.h"
#include "PlayerController.h"

CP_Strife_State_Dash::CP_Strife_State_Dash()
	:Super()
{
	m_strStateName = TEXT("Dash");
	m_vecTransition.push_back(TEXT("Idle"));
	m_vecTransition.push_back(TEXT("Run"));
	m_vecTransition.push_back(TEXT("Aim"));
	m_vecTransition.push_back(TEXT("Jump"));
	m_vecTransition.push_back(TEXT("Impact"));
}

HRESULT CP_Strife_State_Dash::Enter(_int i)
{
	m_iCurrAnimation = i;
	Super::Enter(m_vecAnimIndexTime[i].first);

	return S_OK;
}

void CP_Strife_State_Dash::Tick(const _float& fTimeDelta)
{
	CPlayerController* pController = static_cast<CPlayerController*>(m_pController);

	m_fTimeSum += fTimeDelta;
	//
}

const wstring& CP_Strife_State_Dash::LateTick(const _float& fTimeDelta)
{
	return Transition();
}

void CP_Strife_State_Dash::Exit()
{
}

const wstring& CP_Strife_State_Dash::Transition()
{
	//CPlayerController* pController = static_cast<CPlayerController*>(m_pController);
	
	if (Anims::DASH == m_iCurrAnimation)
	{
		if (m_fTimeSum > m_vecAnimIndexTime[m_iCurrAnimation].second)
		{
			Enter(Anims::DASH_END);
			return m_strStateName;
		}
	}

	else if (Anims::DASH_END == m_iCurrAnimation)
	{
		if (m_fTimeSum > m_vecAnimIndexTime[m_iCurrAnimation].second)
		{
			return m_vecTransition[Trans::IDLE];
		}
	}

	return m_strStateName;
}

CP_Strife_State_Dash* CP_Strife_State_Dash::Create(CGameObject* pGameObject, const STATEANIMS& tStateAnim, CMonoBehaviour* pController)
{
	CP_Strife_State_Dash* pInstance = new CP_Strife_State_Dash();

	if (FAILED(pInstance->Initialize(pGameObject, tStateAnim, pController)))
	{
		MSG_BOX("Failed to Created : CP_Strife_State_Dash");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CP_Strife_State_Dash::Free()
{
	Super::Free();
}