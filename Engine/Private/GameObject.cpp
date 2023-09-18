#include "..\Public\GameObject.h"
#include "GameInstance.h"

CGameObject::CGameObject(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: m_pDevice(pDevice)
	, m_pContext(pContext)
	, m_IsDead(false)
	, m_eLayerTag(LAYERTAG::LAYER_END)
{
	Safe_AddRef(m_pDevice);
	Safe_AddRef(m_pContext);
}

CGameObject::CGameObject(const CGameObject & rhs)
	: m_pDevice(rhs.m_pDevice)
	, m_pContext(rhs.m_pContext)
	, m_IsDead(rhs.m_IsDead)
	, m_eLayerTag(rhs.m_eLayerTag)
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

void CGameObject::Tick(const _float& fTimeDelta)
{
}

void CGameObject::LateTick(const _float& fTimeDelta)
{
}

void CGameObject::DebugRender()
{
}

HRESULT CGameObject::Render()
{
	return S_OK;
}

CTransform* CGameObject::GetTransform() const
{
	CComponent* pComponent = GetFixedComponent(ComponentType::Transform);
	return static_cast<CTransform*>(pComponent);
}

CRigidBody* CGameObject::GetRigidBody() const
{
	CComponent* pComponent = GetFixedComponent(ComponentType::RigidBody);
	return static_cast<CRigidBody*>(pComponent);
}

CVIBuffer* const CGameObject::GetBuffer() const
{
	CComponent* pComponent = GetFixedComponent(ComponentType::Buffer);
	return static_cast<CVIBuffer*>(pComponent);
}

CShader* CGameObject::GetShader() const
{
	CComponent* pComponent = GetFixedComponent(ComponentType::Shader);
	return static_cast<CShader*>(pComponent);
}

CRenderer* CGameObject::GetRenderer() const
{
	CComponent* pComponent = GetFixedComponent(ComponentType::Renderer);
	return static_cast<CRenderer*>(pComponent);
}

CTexture* CGameObject::GetTexture() const
{
	CComponent* pComponent = GetFixedComponent(ComponentType::Texture);
	return static_cast<CTexture*>(pComponent);
}

CTransform* CGameObject::GetOrAddTransform(_uint iLevelIndex)
{
	if (GetTransform() == nullptr)
	{
		AddComponent(iLevelIndex, ComponentType::Transform, TEXT("Prototype_Component_Transform"));
	}

	return GetTransform();
}

CComponent* CGameObject::GetFixedComponent(const ComponentType& type) const
{
	uint8 index = static_cast<uint8>(type);
	assert(index < FIXED_COMPONENT_COUNT);
	return m_arrComponents[index];
}

HRESULT CGameObject::AddComponent(_uint iLevelIndex, ComponentType type, const wstring& strPrototypeTag, void* pArg)
{
	if (nullptr != GetFixedComponent(type))
		return E_FAIL;

	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);

	/* 원형 컴포넌트를 복제하여 사본 컴포넌트를 얻어오자. */
	CComponent* pComponent = pGameInstance->Clone_Component(iLevelIndex, strPrototypeTag, pArg);
	if (nullptr == pComponent)
		return E_FAIL;

	pComponent->SetGameObject(this);

	uint8 index = static_cast<uint8>(static_cast<CComponent*>(pComponent)->GetType());
	if (index < FIXED_COMPONENT_COUNT)
	{
		m_arrComponents[index] = pComponent;
	}
	else
	{
		m_vecScripts.push_back(dynamic_cast<CMonoBehaviour*>(pComponent));
	}

	Safe_AddRef(pComponent);

	Safe_Release(pGameInstance);

	return S_OK;
}

void CGameObject::Free()
{
	Super::Free();

	for (auto& iter : m_arrComponents)
		Safe_Release(iter);

	for (auto& iter : m_vecScripts)
		Safe_Release(iter);
	
	m_vecScripts.clear();

	Safe_Release(m_pDevice);
	Safe_Release(m_pContext);
}
