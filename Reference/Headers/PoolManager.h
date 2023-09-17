#pragma once

#include "Base.h"

BEGIN(Engine)
// 클라이언트로 옮길지 고민
class CGameObject;
class ENGINE_DLL CPoolManager final
	: public CBase
{
	DECLARE_SINGLETON(CPoolManager)

private:
	CPoolManager();
	virtual ~CPoolManager() = default;

public:
	HRESULT	Reserve_Pool(const wstring& strObjectName, const _uint& iReserveCount, void* pArg = nullptr);

	void	Spawn_Object(const wstring& strObjectName, const Vec3& vSpawnPos);
	void	Restore_Object(CGameObject* pGameObject);

private:
	using POOLS = map<const wstring, queue<CGameObject*>>;
	POOLS m_mapPools;

public:
	virtual void Free() override;
};

END