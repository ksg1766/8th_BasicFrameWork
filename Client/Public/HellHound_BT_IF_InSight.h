#pragma once
#include "Client_Defines.h"
#include "GameObject.h"
#include "GameInstance.h"
#include "BT_Decorator.h"
#include "Layer.h"

BEGIN(Client)

class CHellHound_BT_IF_InSight : public CBT_Decorator
{
	using Super = CBT_Decorator;
private:
	CHellHound_BT_IF_InSight() = default;
	CHellHound_BT_IF_InSight(const CHellHound_BT_IF_InSight& rhs) = delete;
	virtual ~CHellHound_BT_IF_InSight() = default;

	virtual void		OnStart() override
	{

	}

	virtual BT_RETURN	OnUpdate(const _float& fTimeDelta) override
	{
		if (IsInSight())	// 시야에 있는지 없는지 판단해서 키값 삽입or삭제(시야 말고 맞았는지도 추가해야함...)
			return m_vecChildren[0]->Tick(fTimeDelta);
		else				// 키값이 없으면 타겟이 시야에 없는 것
			return BT_FAIL;
	}

	virtual void		OnEnd() override
	{

	}

private:
	_bool	IsInSight()
	{
		map<LAYERTAG, class CLayer*>& mapLayers = m_pGameInstance->GetCurrentLevelLayers();
		const map<LAYERTAG, class CLayer*>::iterator& pPlayerLayer = mapLayers.find(LAYERTAG::PLAYER);

		if (pPlayerLayer == mapLayers.end())	// 애초에 플레이어가 없다면 시야에 있을리도 없음
		{
			__debugbreak();
			return false;
		}
		else
		{
			BLACKBOARD& hashBlackBoard = m_pBehaviorTree->GetBlackBoard();
			const auto& tSight = hashBlackBoard.find(TEXT("Sight"));
			const auto& tTarget = hashBlackBoard.find(TEXT("Target"));

			CGameObject* pPlayer = pPlayerLayer->second->GetGameObjects().front();
			if ((pPlayer->GetTransform()->GetPosition() - m_pGameObject->GetTransform()->GetPosition()).Length() < *GET_VALUE(_float, tSight))	// 시야에 있다면
			{				
				if (tTarget == hashBlackBoard.end())	// 타겟의 키값이 블랙보드에 없다면(이전에 없었으면 데이터도 없어야 함) 키값 추가해줌.
				{
					tagBlackBoardData<CGameObject*>* pTarget = new tagBlackBoardData<CGameObject*>(pPlayer);

					hashBlackBoard.emplace(TEXT("Target"), pTarget);

					return true;
				}
				else	// 타겟의 키값이 블랙보드에 있다면(이전에도 있었단 뜻인데 이럴 수가 있나..?)그냥 true.
				{
					//__debugbreak();
					return true;
				}
			}
			else // 시야에 없다면
			{
				BLACKBOARD& hashBlackBoard = m_pBehaviorTree->GetBlackBoard();

				if (tTarget != hashBlackBoard.end())	// 근데 키값이 있다면 제거
				{
					hashBlackBoard.erase(tTarget);
					return false;
				}
				else // 없다면 바로 false
				{
					return false;
				}
			}
		}
	}

public:
	static	CHellHound_BT_IF_InSight* Create(CGameObject* pGameObject, CBehaviorTree* pBehaviorTree, CMonoBehaviour* pController, DecoratorType eDecoratorType)
	{
		CHellHound_BT_IF_InSight* pInstance = new CHellHound_BT_IF_InSight;

		if (FAILED(pInstance->Initialize(pGameObject, pBehaviorTree, pController, eDecoratorType)))
		{
			MSG_BOX("Failed to Created : CHellHound_BT_IF_InSight");
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