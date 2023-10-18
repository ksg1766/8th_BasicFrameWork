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
	Input(fTimeDelta);
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
		return m_vecTransition[Trans::IDLE];
	}

	if (Anims::AIM_IDLE == m_iCurrAnimation)
	{
		if (pController->Run())
		{
			Enter(Anims::AIM_WALK);
		}
	}
	else if (Anims::AIM_WALK == m_iCurrAnimation)
	{
		if (!pController->Run())
		{
			Enter(Anims::AIM_IDLE);
		}
	}
	
	return m_strStateName;
}

void CP_Strife_State_Aim::Input(const _float& fTimeDelta)
{
	CPlayerController* pController = static_cast<CPlayerController*>(m_pController);

	if (KEY_PRESSING(KEY::W) || KEY_DOWN(KEY::W))
	{
		pController->GetMoveMessage(Vec3::UnitZ);
	}
	if (KEY_PRESSING(KEY::A) || KEY_DOWN(KEY::A))
	{
		pController->GetMoveMessage(-Vec3::UnitX);
	}
	if (KEY_PRESSING(KEY::S) || KEY_DOWN(KEY::S))
	{
		pController->GetMoveMessage(-Vec3::UnitZ);
	}
	if (KEY_PRESSING(KEY::D) || KEY_DOWN(KEY::D))
	{
		pController->GetMoveMessage(Vec3::UnitX);
	}

	const POINT& p = CGameInstance::GetInstance()->GetMousePos();
	if (p.x > 1280 || p.x < 0 || p.y > 720 || p.y < 0)
		return;

	if (MOUSE_DOWN(MOUSEKEYSTATE::DIM_LB) || MOUSE_PRESSING(MOUSEKEYSTATE::DIM_LB))
		pController->Fire(fTimeDelta, CStrife_Ammo::AmmoType::DEFAULT);
	if (MOUSE_DOWN(MOUSEKEYSTATE::DIM_RB) || MOUSE_PRESSING(MOUSEKEYSTATE::DIM_RB))
		;//АјАн
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