#pragma once
#include "BT_Node.h"

BEGIN(Engine)

class CGameObject;
class CModel;
class CBehaviorTree;
class CMonoBehaviour;
class ENGINE_DLL CBT_Action : public CBT_Node
{
	using Super = CBT_Node;
public:
	typedef struct tagBehaveAnimations
	{	// cache
		vector<wstring>	vecAnimations;
	}BEHAVEANIMS;

protected:
	CBT_Action();
	CBT_Action(const CBT_Action& rhs);
	virtual ~CBT_Action() = default;
	
public:
	virtual HRESULT		Initialize(CGameObject* pGameObject, CBehaviorTree* pBehaviorTree, const BEHAVEANIMS& tStateAnim, CMonoBehaviour* pController);

	virtual BT_NODETYPE	NodeType() override { return BT_NODETYPE::ACTION; }

protected:
	CModel*						m_pModel = nullptr;
	_int						m_iCurrAnimation = 0;
	vector<pair<_int, _float>>	m_vecAnimIndexTime;
	_float						m_fTimeSum = 0.f;

public:
	virtual void Free() override;
};

END