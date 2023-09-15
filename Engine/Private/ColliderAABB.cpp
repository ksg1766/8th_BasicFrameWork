#include "Engine_Defines.h"
#include "ColliderSphere.h"
#include "ColliderAABB.h"
#include "ColliderOBB.h"
#include "ColliderCylinder.h"

CAABBCollider::CAABBCollider(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:Super(pDevice, pContext, ColliderType::AABB)
{
}

CAABBCollider::CAABBCollider(const CAABBCollider& rhs)
	:Super(rhs)
	, m_tBoundingBox(rhs.m_tBoundingBox)
{
}

HRESULT CAABBCollider::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CAABBCollider::Initialize(void* pArg)
{
	return S_OK;
}

void CAABBCollider::Tick(_float fTimeDelta)
{
}

void CAABBCollider::LateTick(_float fTimeDelta)
{
}

void CAABBCollider::DebugRender()
{
}

_bool CAABBCollider::Intersects(Ray& ray, OUT _float& distance)
{
	return m_tBoundingBox.Intersects(ray.position, ray.direction, OUT distance);
}

_bool CAABBCollider::Intersects(Super*& other)
{
	ColliderType type = other->GetColliderType();

	switch (type)
	{
	case ColliderType::Sphere:
		return m_tBoundingBox.Intersects(dynamic_cast<CSphereCollider*>(other)->GetBoundingSphere());
	case ColliderType::AABB:
		return m_tBoundingBox.Intersects(dynamic_cast<CAABBCollider*>(other)->GetBoundingBox());
	case ColliderType::OBB:
		return m_tBoundingBox.Intersects(dynamic_cast<COBBCollider*>(other)->GetBoundingBox());
		//case ColliderType::Cylinder:
	//	return m_tBoundingBox.Intersects(dynamic_cast<CCylinderCollider*>(other)->GetBoundingCylinder());
	}

	return false;
}

CAABBCollider* CAABBCollider::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CAABBCollider* pInstance = new CAABBCollider(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CAABBCollider");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CComponent* CAABBCollider::Clone(void* pArg)
{
	CAABBCollider* pInstance = new CAABBCollider(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed To Cloned : CAABBCollider");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CAABBCollider::Free()
{
	Super::Free();
}
