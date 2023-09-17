#include "..\Public\ObjectManager.h"
#include "Layer.h"
#include "GameObject.h"
#include "RigidBodyBase.h"
#include "CollisionManager.h"

IMPLEMENT_SINGLETON(CObjectManager)

CObjectManager::CObjectManager()
{
}

HRESULT CObjectManager::Reserve_Manager(_uint iNumLevels)
{
	if (nullptr != m_pLayers)
		return E_FAIL;

	m_pLayers = new LAYERS[iNumLevels];

	m_iNumLevels = iNumLevels;

	return S_OK;
}

HRESULT CObjectManager::Add_Prototype(const wstring& strPrototypeTag, CGameObject* pPrototype)
{
	if (nullptr != Find_Prototype(strPrototypeTag))
		return E_FAIL;

	m_Prototypes.emplace(strPrototypeTag, pPrototype);

	return S_OK;
}

HRESULT CObjectManager::Add_GameObject(_uint iLevelIndex, const LAYERTAG& eLayerTag, const wstring& strPrototypeTag, void * pArg)
{
	/* 복제할 사본을 찾는다. */
	CGameObject*		pPrototype = Find_Prototype(strPrototypeTag);
	if (nullptr == pPrototype)
		return E_FAIL;

	/* 원형을 복제하여 사본을 만든다. */
	CGameObject*		pGameObject = pPrototype->Clone(pArg);
	if (nullptr == pGameObject)
		return E_FAIL;

	CLayer*		pLayer = Find_Layer(iLevelIndex, eLayerTag);

	/* 내가 추가할려고 하는 레이어가 없었다. */
	if (nullptr == pLayer)
	{
		/* 레이어를 만들자. */
		pLayer = CLayer::Create();
		pLayer->SetLayerTag(eLayerTag);
		/* 레이어에 객체를 추가하자. */
		pLayer->Add_GameObject(pGameObject, eLayerTag);

		/* 생성한 레이어를 iLevelIndex용 레이어로 추가하였다. */
		m_pLayers[iLevelIndex].emplace(eLayerTag, pLayer);
	}
	else /* 추가하고자 하는 레이어가 있었다. */
		pLayer->Add_GameObject(pGameObject, eLayerTag);

	return S_OK;
}

void CObjectManager::Tick(const _float& fTimeDelta)
{
	for (size_t i = 0; i < m_iNumLevels; i++)
	{
		for (auto& Pair : m_pLayers[i])
		{
			Pair.second->Tick(fTimeDelta);		
		}
	}
}

void CObjectManager::LateTick(const _float& fTimeDelta)
{
	for (size_t i = 0; i < m_iNumLevels; i++)
	{
		for (auto& Pair : m_pLayers[i])
		{
			Pair.second->LateTick(fTimeDelta);
		}
	}
}

void CObjectManager::DebugRender()
{
	for (size_t i = 0; i < m_iNumLevels; i++)
	{
		for (auto& Pair : m_pLayers[i])
		{
			Pair.second->DebugRender();
		}
	}
}

void CObjectManager::Clear(_uint iLevelIndex)
{
	for (auto& Pair : m_pLayers[iLevelIndex])
	{
		Safe_Release(Pair.second);
	}
	m_pLayers[iLevelIndex].clear();
}

CGameObject * CObjectManager::Find_Prototype(const wstring & strPrototypeTag)
{
	auto	iter = m_Prototypes.find(strPrototypeTag);

	if (iter == m_Prototypes.end())
		return nullptr;

	return iter->second;
}

CLayer * CObjectManager::Find_Layer(_uint iLevelIndex, const LAYERTAG& eLayerTag)
{
	if (iLevelIndex >= m_iNumLevels)
		return nullptr;

	auto	iter = m_pLayers[iLevelIndex].find(eLayerTag);

	if (iter == m_pLayers[iLevelIndex].end())
		return nullptr;

	return iter->second;
}

void CObjectManager::Free()
{
	__super::Free();

	for (size_t i = 0; i < m_iNumLevels; i++)
	{
		for (auto& Pair : m_pLayers[i])
		{
			Safe_Release(Pair.second);
		}
		m_pLayers[i].clear();
	}
	Safe_Delete_Array(m_pLayers);

	for (auto& Pair : m_Prototypes)
		Safe_Release(Pair.second);

	m_Prototypes.clear();
}
