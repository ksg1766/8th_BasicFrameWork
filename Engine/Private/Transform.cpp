#include "Transform.h"

CTransform::CTransform(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: Super(pDevice, pContext, ComponentType::Transform)
{
}

CTransform::CTransform(const CTransform& rhs)
	: Super(rhs)
	, m_vLocalScale(rhs.m_vLocalScale)
	, m_vLocalRotation(rhs.m_vLocalRotation)
	, m_vLocalPosition(rhs.m_vLocalPosition)
	, m_matLocal(rhs.m_matLocal)
	, m_matWorld(rhs.m_matLocal)
	, m_vScale(rhs.m_vScale)
	, m_vRotation(rhs.m_vRotation)
	, m_vPosition(rhs.m_vPosition)
{
}

HRESULT CTransform::Initialize_Prototype()
{
	m_vLocalScale = { 1.f, 1.f, 1.f };
	m_vLocalRotation = { 0.f, 0.f, 0.f };
	m_vLocalPosition = { 0.f, 0.f, 0.f };
	m_matLocal = Matrix::Identity;
	m_matWorld = Matrix::Identity;

	return S_OK;
}

HRESULT CTransform::Initialize(void* pArg)
{
	//if (nullptr != pArg)
	//
	
	return S_OK;
}

void CTransform::Tick(_float fTimeDelta)
{
}

void CTransform::LateTick(_float fTimeDelta)
{
}

Vec3 CTransform::ToEulerAngles(Quaternion q)
{
	Vec3 angles;

	// roll (x-axis rotation)
	_float sinr_cosp = 2.f * (q.w * q.x + q.y * q.z);
	_float cosr_cosp = 1.f - 2.f * (q.x * q.x + q.y * q.y);
	angles.x = std::atan2(sinr_cosp, cosr_cosp);

	// pitch (y-axis rotation)
	_float sinp = std::sqrt(1.f + 2.f * (q.w * q.y - q.x * q.z));
	_float cosp = std::sqrt(1.f - 2.f * (q.w * q.y - q.x * q.z));
	angles.y = 2.f * std::atan2(sinp, cosp) - 3.14159265f / 2.f;

	// yaw (z-axis rotation)
	_float siny_cosp = 2.f * (q.w * q.z + q.x * q.y);
	_float cosy_cosp = 1.f - 2.f * (q.y * q.y + q.z * q.z);
	angles.z = std::atan2(siny_cosp, cosy_cosp);

	return angles;
}

void CTransform::UpdateTransform()
{
	Matrix matScale = Matrix::CreateScale(m_vLocalScale);
	Matrix matRotation = Matrix::CreateRotationX(m_vLocalRotation.x);
	matRotation *= Matrix::CreateRotationY(m_vLocalRotation.y);
	matRotation *= Matrix::CreateRotationZ(m_vLocalRotation.z);
	Matrix matTranslation = Matrix::CreateTranslation(m_vLocalPosition);

	m_matLocal = matScale * matRotation * matTranslation;

	if (HasParent())
	{
		m_matWorld = m_matLocal * m_pParent->GetWorldMatrix();
	}
	else
	{
		m_matWorld = m_matLocal;
	}

	Quaternion quat;
	m_matWorld.Decompose(m_vScale, quat, m_vPosition);
	m_vRotation = ToEulerAngles(quat);

	// Children
	for (CTransform*& child : m_vecChildren)
		child->UpdateTransform();
}

void CTransform::SetScale(const Vec3& worldScale)
{
	if (HasParent())
	{
		Vec3 parentScale = m_pParent->GetScale();
		Vec3 scale = worldScale;
		scale.x /= parentScale.x;
		scale.y /= parentScale.y;
		scale.z /= parentScale.z;
		SetLocalScale(scale);
	}
	else
	{
		SetLocalScale(worldScale);
	}
}

void CTransform::SetRotation(const Vec3& worldRotation)
{
	if (HasParent())
	{
		Matrix inverseMatrix = m_pParent->GetWorldMatrix().Invert();

		Vec3 rotation;
		rotation.TransformNormal(worldRotation, inverseMatrix);

		SetLocalRotation(rotation);
	}
	else
		SetLocalRotation(worldRotation);
}

void CTransform::SetPosition(const Vec3& worldPosition)
{
	if (HasParent())
	{
		Matrix worldToParentLocalMatrix = m_pParent->GetWorldMatrix().Invert();

		Vec3 position;
		position.Transform(worldPosition, worldToParentLocalMatrix);

		SetLocalPosition(position);
	}
	else
	{
		SetLocalPosition(worldPosition);
	}
}

void CTransform::DebugRender()
{
}

CTransform* CTransform::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CTransform* pInstance = new CTransform(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CTransform");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CComponent* CTransform::Clone(void* pArg)
{
	CTransform* pInstance = new CTransform(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed To Cloned : CTransform");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CTransform::Free()
{
	__super::Free();
}
