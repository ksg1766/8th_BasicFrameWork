#pragma once
#include "BT_Node.h"

BEGIN(Engine)

class ENGINE_DLL CBT_Action : public CBT_Node
{
	using Super = CBT_Node;
protected:
	CBT_Action();
	CBT_Action(const CBT_Action& rhs);
	virtual ~CBT_Action() = default;
	
public:
	virtual void		OnStart(const _float& fTimeDelta)	override;
	virtual BT_RETURN	OnUpdate(const _float& fTimeDelta)	override;
	virtual void		OnEnd(const _float& fTimeDelta)		override;

	virtual BT_NODETYPE	NodeType() override { return BT_NODETYPE::ACTION; }

protected:

public:
	virtual void Free() override;
};

END