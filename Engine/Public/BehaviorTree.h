#pragma once
#include "MonoBehaviour.h"

BEGIN(Engine)

class CBT_Node;
class ENGINE_DLL CBehaviorTree final : public CMonoBehaviour
{
	using Super = CMonoBehaviour;
private:
	CBehaviorTree(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CBehaviorTree(const CBehaviorTree& rhs);
	virtual ~CBehaviorTree() = default;
	
public:
	virtual HRESULT Initialize_Prototype()				override;
	virtual HRESULT Initialize(void* pArg)				override;
	virtual void	Tick(const _float& fTimeDelta)		override;
	virtual void	LateTick(const _float& fTimeDelta)	override;
	virtual void	DebugRender()						override;

	void			PushStack(CBT_Node* pNode)	{ m_NodeStack.push(pNode); }
	void			PopStack()					{ m_NodeStack.pop(); }

public:
	CBT_Node*			m_pRootNode = nullptr;
	stack<CBT_Node*>	m_NodeStack;

public:
	static CBehaviorTree* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CComponent* Clone(CGameObject* pGameObject, void* pArg)	override;
	virtual void Free() override;
};

END