#pragma once
#include "Base.h"

BEGIN(Engine)

class CGameInstance;
class CGameObject;
class CMonoBehaviour;
class ENGINE_DLL CBT_Node : public CBase
{
	using Super = CBase;
public:
	enum BT_RETURN { BT_FAIL = -1, BT_RUNNING, BT_SUCCESS, RETURN_END };
	enum class BT_NODETYPE { COMPOSITE, DECORATOR, ABORT, ACTION, TYPE_END };

protected:
	CBT_Node();
	CBT_Node(const CBT_Node& rhs);
	virtual ~CBT_Node() = default;
	
public:
	virtual HRESULT			Initialize(CGameObject* pGameObject, CMonoBehaviour* pController);

	virtual	BT_RETURN		Tick(const _float& fTimeDelta);
	virtual void			OnStart(const _float& fTimeDelta)	PURE;
	virtual BT_RETURN		OnUpdate(const _float& fTimeDelta)	PURE;
	virtual void			OnEnd(const _float& fTimeDelta)		PURE;
	
	virtual BT_NODETYPE		NodeType()							PURE;
	HRESULT					SetChild(CBT_Node* pChild);

protected:
	CGameObject*		m_pGameObject = nullptr;
	CGameInstance*		m_pGameInstance = nullptr;

	CMonoBehaviour*		m_pController = nullptr;

	vector<CBT_Node*>	m_vecChildren;
	BT_RETURN			m_eReturn = BT_RETURN::RETURN_END;

public:
	virtual void Free() override;
};

END