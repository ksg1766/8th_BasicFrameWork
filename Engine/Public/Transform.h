#pragma once
#include "Component.h"

BEGIN(Engine)

class ENGINE_DLL CTransform final : public CComponent
{
	using Super = CComponent;

private:
	CTransform(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CTransform(const CTransform& rhs);
	virtual ~CTransform() = default;

public:
	virtual HRESULT Initialize_Prototype()				override;
	virtual HRESULT Initialize(void* pArg)				override;	// Start

	virtual void	Tick(_float fTimeDelta)				override;
	virtual void	LateTick(_float fTimeDelta)			override;
	//virtual HRESULT FixedUpdate(_float fTimeDelta)	override;

	virtual void	DebugRender()						override;

	void UpdateTransform();

	static Vec3 ToEulerAngles(Quaternion q);

	// Local
	Vec3 GetLocalScale() { return m_vLocalScale; }
	void SetLocalScale(const Vec3& localScale) { m_vLocalScale = localScale; UpdateTransform(); }
	Vec3 GetLocalRotation() { return m_vLocalRotation; }
	void SetLocalRotation(const Vec3& localRotation) { m_vLocalRotation = localRotation; UpdateTransform(); }
	Vec3 GetLocalPosition() { return m_vLocalRotation; }
	void SetLocalPosition(const Vec3& localPosition) { m_vLocalRotation = localPosition; UpdateTransform(); }

	// World
	Vec3 GetScale() { return m_vScale; }
	void SetScale(const Vec3& scale);
	Vec3 GetRotation() { return m_vRotation; }
	void SetRotation(const Vec3& rotation);
	Vec3 GetPosition() { return m_vPosition; }
	void SetPosition(const Vec3& position);

	Vec3 GetRight() { return m_matWorld.Right(); }
	Vec3 GetUp() { return m_matWorld.Up(); }
	Vec3 GetLook() { return m_matWorld.Backward(); }

	Matrix GetWorldMatrix() { return m_matWorld; }

	// °èÃþ °ü°è
	bool HasParent() { return m_pParent != nullptr; }
	
	CTransform* GetParent() { return m_pParent; }
	void SetParent(CTransform* parent) { m_pParent = parent; }

	const vector<CTransform*>& GetChildren() { return m_vecChildren; }
	void AddChild(CTransform* child) { m_vecChildren.push_back(child); }

private:
	Vec3 m_vLocalScale = { 1.f, 1.f, 1.f }; 
	Vec3 m_vLocalRotation = { 0.f, 0.f, 0.f };
	Vec3 m_vLocalPosition = { 0.f, 0.f, 0.f };

	// Cache
	Matrix m_matLocal = Matrix::Identity;
	Matrix m_matWorld = Matrix::Identity;
	
	Vec3 m_vScale;
	Vec3 m_vRotation;
	Vec3 m_vPosition;

private:
	CTransform* m_pParent;
	vector<CTransform*> m_vecChildren;

public:
	static CTransform* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CComponent* Clone(void* pArg) override;	// Awake
	virtual void Free() override;
};

END