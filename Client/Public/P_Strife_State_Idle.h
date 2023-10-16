#pragma once
#include "Client_Defines.h"
#include "Component.h"
#include "State.h"

BEGIN(Engine)

class CGameObject;

END

BEGIN(Client)

class CP_Strife_State_Idle : public CState
{
	using Super = CState;

private:
	CP_Strife_State_Idle();
	CP_Strife_State_Idle(const CP_Strife_State_Idle& rhs);
	virtual ~CP_Strife_State_Idle() = default;
	
public:
	virtual HRESULT	Enter(_int iIndex = 0)				override;

	virtual void	Tick(const _float& fTimeDelta)		override;
	virtual void	LateTick(const _float& fTimeDelta)	override;

	virtual void	Exit()								override;


public:
	static	CP_Strife_State_Idle* Create(const wstring& strStateName, CGameObject* pGameObject, const STATEANIMS& tStateAnim);
	virtual void Free() override;
};

END