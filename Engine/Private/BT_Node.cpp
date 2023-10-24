#include "BT_Node.h"
#include "GameInstance.h"
#include "GameObject.h"
#include "MonoBehaviour.h"

CBT_Node::CBT_Node()
{
	m_pGameInstance = GET_INSTANCE(CGameInstance);
}

CBT_Node::CBT_Node(const CBT_Node& rhs)
	:Super(rhs)
	, m_pGameInstance(rhs.m_pGameInstance)
{
	Safe_AddRef(m_pGameInstance);
}

HRESULT CBT_Node::Initialize(CGameObject* pGameObject, CMonoBehaviour* pController)
{
	m_pGameObject = pGameObject;
	m_pController = pController;

	return S_OK;
}

CBT_Node::BT_RETURN CBT_Node::Tick(const _float& fTimeDelta)
{
	if (BT_RETURN::BT_RUNNING != m_eReturn)
		OnStart(fTimeDelta);

	m_eReturn = OnUpdate(fTimeDelta);

	if (BT_RETURN::BT_RUNNING != m_eReturn)
		OnEnd(fTimeDelta);

	return m_eReturn;
}

HRESULT CBT_Node::SetChild(CBT_Node* pChild)
{
	if (BT_NODETYPE::ACTION == NodeType())
		return E_FAIL;
	else if (BT_NODETYPE::DECORATOR == NodeType() && 0 != m_vecChildren.size())
		return E_FAIL;
	else
		m_vecChildren.push_back(pChild);

	return S_OK;
}

void CBT_Node::Free()
{
	Super::Free();
	GET_INSTANCE(CGameInstance);
}