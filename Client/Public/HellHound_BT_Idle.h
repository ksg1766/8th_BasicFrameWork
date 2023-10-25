#pragma once
#include "Client_Defines.h"
#include "BT_Action.h"

BEGIN(Client)

class CHellHound_BT_Idle : public CBT_Action
{
	using Super = CBT_Action;
private:
	CHellHound_BT_Idle();
	CHellHound_BT_Idle(const CHellHound_BT_Idle& rhs) = delete;
	virtual ~CHellHound_BT_Idle() = default;
	
	virtual void		OnStart()							override;
	virtual BT_RETURN	OnUpdate(const _float& fTimeDelta)	override;
	virtual void		OnEnd()								override;

public:
	static	CHellHound_BT_Idle* Create(CGameObject* pGameObject, CBehaviorTree* pBehaviorTree, const BEHAVEANIMS& tBehaveAnim, CMonoBehaviour* pController);
	virtual void Free() override;
};

END