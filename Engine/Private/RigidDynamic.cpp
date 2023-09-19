#include "RigidDynamic.h"
#include "Transform.h"
#include "GameObject.h"
#include "ColliderSphere.h"
#include "ColliderOBB.h"

CRigidDynamic::CRigidDynamic(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: Super(pDevice, pContext, RigidBodyType::DYNAMIC)
	, m_IsSleeping(true)
	, m_fSleepThreshold(0.05f)
	, m_UseGravity(true)
	, m_IsKinematic(false)
	, m_fMass(1.f)
	, m_fDrag(0.01f)
	, m_fAngularDrag(0.01f)
	, m_byConstraints(0)
	, m_vLinearAcceleration(Vec3::Zero)
	, m_vAngularAcceleration(Vec3::Zero)
	, m_vLinearVelocity(Vec3::Zero)
	, m_vAngularVelocity(Vec3::Zero)
{
}

CRigidDynamic::CRigidDynamic(const CRigidDynamic& rhs)
	: Super(rhs)
	, m_IsSleeping(rhs.m_IsSleeping)
	, m_fSleepThreshold(rhs.m_fSleepThreshold)
	, m_UseGravity(rhs.m_UseGravity)
	, m_IsKinematic(rhs.m_IsKinematic)
	, m_fMass(rhs.m_fMass)
	, m_fDrag(rhs.m_fDrag)
	, m_fAngularDrag(rhs.m_fAngularDrag)
	, m_byConstraints(rhs.m_byConstraints)
	, m_vLinearAcceleration(rhs.m_vLinearAcceleration)
	, m_vAngularAcceleration(rhs.m_vAngularAcceleration)
	, m_vLinearVelocity(rhs.m_vLinearVelocity)
	, m_vAngularVelocity(rhs.m_vAngularVelocity)
{
}

HRESULT CRigidDynamic::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CRigidDynamic::Initialize(void* pArg)
{
	return S_OK;
}

void CRigidDynamic::Tick(const _float& fTimeDelta)	// FixedUpdate ó�� �����ϱ� ���� RigidBody�� ������Ʈ�� ���� �켱 ȣ���ؾ� ��.
{
	if (m_IsSleeping)
		return;

	if (m_fSleepThreshold > m_vLinearVelocity.Length() && m_fSleepThreshold > 180.f * XM_1DIVPI * m_vAngularVelocity.Length())
	{
		Sleep();
		return;
	}

	if (m_IsKinematic)	// Kinematic
		KinematicUpdate(fTimeDelta);
	else				// Kinetic
		KineticUpdate(fTimeDelta);

	// ColliderUpdate
	if (m_pSphereCollider)	m_pSphereCollider->Tick(fTimeDelta);
	if (m_pBoxCollider)	m_pBoxCollider->Tick(fTimeDelta);
}

void CRigidDynamic::LateTick(const _float& fTimeDelta)
{
	// Collision

}

void CRigidDynamic::DebugRender()
{
}

void CRigidDynamic::KineticUpdate(const _float& fTimeDelta)
{
	if (m_UseGravity)
		m_vLinearVelocity.y += -9.81f * fTimeDelta;

	m_vAngularVelocity += m_vAngularAcceleration * fTimeDelta;
	m_vLinearVelocity += m_vLinearAcceleration * fTimeDelta;

	_float fAngularResistance = m_fAngularDrag;
	_float fLinearResistance = m_fDrag;

	(fAngularResistance < 1.f) ? (m_vAngularVelocity = m_vAngularVelocity * (1.f - fAngularResistance)) : (m_vAngularVelocity = Vec3::Zero);
	(fLinearResistance < 1.f) ? (m_vLinearVelocity = m_vLinearVelocity * (1.f - fLinearResistance)) : (m_vLinearVelocity = Vec3::Zero);

	// Constraints Check
	if (m_byConstraints)
	{
		_int i = 0;
		while(i < 3)
			(m_byConstraints & 1 << i) ? (*(((_float*)&m_vLinearVelocity) + i++) = 0) : i++;
		while(i < 6)
			(m_byConstraints & 1 << i) ? (*(((_float*)&m_vAngularVelocity) + i++) = 0) : i++;
	}

	UpdateTransform(fTimeDelta);
}

void CRigidDynamic::KinematicUpdate(const _float& fTimeDelta)
{
	UpdateTransform(fTimeDelta);

	//ClearNetPower();
	ClearForce(ForceMode::VELOCITY_CHANGE);
	ClearTorque(ForceMode::VELOCITY_CHANGE);
}

void CRigidDynamic::UpdateTransform(const _float& fTimeDelta)
{
	CTransform* pTransform = GetTransform();

	pTransform->Rotate(m_vAngularVelocity * fTimeDelta);
	pTransform->Translate(m_vLinearVelocity * fTimeDelta);
}

void CRigidDynamic::AddForce(const Vec3& vForce, ForceMode eMode)
{
	switch (eMode)
	{
	case ForceMode::FORCE:
		m_vLinearAcceleration += vForce / m_fMass;
		break;
	case ForceMode::IMPULSE:
		m_vLinearVelocity += vForce / m_fMass;
		break;
	case ForceMode::ACCELERATION:
		m_vLinearAcceleration += vForce;
		break;
	case ForceMode::VELOCITY_CHANGE:
		m_vLinearVelocity += vForce;
		break;
	}

	WakeUp();
}

void CRigidDynamic::AddTorque(const Vec3& vTorque, ForceMode eMode)
{
	switch (eMode)
	{
	case ForceMode::FORCE:								// ���� : r == 1	 �̸� T == F
		m_vAngularAcceleration += vTorque / m_fMass;	// (�� ����)���� ���Ʈ I == m * pow(r, 2) // �����ӵ� a == T / I
		break;
	case ForceMode::IMPULSE:
		m_vAngularVelocity += vTorque / m_fMass;		// ����� L == I * w	// ������ I == m // ���� ���ӵ� w == L / m
		break;
	case ForceMode::ACCELERATION:
		m_vAngularAcceleration += vTorque;
		break;
	case ForceMode::VELOCITY_CHANGE:
		m_vAngularVelocity += vTorque;
		break;
	}

	WakeUp();
}

void CRigidDynamic::SetForceAndTorque(const Vec3& vForce, const Vec3& vTorque, ForceMode eMode)
{
	switch (eMode)
	{
	case ForceMode::FORCE:
		m_vLinearAcceleration = vForce / m_fMass;
		m_vAngularAcceleration = vTorque / m_fMass;
		break;
	case ForceMode::IMPULSE:
		m_vLinearVelocity = vForce / m_fMass;
		m_vAngularVelocity = vTorque / m_fMass;
		break;
	case ForceMode::ACCELERATION:
		m_vLinearAcceleration = vForce;
		m_vAngularAcceleration = vTorque;
		break;
	case ForceMode::VELOCITY_CHANGE:
		m_vLinearVelocity = vForce;
		m_vAngularVelocity = vTorque;
		break;
	}

	WakeUp();
}

void CRigidDynamic::ClearForce(ForceMode eMode)
{
	switch (eMode)
	{
	case ForceMode::FORCE:
		::ZeroMemory(&m_vLinearAcceleration, sizeof(Vec3));
		break;
	case ForceMode::IMPULSE:
		::ZeroMemory(&m_vLinearVelocity, sizeof(Vec3));
		break;
	case ForceMode::ACCELERATION:
		::ZeroMemory(&m_vLinearAcceleration, sizeof(Vec3));
		break;
	case ForceMode::VELOCITY_CHANGE:
		::ZeroMemory(&m_vLinearVelocity, sizeof(Vec3));
		break;
	}
}

void CRigidDynamic::ClearTorque(ForceMode eMode)
{
	switch (eMode)
	{
	case ForceMode::FORCE:										// ���� : r == 1	 �̸� T == F
		::ZeroMemory(&m_vAngularAcceleration, sizeof(Vec3));	// (�� ����)���� ���Ʈ I == m * pow(r, 2) // �����ӵ� a == T / I
		break;
	case ForceMode::IMPULSE:
		::ZeroMemory(&m_vAngularVelocity, sizeof(Vec3));		// ����� L == I * w	// ������ I == m // ���� ���ӵ� w == L / m
		break;
	case ForceMode::ACCELERATION:
		::ZeroMemory(&m_vAngularAcceleration, sizeof(Vec3));
		break;
	case ForceMode::VELOCITY_CHANGE:
		::ZeroMemory(&m_vAngularVelocity, sizeof(Vec3));
		break;
	}
}

void CRigidDynamic::ClearNetPower()
{
	::ZeroMemory(&m_vLinearAcceleration, sizeof(Vec3));
	::ZeroMemory(&m_vAngularAcceleration, sizeof(Vec3));
	::ZeroMemory(&m_vAngularVelocity, sizeof(Vec3));
	::ZeroMemory(&m_vLinearVelocity, sizeof(Vec3));
}

void CRigidDynamic::OnCollisionEnter(const COLLISION_DESC& desc)
{
	if (!m_IsKinematic)	// Kinetic
	{
		const CRigidDynamic* pRigidR = dynamic_cast<const CRigidDynamic*>(desc.pOther);

		UpdateTransform(-desc.fTimeDelta);	// fTimeDelta�� Tick���� �����͵� �Ǳ� ��
		
		_float fMassRatio = m_fMass / pRigidR->GetMass();
		m_vLinearVelocity = (2.f * pRigidR->GetLinearVelocity() - (1.f - fMassRatio) * m_vLinearVelocity) / (1.f + fMassRatio);
	}
	else	// Kinematic
	{
		UpdateTransform(-desc.fTimeDelta);
	}

	m_pGameObject->OnCollisionEnter(desc.pOther->GetGameObject());
}

void CRigidDynamic::OnCollisionStay(const COLLISION_DESC& desc)
{
	if (!m_IsKinematic)	// Kinetic
	{
		const CRigidDynamic* pRigidR = dynamic_cast<const CRigidDynamic*>(desc.pOther);

		UpdateTransform(-desc.fTimeDelta);	// fTimeDelta�� Tick���� �����͵� �Ǳ� ��

		_float fMassRatio = m_fMass / pRigidR->GetMass();
		m_vLinearVelocity = (2.f * pRigidR->GetLinearVelocity() - (1.f - fMassRatio) * m_vLinearVelocity) / (1.f + fMassRatio);
	}
	else	// Kinematic
	{
		UpdateTransform(-desc.fTimeDelta);
	}

	m_pGameObject->OnCollisionStay(desc.pOther->GetGameObject());
}

void CRigidDynamic::OnCollisionExit(const COLLISION_DESC& desc)
{
}

void CRigidDynamic::SetLinearVelocity(const Vec3& vLinearVelocity)
{
	m_vLinearVelocity = vLinearVelocity;

	WakeUp();
}

void CRigidDynamic::SetAngularVelocity(const Vec3& vAngularVelocity)
{
	m_vAngularVelocity = vAngularVelocity;

	WakeUp();
}

CRigidDynamic* CRigidDynamic::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CRigidDynamic* pInstance = new CRigidDynamic(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CRigidDynamic");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CComponent* CRigidDynamic::Clone(CGameObject* pGameObject, void* pArg)
{
	CRigidDynamic* pInstance = new CRigidDynamic(*this);
	pInstance->m_pGameObject = pGameObject;

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed To Cloned : CRigidDynamic");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CRigidDynamic::Free()
{
	Super::Free();
}