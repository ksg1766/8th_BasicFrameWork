#include "CollisionManager.h"
#include "LevelManager.h"
#include "ObjectManager.h"
#include "GameObject.h"
#include "Layer.h"
#include "RigidDynamic.h"
#include "ColliderSphere.h"
#include "QuadTree.h"
#include "QuadTreeNode.h"

IMPLEMENT_SINGLETON(CCollisionManager);

CCollisionManager::CCollisionManager()
{
}

void CCollisionManager::LateTick_Collision(const _float fTimeDelta)
{
	::ZeroMemory(m_arrSorted, (_uint)LAYERTAG::LAYER_END * sizeof(_bool));

	for (UINT iRow = 0; iRow < (UINT)LAYERTAG::LAYER_END; ++iRow)
	{
		for (UINT iCol = iRow; iCol < (UINT)LAYERTAG::LAYER_END; ++iCol)
		{
			if (m_arrCheck[iRow] & (1 << iCol))
			{
				CheckDynamicCollision((LAYERTAG)iRow, (LAYERTAG)iCol, fTimeDelta);
			}
		}
	}
}

void CCollisionManager::CheckGroup(LAYERTAG eLeft, LAYERTAG eRight)
{
	_uint iRow = (_uint)eLeft;
	_uint iCol = (_uint)eRight;

	if (iCol < iRow)
	{
		iRow = (_uint)eRight;
		iCol = (_uint)eLeft;
	}

	if (m_arrCheck[iRow] & (1 << iCol))
	{
		m_arrCheck[iRow] &= ~(1 << iCol);
	}
	else
	{
		m_arrCheck[iRow] |= (1 << iCol);
	}
}

void CCollisionManager::Reset()
{
	::ZeroMemory(m_arrCheck, sizeof(UINT) * (UINT)LAYERTAG::LAYER_END);
	pCurrentLevelLayers = CObjectManager::GetInstance()->GetCurrentLevelLayers();
}

bool CCollisionManager::IsCollided(CCollider* pLeft, CCollider* pRight)
{
	if (pLeft->Intersects(pRight))
	{
		if (pLeft->GetRigidBody()->GetBoxCollider()->Intersects(pRight->GetRigidBody()->GetBoxCollider()))
			return true;
	}
    return false;
}

void CCollisionManager::CheckDynamicCollision(LAYERTAG eLayerLeft, LAYERTAG eLayerRight , const _float fTimeDelta)
{
	vector<CGameObject*>& vecLeft = (*pCurrentLevelLayers)[eLayerLeft]->GetGameObjects();
	vector<CGameObject*>& vecRight = (*pCurrentLevelLayers)[eLayerRight]->GetGameObjects();
	
	if (!m_arrSorted[(_uint)eLayerRight])
	{
		::sort(vecRight.begin(), vecRight.end(), [&](CGameObject* pObjL, CGameObject* pObjR) ->_bool
			{
				BoundingSphere& tBoundingSphereL = pObjL->GetRigidBody()->GetSphereCollider()->GetBoundingSphere();
				BoundingSphere& tBoundingSphereR = pObjR->GetRigidBody()->GetSphereCollider()->GetBoundingSphere();
				return tBoundingSphereL.Center.z - tBoundingSphereL.Radius < tBoundingSphereR.Center.z - tBoundingSphereR.Radius;
			});

		m_arrSorted[(_uint)eLayerRight] = true;
	}

	unordered_map<ULONGLONG, bool>::iterator iter;
	//map<ULONGLONG, bool>::iterator iter;

	for (auto& iterL : vecLeft)
	{
		CRigidBody* pRigidBodyL = iterL->GetRigidBody();

		// �ϴ� DynamicRigid�� ������ SphereCollider�� ������ �ִٴ� ����.
		CSphereCollider* pLeftCol = pRigidBodyL->GetSphereCollider();
		if (nullptr == pLeftCol)
			continue;

		_float	fLeftMinZ = pLeftCol->GetBoundingSphere().Center.z - pLeftCol->GetBoundingSphere().Radius;
		_bool	bIgnoreRest = false;

		for (auto& iterR : vecRight)
		{
			CRigidBody* pRigidBodyR = iterR->GetRigidBody();

			CSphereCollider* pRightCol = pRigidBodyR->GetSphereCollider();
			if (nullptr == pRightCol || iterL == iterR)
				continue;

			if (!bIgnoreRest)
			{
				_float fRightMinZ = pRightCol->GetBoundingSphere().Center.z - pRightCol->GetBoundingSphere().Radius;

				if (false == CompareMinZ(fLeftMinZ, fRightMinZ)) // false�� ���� ���� or Exitȣ��
				{
					bIgnoreRest = true;
				}

				COLLIDER_ID ID;
				ID.Left_id = pLeftCol->GetID();
				ID.Right_id = pRightCol->GetID();

				iter = m_mapColInfo.find(ID.ID);

				if (m_mapColInfo.end() == iter)
				{
					m_mapColInfo.insert(make_pair(ID.ID, false));
					iter = m_mapColInfo.find(ID.ID);
				}

				if (IsCollided(pLeftCol, pRightCol))
				{	// ���� �浹 ��

					// TODO : bIgnoreRest �� true�� ���� ��� �ؾ� �ϴ°�. Exit�� �Ű������� desc�� �ʿ��Ѱ�...
					COLLISION_DESC lDesc, rDesc;
					MakeCollisionDesc(lDesc, rDesc, pRigidBodyL, pRigidBodyR, fTimeDelta);

					if (iter->second)
					{	// �������� �浹
						if (iterL->IsDead() || iterR->IsDead())
						{	// �� �� �ϳ� ���� �����̸� �浹 ����
							pRigidBodyL->OnCollisionExit(lDesc);
							pRigidBodyR->OnCollisionExit(rDesc);
							iter->second = false;
						}
						else
						{
							pRigidBodyL->OnCollisionStay(lDesc);
							pRigidBodyR->OnCollisionStay(rDesc);
						}
					}
					else
					{	// �������� �浹 x	// �ٵ� �� �� �ϳ� ���� �����̸� �浹���� ���� ������ ���
						if (!iterL->IsDead() && !iterR->IsDead())
						{
							pRigidBodyL->OnCollisionEnter(lDesc);
							pRigidBodyR->OnCollisionEnter(rDesc);
							iter->second = true;
						}
						else
						{
							pRigidBodyL->OnCollisionExit(lDesc);
							pRigidBodyR->OnCollisionExit(rDesc);
							iter->second = false;
						}
					}
				}
				else	// ���� �浹 x��
				{	
					if (iter->second)	//���� ������ �浹
					{	
						// TODO: �Ʒ� desc�� ���߿� �ʿ信 ���� ���ִ°� ����غ����� ����.
						COLLISION_DESC lDesc, rDesc;
						MakeCollisionDesc(lDesc, rDesc, pRigidBodyL, pRigidBodyR, fTimeDelta);
						//
						pRigidBodyL->OnCollisionExit(lDesc);
						pRigidBodyR->OnCollisionExit(rDesc);
						iter->second = false;
					}
				}
			}
			else	// Z�� ��� �����ص� �ȴٸ�
			{
				if (iter->second)	// �������� �浹�ϰ� �־��ٸ�
				{
					// TODO: �Ʒ� desc�� ���߿� �ʿ信 ���� ���ִ°� ����غ����� ����.
					COLLISION_DESC lDesc, rDesc;
					MakeCollisionDesc(lDesc, rDesc, pRigidBodyL, pRigidBodyR, fTimeDelta);
					//
					pRigidBodyL->OnCollisionExit(lDesc);
					pRigidBodyR->OnCollisionExit(rDesc);
					iter->second = false;
				}
			}
		}
	}
}

void CCollisionManager::CheckStaticCollision(LAYERTAG eDynamicLayer, const _float fTimeDelta)
{
	vector<CGameObject*>& vecLeft = (*pCurrentLevelLayers)[eDynamicLayer]->GetGameObjects();
	
	CQuadTree* pQuadTreeInstance = CQuadTree::GetInstance();
	
	unordered_map<ULONGLONG, bool>::iterator iter;
	//map<ULONGLONG, bool>::iterator iter;

	for (auto& iterL : vecLeft)
	{
		CRigidBody* pRigidBodyL = iterL->GetRigidBody();

		CSphereCollider* pLeftCol = pRigidBodyL->GetSphereCollider();
		if (nullptr == pLeftCol)
			continue;

		CQuadTreeNode* pCurrentNode = pQuadTreeInstance->GetCurrentNodeByPos(iterL->GetTransform()->GetPosition(), pQuadTreeInstance->GetCQuadTreeRoot());
		
		if (!pCurrentNode) return;

		vector<CGameObject*>& vecRight = pCurrentNode->GetObjectList();

		//const _bool	IsLeftTrigger = pLeftCol->IsTrigger();

		for (auto& iterR : vecRight)
		{
			CRigidBody* pRigidBodyR = iterR->GetRigidBody();

			CSphereCollider* pRightCol = pRigidBodyR->GetSphereCollider();
			if (nullptr == pRightCol || iterL == iterR)
				continue;

			COLLIDER_ID ID;
			ID.Left_id = pLeftCol->GetID();
			ID.Right_id = pRightCol->GetID();

			iter = m_mapColInfo.find(ID.ID);

			if (m_mapColInfo.end() == iter)
			{
				m_mapColInfo.insert(make_pair(ID.ID, false));
				iter = m_mapColInfo.find(ID.ID);
			}

			// TODO: �Ʒ� ó�� �ϵ��� TriggerLayer ���� �ΰ� �Լ��� Ʈ���� �浹�� ���� �����ϵ��� ��������.
			/*const _bool	IsRightTrigger = pRightCol->IsTrigger();

			if (!IsLeftTrigger && IsRightTrigger)
			{
				// ~~~
			}
			else if (IsLeftTrigger && !IsRightTrigger)
			{
				// ~~~
			}
			else if (IsLeftTrigger && IsRightTrigger)
			{
				continue;
			}*/

			if (IsCollided(pLeftCol, pRightCol))
			{	// ���� �浹 ��
				COLLISION_DESC lDesc;
				MakeCollisionDescStatic(lDesc, pRigidBodyL, pRigidBodyR, fTimeDelta);

				if (iter->second)
				{	// �������� �浹
					if (iterL->IsDead() || iterR->IsDead())
					{	// �� �� �ϳ� ���� �����̸� �浹 ����
						pRigidBodyL->OnCollisionExit(lDesc);
						//pRightCol->OnCollisionExit(pLeftCol);
						iter->second = false;
					}
					else
					{
						pRigidBodyL->OnCollisionStay(lDesc);
						//pRightCol->OnCollisionStay(pLeftCol);
					}
				}
				else
				{	// �������� �浹 x	// �ٵ� �� �� �ϳ� ���� �����̸� �浹���� ���� ������ ���
					if (!iterL->IsDead() && !iterR->IsDead())
					{
						pRigidBodyL->OnCollisionEnter(lDesc);
						//pRightCol->OnCollisionEnter(pLeftCol);
						iter->second = true;
					}
					else
					{
						pRigidBodyL->OnCollisionExit(lDesc);
						//pRightCol->OnCollisionExit(pLeftCol);
						iter->second = false;
					}
				}
			}
			else
			{		// ���� �浹 x��
				if (iter->second)
				{	//���� ������ �浹
					COLLISION_DESC lDesc;
					MakeCollisionDescStatic(lDesc, pRigidBodyL, pRigidBodyR, fTimeDelta);

					pRigidBodyL->OnCollisionExit(lDesc);
					//pRightCol->OnCollisionExit(pLeftCol);
					iter->second = false;
				}
			}
		}
	}
}

_bool CCollisionManager::CompareMinZ(_float& fLeftMinZ, _float& fRightMinZ)
{
	if (fLeftMinZ < fRightMinZ)
		return false;
	return true;
}

void CCollisionManager::MakeCollisionDesc(OUT COLLISION_DESC& descLeft, OUT COLLISION_DESC& descRight, CRigidBody* lRigid, CRigidBody* rRigid, const _float fTimeDelta)
{
	descLeft.pOther = rRigid;
	descLeft.fTimeDelta = fTimeDelta;

	descRight.pOther = lRigid;
	descRight.fTimeDelta = fTimeDelta;

	/*CRigidDynamic* pRigidL = dynamic_cast<CRigidDynamic*>(lObj->GetRigidBody());
	CRigidDynamic* pRigidR = dynamic_cast<CRigidDynamic*>(rObj->GetRigidBody());

	Vec3 vVelocityL = pRigidL->GetLinearVelocity();
	Vec3 vVelocityR = pRigidR->GetLinearVelocity();

	_float fMassRatio = pRigidL->GetMass() / pRigidR->GetMass();	// L/R

	descLeft.pOther = rObj;
	descLeft.vResultVelocity = (2.f * vVelocityR - (1.f - fMassRatio) * vVelocityL) / (1.f + fMassRatio);
	
	descRight.pOther = lObj;
	descRight.vResultVelocity = (2.f * vVelocityL - (1.f - 1.f / fMassRatio) * vVelocityR) / (1.f + 1.f / fMassRatio);*/
}

void CCollisionManager::MakeCollisionDescStatic(OUT COLLISION_DESC& descLeft, CRigidBody* lRigid, CRigidBody* rRigid, const _float fTimeDelta)
{
	descLeft.pOther = rRigid;
	descLeft.fTimeDelta = fTimeDelta;

	/*descLeft.pOther = rObj;
	descLeft.vResultVelocity = -dynamic_cast<CRigidDynamic*>(lObj->GetRigidBody())->GetLinearVelocity();*/
}

void CCollisionManager::Free()
{
	__super::Free();
}
