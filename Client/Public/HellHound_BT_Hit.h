#pragma once
#include "Client_Defines.h"
#include "BT_Action.h"

BEGIN(Client)

class CHellHound_BT_Hit : public CBT_Action
{
	using Super = CBT_Action;
private:
	CHellHound_BT_Hit();
	CHellHound_BT_Hit(const CHellHound_BT_Hit& rhs) = delete;
	virtual ~CHellHound_BT_Hit() = default;
	
	virtual void		OnStart()							override;
	virtual BT_RETURN	OnUpdate(const _float& fTimeDelta)	override;
	virtual void		OnEnd()								override;

public:
	static	CHellHound_BT_Hit* Create(CGameObject* pGameObject, CBehaviorTree* pBehaviorTree, const BEHAVEANIMS& tBehaveAnim, CMonoBehaviour* pController);
	virtual void Free() override;
};

END