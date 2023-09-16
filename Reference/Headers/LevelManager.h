#pragma once

#include "Base.h"

/* Open_Level*/
/* ���� ���ӳ����� Ȱ���ϰ� �ִ� ������ü�� �ּҸ� �����Ѵ�. */
/* ���� ��ü�ÿ� ���� ������ �ı��Ѵ�. + ���� ���������� �������� �ڿ����� �����Ѵ�. */

/* �����ϰ� �ִ� ������ ƽ�� �ݺ������� ȣ�����ش�. */


BEGIN(Engine)

class CLevelManager final : public CBase
{
	DECLARE_SINGLETON(CLevelManager)
private:
	CLevelManager();
	virtual ~CLevelManager() = default;

public:
	void Tick(_float fTimeDelta);
	void LateTick(_float fTimeDelta);
	void DebugRender();

public:
	HRESULT Open_Level(_uint iLevelIndex, class CLevel* pNewLevel);
	_uint GetCurrentLevelIndex() const { return m_iCurrentLevelIndex; }

private:
	_uint						m_iCurrentLevelIndex = { 0 };
	class CLevel*				m_pCurrentLevel = { nullptr };

public:
	virtual void Free() override;
};

END