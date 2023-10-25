#pragma once
#include "MonoBehaviour.h"
#include "Hasher.h"

BEGIN(Engine)

class CBT_Node;
class CBT_Composite;
class ENGINE_DLL CBehaviorTree final : public CMonoBehaviour
{
	using Super = CMonoBehaviour;
protected:
	CBehaviorTree(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CBehaviorTree(const CBehaviorTree& rhs);
	virtual ~CBehaviorTree() = default;
	
public:
	virtual HRESULT Initialize_Prototype()				override;
	virtual HRESULT Initialize(void* pArg)				override;
	virtual void	Tick(const _float& fTimeDelta)		override;
	virtual void	LateTick(const _float& fTimeDelta)	override;
	virtual void	DebugRender()						override;

	void			SetRoot(CBT_Composite* pComposite)	{ m_pRootNode = pComposite; }
	void			PushStack(CBT_Node* pNode)			{ m_NodeStack.push(pNode); }
	void			PopStack()							{ m_NodeStack.pop(); }

protected:
	CBT_Composite*		m_pRootNode = nullptr;
	stack<CBT_Node*>	m_NodeStack;
	using BLACKBOARD = unordered_map<wstring, _float, djb2Hasher>;
	BLACKBOARD			m_BlackBoard;

public:
	static CBehaviorTree* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CComponent* Clone(CGameObject* pGameObject, void* pArg)	override;
	virtual void Free() override;
};

END