#include "BT_Composite.h"
#include "GameInstance.h"
#include "GameObject.h"
#include "MonoBehaviour.h"

CBT_Composite::CBT_Composite()
{
}

CBT_Composite::CBT_Composite(const CBT_Composite& rhs)
	:Super(rhs)
{
}

HRESULT CBT_Composite::Initialize(CGameObject* pGameObject, CBehaviorTree* pBehaviorTree, CMonoBehaviour* pController, CompositeType eCompositeType)
{
	Super::Initialize(pGameObject, pBehaviorTree, pController);
	m_eCompositeType = eCompositeType;

	return S_OK;
}

void CBT_Composite::OnStart(const _float& fTimeDelta)
{
	m_RunningChild = m_vecChildren.begin();
}

CBT_Node::BT_RETURN CBT_Composite::OnUpdate(const _float& fTimeDelta)
{
	while (true)
	{
		m_eReturn = (*m_RunningChild)->Tick(fTimeDelta);

		switch (m_eCompositeType)
		{
		case CompositeType::SELECTOR:
		{
			if (BT_FAIL != m_eReturn)
				return m_eReturn;

			if (++m_RunningChild == m_vecChildren.end())
				return BT_FAIL;
		}
		break;
		case CompositeType::SEQUENCE:
		{
			if (BT_SUCCESS != m_eReturn)
				return m_eReturn;

			if (++m_RunningChild == m_vecChildren.end())
				return BT_SUCCESS;
		}
		break;
		}
	}
}

void CBT_Composite::OnEnd(const _float& fTimeDelta)
{
}

HRESULT CBT_Composite::SetAbort(CBT_Abort* pAbort)
{
	if (m_pConditionalAbort)
		return E_FAIL;

	m_pConditionalAbort = pAbort;
	return S_OK;
}

void CBT_Composite::Free()
{
	Super::Free();
}