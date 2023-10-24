#pragma once
#include "BT_Node.h"

BEGIN(Engine)

class ENGINE_DLL CBT_Decorator : public CBT_Node
{
	using Super = CBT_Node;
public:
	enum class DecoratorType { IF, WHILE, REPEAT, TYPE_END };

protected:
	CBT_Decorator();
	CBT_Decorator(const CBT_Decorator& rhs);
	virtual ~CBT_Decorator() = default;
	
public:
	HRESULT				Initialize(CGameObject* pGameObject, CMonoBehaviour* pController, DecoratorType eDecoratorType);

	virtual void		OnStart(const _float& fTimeDelta)	override;
	virtual BT_RETURN	OnUpdate(const _float& fTimeDelta)	override;
	virtual void		OnEnd(const _float& fTimeDelta)		override;

	virtual BT_NODETYPE	NodeType() override								{ return BT_NODETYPE::DECORATOR; }

	void				SetFunctionPtr(function<_bool()> ftCondition)	{ m_ftCondition = ftCondition; }
	void				SetLimitCount(_int iLimitCount)					{ m_iLimitCount = iLimitCount; }

protected:
	DecoratorType		m_eDecoratorType = DecoratorType::TYPE_END;

	_int				m_iLimitCount = 0;
	_int				m_iCounter = 0;

	function<_bool()>	m_ftCondition;

public:
	virtual void Free() override;
};

END