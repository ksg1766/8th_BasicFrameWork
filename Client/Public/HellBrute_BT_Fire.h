#pragma once
#include "Client_Defines.h"
#include "BT_Action.h"

BEGIN(Client)

class CHellBrute_BT_Fire final : public CBT_Action
{
	using Super = CBT_Action;
private:
	CHellBrute_BT_Fire();
	CHellBrute_BT_Fire(const CHellBrute_BT_Fire& rhs) = delete;
	virtual ~CHellBrute_BT_Fire() = default;
	
	virtual void		OnStart()							override;
	virtual BT_RETURN	OnUpdate(const _float& fTimeDelta)	override;
	virtual void		OnEnd()								override;

private:
	virtual void		ConditionalAbort(const _float& fTimeDelta)	override;
	_bool				IsInRange();
	_bool				IsInSight();
	_bool				IsZeroHP();

public:
	static	CHellBrute_BT_Fire* Create(CGameObject* pGameObject, CBehaviorTree* pBehaviorTree, const BEHAVEANIMS& tBehaveAnim, CMonoBehaviour* pController);
	virtual void Free() override;
};

END