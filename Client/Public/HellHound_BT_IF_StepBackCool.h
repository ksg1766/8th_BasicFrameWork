#pragma once
#include "Client_Defines.h"
#include "BT_Decorator.h"

BEGIN(Client)

class CHellHound_BT_IF_StepBackCool : public CBT_Decorator
{
	using Super = CBT_Decorator;
private:
	CHellHound_BT_IF_StepBackCool() = default;
	CHellHound_BT_IF_StepBackCool(const CHellHound_BT_IF_StepBackCool& rhs) = delete;
	virtual ~CHellHound_BT_IF_StepBackCool() = default;

	virtual void		OnStart() override
	{

	}

	virtual BT_RETURN	OnUpdate(const _float& fTimeDelta) override
	{
		BLACKBOARD& hashBlackBoard = m_pBehaviorTree->GetBlackBoard();
		const auto& fStepBackCoolDown = hashBlackBoard.find(TEXT("StepBackCoolDown"));

		if (fStepBackCoolDown == hashBlackBoard.end())	// 키 없다면(쿨 다 돌았다면) 자식노드 ㄱㄱ
			return m_vecChildren[0]->Tick(fTimeDelta);
		else											// 키 있다면(쿨 도는 중이라면) FAIL
			return BT_FAIL;
	}

	virtual void		OnEnd() override
	{

	}

public:
	static	CHellHound_BT_IF_StepBackCool* Create(CGameObject* pGameObject, CBehaviorTree* pBehaviorTree, CMonoBehaviour* pController, DecoratorType eDecoratorType)
	{
		CHellHound_BT_IF_StepBackCool* pInstance = new CHellHound_BT_IF_StepBackCool;

		if (FAILED(pInstance->Initialize(pGameObject, pBehaviorTree, pController, eDecoratorType)))
		{
			MSG_BOX("Failed to Created : CHellHound_BT_IF_StepBackCool");
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