#pragma once

#include "Base.h"
#include "LevelManager.h"
#include "PoolManager.h"
/* ���ӳ��� ���� ��ü���� ���������� ������ü���� �����Ѵ�.(����) */
/* ���ӳ��� ���� ��ü���� �����Ѵ�.(�纻) */

BEGIN(Engine)

class CObjectManager final : public CBase
{
	DECLARE_SINGLETON(CObjectManager)

private:
	CObjectManager();
	virtual ~CObjectManager() = default;

public:
	void Tick(const _float& fTimeDelta);
	void LateTick(const _float& fTimeDelta);
	void DebugRender();
	void Clear(_uint iLevelIndex);

	HRESULT Reserve_Manager(_uint iNumLevels);
	HRESULT Add_Prototype(const wstring& strPrototypeTag, class CGameObject* pPrototype);
	HRESULT Add_GameObject(_uint iLevelIndex, const LAYERTAG& eLayerTag, const wstring& strPrototypeTag, void* pArg);
	
	using LAYERS = map<LAYERTAG, class CLayer*>;
	LAYERS* GetCurrentLevelLayers() const { return &m_pLayers[CLevelManager::GetInstance()->GetCurrentLevelIndex()]; }

private:
	/* ������ü���� �������� �����ұ�?! */
	map<const wstring, class CGameObject*>			m_Prototypes;

private:
	/* �纻��ü���� �������� �׷�(CLayer)��� �����Ѵ�. */
	_uint											m_iNumLevels = { 0 };

	map<LAYERTAG, class CLayer*>*					m_pLayers = { nullptr };

private:
	class CGameObject* Find_Prototype(const wstring& strPrototypeTag);
	class CLayer* Find_Layer(_uint iLevelIndex, const LAYERTAG& eLayerTag);

	friend HRESULT CPoolManager::Reserve_Pool(const wstring&, const _uint&, void*);

public:
	virtual void Free() override;
};

END