#include "stdafx.h"
#include "GameObject.h"
#include "GameInstance.h"
#include "P_Strife_State_Aim.h"
#include "PlayerController.h"

CP_Strife_State_Aim::CP_Strife_State_Aim()
	:Super()
{
	m_strStateName = TEXT("Aim");
	m_vecTransition.push_back(TEXT("Idle"));
	m_vecTransition.push_back(TEXT("Run"));
	m_vecTransition.push_back(TEXT("Jump"));
	m_vecTransition.push_back(TEXT("Dash"));
	m_vecTransition.push_back(TEXT("Impact"));
}

HRESULT CP_Strife_State_Aim::Enter(_int i)
{
	m_iCurrAnimation = i;
	Super::Enter(m_vecAnimIndexTime[i].first);

	return S_OK;
}

void CP_Strife_State_Aim::Tick(const _float& fTimeDelta)
{
}

const wstring& CP_Strife_State_Aim::LateTick(const _float& fTimeDelta)
{
	return Transition();
}

void CP_Strife_State_Aim::Exit()
{
}

const wstring& CP_Strife_State_Aim::Transition()
{
	CPlayerController* pController = static_cast<CPlayerController*>(m_pController);
	
	if (pController->Dash())
		return m_vecTransition[Trans::DASH];

	if (pController->Jump())
		return m_vecTransition[Trans::JUMP];

	if (!pController->Aim())
	{
		if(pController->Run())
			return m_vecTransition[Trans::RUN];
		else
			return m_vecTransition[Trans::IDLE];
	}

	return m_strStateName;
}

void CP_Strife_State_Aim::Input(const _float& fTimeDelta)
{
	CPlayerController* pController = static_cast<CPlayerController*>(m_pController);

	if (MOUSE_DOWN(MOUSEKEYSTATE::DIM_LB) || MOUSE_PRESSING(MOUSEKEYSTATE::DIM_LB))
		;//공격
	if (MOUSE_DOWN(MOUSEKEYSTATE::DIM_RB) || MOUSE_PRESSING(MOUSEKEYSTATE::DIM_RB))
		;//공격
}

CP_Strife_State_Aim* CP_Strife_State_Aim::Create(CGameObject* pGameObject, const STATEANIMS& tStateAnim, CMonoBehaviour* pController)
{
	CP_Strife_State_Aim* pInstance = new CP_Strife_State_Aim();

	if (FAILED(pInstance->Initialize(pGameObject, tStateAnim, pController)))
	{
		MSG_BOX("Failed to Created : CP_Strife_State_Aim");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CP_Strife_State_Aim::Free()
{
	Super::Free();
}