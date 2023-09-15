#include "Engine_Defines.h"
#include "ColliderBase.h"
#include "GameObject.h"
#include "TransformEx.h"
#include "RigidDynamic.h"
#include "RigidStatic.h"

_uint CCollider::g_iNextID = 0;

CCollider::CCollider(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, ColliderType eColliderType)
	: Super(pDevice, pContext, ComponentType::Collider)
	, m_iID(g_iNextID++)
	, m_eColliderType(eColliderType)
	, m_IsTrigger(false)
{

}

CCollider::CCollider(const CCollider& rhs)
	: Super(rhs)
	, m_iID(g_iNextID++)
	, m_eColliderType(rhs.m_eColliderType)
	, m_IsTrigger(rhs.m_IsTrigger)
{

}

void CCollider::Free()
{
	Super::Free();
}
