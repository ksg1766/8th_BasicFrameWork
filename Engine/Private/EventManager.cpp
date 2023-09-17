#include "EventManager.h"
#include "LevelManager.h"
#include "ObjectManager.h"
#include "GameObject.h"
#include "Layer.h"
#include "RigidDynamic.h"
#include "ColliderSphere.h"
#include "QuadTree.h"
#include "QuadTreeNode.h"

IMPLEMENT_SINGLETON(CEventManager);

CEventManager::CEventManager()
{
}

void CEventManager::LastTick()
{
	for (size_t i = 0; i < m_vecDead.size(); ++i)
	{
		// 여기에서 삭제 해주고, Level에서는 벡터 원소만 날려 줌.
		Safe_Release(m_vecDead[i]);
	}
	m_vecDead.clear();

	for (size_t i = 0; i < m_vecEvent.size(); ++i)
	{
		Execute(m_vecEvent[i]);
	}
	m_vecEvent.clear();
}

void CEventManager::CreateObject(CGameObject* pObj, LAYERTAG eLayer)
{
	tagEvent evn = {};
	evn.eEvent = EVENT_TYPE::CREATE_OBJECT;
	evn.lParam = (DWORD_PTR)pObj;
	evn.wParam = (DWORD_PTR)eLayer;

	AddEvent(evn);
}

void CEventManager::DeleteObject(CGameObject* pObj)
{
	tagEvent evn = {};
	evn.eEvent = EVENT_TYPE::DELETE_OBJECT;
	evn.lParam = (DWORD_PTR)pObj;

	AddEvent(evn);
}

void CEventManager::LevelChange(CLevel* pLevel, _uint iLevelId)
{
	tagEvent evn = {};
	evn.eEvent = EVENT_TYPE::LEVEL_CHANGE;
	evn.lParam = (DWORD_PTR)pLevel;
	evn.wParam = (DWORD_PTR)iLevelId;

	AddEvent(evn);
}

void CEventManager::Execute(const tagEvent& eve)
{
	switch (eve.eEvent)
	{
	case EVENT_TYPE::CREATE_OBJECT:
	{
		CGameObject* pNewObject = (CGameObject*)eve.lParam;
		LAYERTAG eLayer = (LAYERTAG)eve.wParam;

		(*CObjectManager::GetInstance()->GetCurrentLevelLayers())[eLayer]->Add_GameObject(pNewObject, eLayer);
	}
	break;
	case EVENT_TYPE::DELETE_OBJECT:
	{
		CGameObject* pDeadObject = (CGameObject*)eve.lParam;

		pDeadObject->SetDeadState(true);
		m_vecDead.push_back(pDeadObject);
	}
	break;
	case EVENT_TYPE::LEVEL_CHANGE:
	{
		
	}
	break;
	}
}

void CEventManager::Free()
{
	__super::Free();
}
