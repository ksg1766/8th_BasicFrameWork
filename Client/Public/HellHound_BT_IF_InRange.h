#pragma once
#include "Client_Defines.h"
#include "BT_Decorator.h"
//
#include "GameObject.h"
#include "MonsterController.h"

BEGIN(Client)

class CHellHound_BT_IF_InRange : public CBT_Decorator
{
	using Super = CBT_Decorator;
private:
	CHellHound_BT_IF_InRange() = default;
	CHellHound_BT_IF_InRange(const CHellHound_BT_IF_InRange& rhs) = delete;
	virtual ~CHellHound_BT_IF_InRange() = default;

	virtual void		OnStart() override
	{

	}

	virtual BT_RETURN	OnUpdate(const _float& fTimeDelta) override
	{
		//if (attackRange == hashBlackBoard.end())
		//{
		//	__debugbreak(); // ���� �̷��� ������ �ȵǴ� �̸� BT�̴ϼȶ������ �߰�������
		//}
		if (!IsInRange())
			return BT_FAIL;
		else
			return m_vecChildren[0]->Tick(fTimeDelta);
	}

	virtual void		OnEnd() override
	{

	}

private:
	_bool	IsInRange()
	{
		BLACKBOARD& hashBlackBoard = m_pBehaviorTree->GetBlackBoard();
		const auto& attackRange = hashBlackBoard.find(TEXT("AttackRange"));
		const auto& target = hashBlackBoard.find(TEXT("Target"));

		if ((GET_VALUE(CGameObject, target)->GetTransform()->GetPosition() - m_pGameObject->GetTransform()->GetPosition()).Length() < *GET_VALUE(_float, attackRange))
			return true;
		else
			return false;
	}

public:
	static	CHellHound_BT_IF_InRange* Create(CGameObject* pGameObject, CBehaviorTree* pBehaviorTree, CMonoBehaviour* pController, DecoratorType eDecoratorType)
	{
		CHellHound_BT_IF_InRange* pInstance = new CHellHound_BT_IF_InRange;

		if (FAILED(pInstance->Initialize(pGameObject, pBehaviorTree, pController, eDecoratorType)))
		{
			MSG_BOX("Failed to Created : CHellHound_BT_IF_InRange");
			Safe_Release(pInstance);
		}

		return pInstance;
	}
	virtual void Free() override
	{
		Super::Free();
	}
};

END