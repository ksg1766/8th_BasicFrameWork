#pragma once

#include "Base.h"

BEGIN(Engine)

class CGameObject;
class CTransformEx;

enum class ComponentType : uint8
{
	Transform,
	RigidBody,
	Collider,
	Buffer,
	Shader,
	Renderer,
	Texture,
	//MeshRenderer,
	//ModelRenderer,
	Camera,
	Animator,
	Light,
	Terrain,
	// ...
	Script,

	End,
};

enum
{
	FIXED_COMPONENT_COUNT = static_cast<uint8>(ComponentType::End) - 1
};

class ENGINE_DLL CComponent abstract : public CBase
{
protected:
	CComponent(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, ComponentType type);
	CComponent(const CComponent& rhs);
	virtual ~CComponent() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg)				PURE;
	virtual void	Tick(const _float& fTimeDelta)		{};
	virtual void	LateTick(const _float& fTimeDelta)	{};
	//virtual HRESULT FixedUpdate(_float fTimeDelta)	PURE;

	virtual void	DebugRender()						{};

public:
	ComponentType	GetType() const { return m_eType; }

	CGameObject*	GetGameObject() const;
	CTransformEx*	GetTransform() const;

protected:
	ID3D11Device*			m_pDevice = { nullptr };
	ID3D11DeviceContext*	m_pContext = { nullptr };

	_bool					m_isCloned = { false };

protected:
	ComponentType			m_eType;
	CGameObject*			m_pGameObject;

private:
	friend class CGameObject;
	void SetGameObject(CGameObject* _gameObject) { m_pGameObject = _gameObject; }

public:
	virtual CComponent* Clone(void* pArg) = 0;
	virtual void Free() override;
};

END