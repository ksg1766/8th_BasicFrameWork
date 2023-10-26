#pragma once
#include "Client_Defines.h"
#include "BT_Action.h"

BEGIN(Client)

class CHellHound_BT_StepBack final : public CBT_Action
{
	using Super = CBT_Action;
private:
	CHellHound_BT_StepBack();
	CHellHound_BT_StepBack(const CHellHound_BT_StepBack& rhs) = delete;
	virtual ~CHellHound_BT_StepBack() = default;
	
	virtual void		OnStart()							override;
	virtual BT_RETURN	OnUpdate(const _float& fTimeDelta)	override;
	virtual void		OnEnd()								override;

private:
	virtual void		ConditionalAbort(const _float& fTimeDelta)	override;
	void				StartCoolDown();
	void				RunAttackCoolDown(const _float& fTimeDelta);

public:
	static	CHellHound_BT_StepBack* Create(CGameObject* pGameObject, CBehaviorTree* pBehaviorTree, const BEHAVEANIMS& tBehaveAnim, CMonoBehaviour* pController);
	virtual void Free() override;
};

END