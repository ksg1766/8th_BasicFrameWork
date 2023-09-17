#include "Transform.h"
#include "Shader.h"

CTransform::CTransform(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:Super(pDevice, pContext, ComponentType::Transform)
{
}

CTransform::CTransform(const CTransform& rhs)
	:Super(rhs)
{
}

HRESULT CTransform::Initialize_Prototype()
{
	/*::ZeroMemory(m_vInfo, sizeof(m_vInfo));
	m_vInfo[INFO_RIGHT].x = 1.f;
	m_vInfo[INFO_UP].y = 1.f;
	m_vInfo[INFO_LOOK].z = 1.f;*/

	return S_OK;
}

HRESULT CTransform::Initialize(void* pArg)
{
	return S_OK;
}

void CTransform::Scale(Vec3& vScale)
{
	/*_vec3 vScale;
	vScale.x = D3DXVec3Length(&m_vInfo[INFO_RIGHT]);
	vScale.y = D3DXVec3Length(&m_vInfo[INFO_UP]);
	vScale.z = D3DXVec3Length(&m_vInfo[INFO_LOOK]);*/

	/*for (_int i = 0; i < INFO_POS; ++i)
	{
		D3DXVec3Normalize(&m_vInfo[i], &m_vInfo[i]);
		//m_vInfo[i] *= (*(((_float*)&vScale) + i) * *(((_float*)&_vScale) + i));
		m_vInfo[i] *= *(((_float*)&_vScale) + i);
	}
	if (!m_pChild.empty())
	{
		//m_pChild->Scale(_vScale); 
		//for (auto& iter : m_pChild)
			//iter->Scale(_vScale);
	}*/
}

void CTransform::Scale(const Vec3& vScale)
{
	Scale(const_cast<Vec3&>(vScale));
}

void CTransform::Rotate(Vec3& vEulers)
{
	/*Matrix matRotate;
	Quaternion quat;
	Vec3 vRotAxis;

	if (0.f != _vEulers.y)
		D3DXQuaternionRotationAxis(&quat, &Vec3(0.f, 1.f, 0.f), vEulers.y);
	else if (0.f != _vEulers.x)
		D3DXQuaternionRotationAxis(&quat, &m_vInfo[INFO_RIGHT], vEulers.x);
	else if (0.f != _vEulers.z)
		D3DXQuaternionRotationAxis(&quat, &m_vInfo[INFO_LOOK], vEulers.z);
	else
		return;

	D3DXMatrixRotationQuaternion(&matRotate, &quat);

	for (int i = 0; i < INFO_POS; ++i)
		D3DXVec3TransformNormal(&m_vInfo[i], &m_vInfo[i], &matRotate);

	if (!m_pChild.empty())
	{
		if (0.f != _vEulers.y)
			for (auto& iter : m_pChild)
				iter->RotateAround(m_vInfo[INFO_POS], Vec3(0.f, 1.f, 0.f), vEulers.y);
		if (0.f != _vEulers.x)
			for (auto& iter : m_pChild)
				iter->RotateAround(m_vInfo[INFO_POS], m_vInfo[INFO_RIGHT], vEulers.x);
		if (0.f != _vEulers.z)
			for (auto& iter : m_pChild)
				iter->RotateAround(m_vInfo[INFO_POS], m_vInfo[INFO_LOOK], vEulers.z);
	}*/
}

void CTransform::Rotate(const Vec3& vEulers)
{
	Rotate(const_cast<Vec3&>(vEulers));
}

void CTransform::Rotate(const _float& fXangle, const _float& fYangle, const _float& fZangle)
{
	Rotate(Vec3(fXangle, fYangle, fZangle));
}

void CTransform::RotateAround(const Vec3& vPoint, const Vec3& vAxis, const _float& fAngle)
{
	/*Quaternion quat;
	Matrix matRotate;
	Vec3 vOut;

	// D3DXMatrixRotationAxis(&matRotate, &vAxis, fAngle);
	D3DXQuaternionRotationAxis(&quat, &vAxis, fAngle);
	D3DXMatrixRotationQuaternion(&matRotate, &quat);
	D3DXVec3TransformCoord(&vOut, &(m_vInfo[INFO_POS] - vPoint), &matRotate);
	for (int i = 0; i < INFO_POS; ++i)
		D3DXVec3TransformNormal(&m_vInfo[i], &m_vInfo[i], &matRotate);

	m_vInfo[INFO_POS] = vOut + vPoint;*/
}

void CTransform::Translate(Vec3& vTranslation)
{
	/*m_vInfo[INFO_POS] += vTranslation;
	if (!m_pChild.empty())
	{
		for (auto& iter : m_pChild)
			iter->Translate(vTranslation);
	}*/
}

void CTransform::Translate(const Vec3& vTranslation)
{
	Translate(const_cast<Vec3&>(vTranslation));
}

const _matrix CTransform::WorldMatrix()
{
	_matrix matWorld;
	/*D3DXMatrixIdentity(&matWorld);

	for (int i = 0; i < INFO_END; ++i)
		::CopyMemory(&matWorld.m[i], m_vInfo[i], sizeof(Vec3));*/

	return matWorld;
}

void CTransform::Set_WorldMatrix(_matrix& matWorld)
{
	/*for (_int i = 0; i < INFO_END; ++i)
		::CopyMemory(&m_vInfo[i], &matWorld.m[i][0], sizeof(Vec3));*/
}

Vec3 CTransform::LocalScale()
{
	Vec3 vScale;
	/*for (_int i = 0; i < INFO_POS; ++i)
		*(((_float*)&vScale) + i) = D3DXVec3Length(&m_vInfo[i]);*/

	return vScale;
}

void CTransform::Tick(const _float& fTimeDelta)
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
	Super::Free();
}