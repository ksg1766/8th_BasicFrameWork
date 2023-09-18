#pragma once
#include "Base.h"
#include "MurmurHash3.h"

BEGIN(Engine)

class CCollider;
class CSphereCollider;

union COLLIDER_ID
{
	struct
	{
		_uint Left_id;
		_uint Right_id;
	};
	ULONGLONG ID;
};

class ENGINE_DLL CCollisionManager :
    public CBase
{
    DECLARE_SINGLETON(CCollisionManager);

private:
	CCollisionManager();
	virtual ~CCollisionManager() = default;

public:
	void	LateTick_Collision(const _float& fTimeDelta);

public:
	HRESULT Reserve_Manager(_uint iNumLevels);
	//map<ULONGLONG, _bool>& GetMapColInfo() { return m_mapColInfo; }
	//void SetMapColInfo(ULONGLONG ID, _bool bCollision) { m_mapColInfo[ID] = bCollision; }
	
private:
	void	CheckGroup(LAYERTAG eLeft, LAYERTAG eRight);
	void	Reset();
	
	_bool	IsCollided(CCollider* pLeft, CCollider* pRight);

	void	CheckDynamicCollision(LAYERTAG eObjectLeft, LAYERTAG eObjectRight, const _float fTimeDelta);
	void	CheckStaticCollision(LAYERTAG eDynamicLayer, const _float fTimeDelta);
	_bool	CompareMinZ(_float& fLeftMinZ, _float& fRightMinZ);

	void	MakeCollisionDesc(OUT COLLISION_DESC& descLeft, OUT COLLISION_DESC& descRight, CRigidBody* lRigid, CRigidBody* rRigid, const _float& fTimeDelta);
	void	MakeCollisionDescStatic(OUT COLLISION_DESC& descLeft, CRigidBody* lRigid, CRigidBody* rRigid, const _float& fTimeDelta);

private:
	struct MurmurHash3Hasher
	{
		size_t operator() (const uint64& key) const
		{
			uint64 hashResult[2];
			MurmurHash3_x64_128(&key, sizeof(key), 0, hashResult);
			size_t combinedHash = static_cast<size_t>(hashResult[0]) ^ static_cast<size_t>(hashResult[1]);
			return combinedHash;
		}
	};	// HashFunctionObject

	using CollisionMap = unordered_map<ULONGLONG, _bool, MurmurHash3Hasher>;
	CollisionMap m_hashColInfo;

	_uint	m_arrCheck[(_uint)LAYERTAG::LAYER_END] = { 0 };
	_bool	m_arrSorted[(_uint)LAYERTAG::LAYER_END] = { 0 };

	map<LAYERTAG, class CLayer*>* pCurrentLevelLayers;//

public:
	virtual void Free() override;
};

END