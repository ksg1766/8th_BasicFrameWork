#pragma once

#include "Base.h"

BEGIN(Engine)

class CGameObject;
class CPoolManager final : public CBase
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
	struct djb2Hasher
	{
		size_t operator() (const wstring& str) const
		{
			_ulong	hash = 5381;
			_uint	size = str.length();

			for (_uint i = 0; i < size; i++)
				hash = ((hash << 5) + hash) + (str[i]); /* hash * 33 + c */

			return hash;
		}
	};	// HashFunctionObject (djb2)

	using POOLS = unordered_map<const wstring, queue<CGameObject*>, djb2Hasher>;
	POOLS m_hashPools;

public:
	virtual void Free() override;
};

END