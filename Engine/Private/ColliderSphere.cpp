#include "Engine_Defines.h"
#include "GameObject.h"
#include "Transform.h"
#include "ColliderSphere.h"
#include "ColliderAABB.h"
#include "ColliderOBB.h"
#include "ColliderCylinder.h"
#include "RigidDynamic.h"

CSphereCollider::CSphereCollider(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:Super(pDevice, pContext, ColliderType::Sphere)
{
}

CSphereCollider::CSphereCollider(const CSphereCollider& rhs)
	:Super(rhs)
	, m_fRadius(rhs.m_fRadius)
	, m_tBoundingSphere(rhs.m_tBoundingSphere)
{

}

HRESULT CSphereCollider::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CSphereCollider::Initialize(void* pArg)
{
	return S_OK;
}

void CSphereCollider::Tick(const _float& fTimeDelta)
{
	m_tBoundingSphere.Center = GetGameObject()->GetTransform()->GetPosition();

	Vec3 scale = GetGameObject()->GetTransform()->GetLocalScale();
	m_tBoundingSphere.Radius = m_fRadius * max(max(scale.x, scale.y), scale.z);
}

void CSphereCollider::LateTick(const _float& fTimeDelta)
{
}

void CSphereCollider::DebugRender()
{
}

_bool CSphereCollider::Intersects(Ray& ray, OUT _float& distance)
{
	return m_tBoundingSphere.Intersects(ray.position, ray.direction, OUT distance);
}

_bool CSphereCollider::Intersects(Super*& other)
{
	ColliderType type = other->GetColliderType();

	switch (type)
	{
	case ColliderType::Sphere:
		return m_tBoundingSphere.Intersects(dynamic_cast<CSphereCollider*>(other)->GetBoundingSphere());
	case ColliderType::AABB:
		return m_tBoundingSphere.Intersects(dynamic_cast<CAABBCollider*>(other)->GetBoundingBox());
	case ColliderType::OBB:
		return m_tBoundingSphere.Intersects(dynamic_cast<COBBCollider*>(other)->GetBoundingBox());
	//case ColliderType::Cylinder:
	//	return m_tBoundingSphere.Intersects(dynamic_cast<CCylinderCollider*>(other)->GetBoundingCylinder());
	}

	return false;
}

CSphereCollider* CSphereCollider::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CSphereCollider* pInstance = new CSphereCollider(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CSphereCollider");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CComponent* CSphereCollider::Clone(void* pArg)
{
	CSphereCollider* pInstance = new CSphereCollider(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed To Cloned : CSphereCollider");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CSphereCollider::Free()
{
	Super::Free();
}

void CSphereCollider::OnTriggerEnter(const CCollider* pOther)
{
	m_pGameObject->OnTriggerEnter(pOther->GetGameObject());
}

void CSphereCollider::OnTriggerStay(const CCollider* pOther)
{
	m_pGameObject->OnTriggerStay(pOther->GetGameObject());
}

void CSphereCollider::OnTriggerExit(const CCollider* pOther)
{
	m_pGameObject->OnTriggerExit(pOther->GetGameObject());
}
