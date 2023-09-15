#pragma once
#include "Base.h"

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
	void LateTick_Collision(const _float fTimeDelta);

public:
	//map<ULONGLONG, _bool>& GetMapColInfo() { return m_mapColInfo; }
	//void SetMapColInfo(ULONGLONG ID, _bool bCollision) { m_mapColInfo[ID] = bCollision; }
	
private:
	void	CheckGroup(LAYERTAG eLeft, LAYERTAG eRight);
	void	Reset();
	
	_bool	IsCollided(CCollider* pLeft, CCollider* pRight);

	void	CheckDynamicCollision(LAYERTAG eObjectLeft, LAYERTAG eObjectRight, const _float fTimeDelta);
	void	CheckStaticCollision(LAYERTAG eDynamicLayer, const _float fTimeDelta);
	_bool	CompareMinZ(_float& fLeftMinZ, _float& fRightMinZ);

	void	MakeCollisionDesc(OUT COLLISION_DESC& descLeft, OUT COLLISION_DESC& descRight, CRigidBody* lRigid, CRigidBody* rRigid, const _float fTimeDelta);
	void	MakeCollisionDescStatic(OUT COLLISION_DESC& descLeft, CRigidBody* lRigid, CRigidBody* rRigid, const _float fTimeDelta);

private:
	//map<ULONGLONG, _bool> m_mapColInfo;
	unordered_map<ULONGLONG, _bool> m_mapColInfo;
	_uint	m_arrCheck[(_uint)LAYERTAG::LAYER_END];
	_bool	m_arrSorted[(_uint)LAYERTAG::LAYER_END];

	map<LAYERTAG, class CLayer*>* pCurrentLevelLayers;//

public:
	virtual void Free() override;
};

END