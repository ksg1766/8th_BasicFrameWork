#include "..\Public\GameObject.h"
#include "Transform.h"
#include "MonoBehaviour.h"
//#include "Camera.h"
//#include "MeshRenderer.h"
//#include "ModelRenderer.h"
//#include "ModelAnimator.h"
//#include "Light.h"
//#include "BaseCollider.h"
//#include "Terrain.h"
//#include "Button.h"

CGameObject::CGameObject(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: m_pDevice(pDevice)
	, m_pContext(pContext)
{
	Safe_AddRef(m_pDevice);
	Safe_AddRef(m_pContext);
}

CGameObject::CGameObject(const CGameObject & rhs)
	: m_pDevice(rhs.m_pDevice)
	, m_pContext(rhs.m_pContext)
{
	Safe_AddRef(m_pDevice);
	Safe_AddRef(m_pContext);
}

HRESULT CGameObject::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CGameObject::Initialize(void* pArg)
{
	return S_OK;
}

void CGameObject::Tick(_float fTimeDelta)
{
}

void CGameObject::LateTick(_float fTimeDelta)
{
}

HRESULT CGameObject::Render()
{
	return S_OK;
}

CComponent* CGameObject::GetFixedComponent(ComponentType type)
{
	return nullptr;
}

CTransform* CGameObject::GetTransform()
{
	CComponent* pComponent = GetFixedComponent(ComponentType::Transform);
	return static_cast<CTransform*>(pComponent);
}

CTransform* CGameObject::GetOrAddTransform()
{
	if (GetTransform() == nullptr)
	{
		//
		CComponent* pTransform = new CTransform(m_pDevice, m_pContext);
		AddComponent(pTransform);
	}

	return GetTransform();
}

void CGameObject::AddComponent(CComponent* component)
{
	component->SetGameObject(this);

	uint8 index = static_cast<uint8>(component->GetType());
	if (index < FIXED_COMPONENT_COUNT)
	{
		m_arrComponents[index] = component;
	}
	else
	{
		m_vecScripts.push_back(dynamic_cast<CMonoBehaviour*>(component));
	}
}

void CGameObject::Free()
{
	__super::Free();

	Safe_Release(m_pDevice);
	Safe_Release(m_pContext);
}
