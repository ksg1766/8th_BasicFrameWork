#pragma once
#include "BT_Node.h"

BEGIN(Engine)

class ENGINE_DLL CBT_Abort : public CBT_Node
{
	using Super = CBT_Node;
public:
	enum class AbortType { SELF, LOWERPRIORITY, BOTH, TYPE_END };

protected:
	CBT_Abort();
	CBT_Abort(const CBT_Abort& rhs);
	virtual ~CBT_Abort() = default;
	
public:
	HRESULT				Initialize(CGameObject* pGameObject, CMonoBehaviour* pController, AbortType	eAbortType);

	virtual void		OnStart(const _float& fTimeDelta)		override;
	virtual BT_RETURN	OnUpdate(const _float& fTimeDelta)		override;
	virtual void		OnEnd(const _float& fTimeDelta)			override;
	
	virtual BT_NODETYPE	NodeType() override { return BT_NODETYPE::ABORT; }

protected:
	AbortType	m_eAbortType = AbortType::TYPE_END;
	// _bool	m_bCancel = true;

public:
	virtual void Free() override;
};

END