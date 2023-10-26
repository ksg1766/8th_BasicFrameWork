#pragma once
#include "Client_Defines.h"
#include "BT_Decorator.h"

BEGIN(Client)

class CHellHound_BT_IF_AttackCool : public CBT_Decorator
{
	using Super = CBT_Decorator;
private:
	CHellHound_BT_IF_AttackCool() = default;
	CHellHound_BT_IF_AttackCool(const CHellHound_BT_IF_AttackCool& rhs) = delete;
	virtual ~CHellHound_BT_IF_AttackCool() = default;

	virtual void	OnStart() override
	{

	}

	virtual BT_RETURN	OnUpdate(const _float& fTimeDelta) override
	{
		BLACKBOARD& hashBlackBoard = m_pBehaviorTree->GetBlackBoard();
		const auto& fAttackCoolDown = hashBlackBoard.find(TEXT("AttackCoolDown"));

		if (fAttackCoolDown == hashBlackBoard.end())	// Ű ���ٸ�(�� �� ���Ҵٸ�) �ڽĳ�� ����
			return m_vecChildren[0]->Tick(fTimeDelta);
		else											// Ű �ִٸ�(�� ���� ���̶��) FAIL
			return BT_FAIL;
	}

	virtual void	OnEnd() override
	{

	}

public:
	static	CHellHound_BT_IF_AttackCool* Create(CGameObject* pGameObject, CBehaviorTree* pBehaviorTree, CMonoBehaviour* pController, DecoratorType eDecoratorType)
	{
		CHellHound_BT_IF_AttackCool* pInstance = new CHellHound_BT_IF_AttackCool;

		if (FAILED(pInstance->Initialize(pGameObject, pBehaviorTree, pController, eDecoratorType)))
		{
			MSG_BOX("Failed to Created : CHellHound_BT_IF_AttackCool");
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