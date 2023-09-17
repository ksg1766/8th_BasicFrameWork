#include "RigidBodyBase.h"

CRigidBody::CRigidBody(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, RigidBodyType eRigidBodyType)
	: Super(pDevice, pContext, ComponentType::RigidBody)
	, m_eRigidBodyType(eRigidBodyType)
{
}

CRigidBody::CRigidBody(const CRigidBody& rhs)
	: Super(rhs)
	, m_eRigidBodyType(rhs.m_eRigidBodyType)
{
}

void CRigidBody::Free()
{
	Super::Free();
}

HRESULT CRigidBody::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CRigidBody::Initialize(void* pArg)
{
	return S_OK;
}

void CRigidBody::Tick(const _float& fTimeDelta)
{
}

void CRigidBody::LateTick(const _float& fTimeDelta)
{
}

void CRigidBody::DebugRender()
{
}
