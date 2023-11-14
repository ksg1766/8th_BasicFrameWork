#pragma once
#include "Client_Defines.h"
#include "BT_Decorator.h"
#include "BossController.h"
#include "MonsterStats.h"

BEGIN(Client)

class CMoloch_BT_REPEAT : public CBT_Decorator
{
	using Super = CBT_Decorator;
private:
	CMoloch_BT_REPEAT() = default;
	CMoloch_BT_REPEAT(const CMoloch_BT_REPEAT& rhs) = delete;
	virtual ~CMoloch_BT_REPEAT() = default;

	HRESULT	Initialize(CGameObject* pGameObject, CBehaviorTree* pBehaviorTree, CMonoBehaviour* pController, _int iRepeatCount)
	{
		Super::Initialize(pGameObject, pBehaviorTree, pController, DecoratorType::REPEAT);
		m_iRepeatCount = iRepeatCount;

		return S_OK;
	}

	virtual void OnStart() override
	{
		m_iCounter = m_iRepeatCount;
	}

	virtual BT_RETURN OnUpdate(const _float& fTimeDelta) override
	{
		if (IsRepeatEnd())
			return BT_SUCCESS;
		else
		{
			--m_iCounter;
			m_vecChildren[0]->Tick(fTimeDelta);	// �ϴ� �ӽ÷� ���� ����� ������ �׻� FAIL;
			return BT_RUNNING;
		}
	}

	virtual void OnEnd() override
	{
		Reset();
	}

private:
	_bool	IsRepeatEnd()
	{
		if (0 == m_iCounter || IsZeroHP())
			return true;

		return false;
	}

	_bool	IsZeroHP()
	{
		if (static_cast<CBossController*>(m_pController)->IsZeroHP())
			return true;

		return false;
	}

	_int m_iRepeatCount = 0;
	_int m_iCounter = 0;

public:
	static	CMoloch_BT_REPEAT* Create(CGameObject* pGameObject, CBehaviorTree* pBehaviorTree, CMonoBehaviour* pController, _int iRepeatCount)
	{
		CMoloch_BT_REPEAT* pInstance = new CMoloch_BT_REPEAT;

		if (FAILED(pInstance->Initialize(pGameObject, pBehaviorTree, pController, iRepeatCount)))
		{
			MSG_BOX("Failed to Created : CMoloch_BT_REPEAT");
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