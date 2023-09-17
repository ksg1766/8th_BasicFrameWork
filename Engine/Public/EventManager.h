#pragma once
#include "Base.h"

BEGIN(Engine)

enum class EVENT_TYPE { CREATE_OBJECT, DELETE_OBJECT, LEVEL_CHANGE, EVENT_END };

struct tagEvent
{
	EVENT_TYPE	eEvent;
	DWORD_PTR	lParam;
	DWORD_PTR	wParam;
};

class ENGINE_DLL CEventManager :
	public CBase
{
	DECLARE_SINGLETON(CEventManager);

private:
	CEventManager();
	virtual ~CEventManager() = default;

public:
	void LastTick();

	void AddEvent(const tagEvent& eve) { m_vecEvent.push_back(eve); }

	void CreateObject(CGameObject* pObj, LAYERTAG eLayer);
	void DeleteObject(CGameObject* pObj);
	void LevelChange(class CLevel* pLevel, _uint iLevelId);

private:
	void Execute(const tagEvent& eve);

private:
	vector<tagEvent> m_vecEvent;
	vector<CGameObject*> m_vecCreate;
	vector<CGameObject*> m_vecDead;

public:
	virtual void Free() override;
};

END