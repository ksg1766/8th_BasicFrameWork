#pragma once
#include "Component.h"

BEGIN(Engine)

class ENGINE_DLL CTransform final : public CComponent
{
	using Super = CComponent;

public:
	enum STATE { STATE_RIGHT, STATE_UP, STATE_LOOK, STATE_POSITION, STATE_END };

private:
	CTransform(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CTransform(const CTransform& rhs);
	virtual ~CTransform() = default;

public:
	virtual HRESULT Initialize_Prototype()				override;
	virtual HRESULT Initialize(void* pArg)				override;	// Start
	virtual void	Tick(const _float& fTimeDelta)		override;

	void	SetScale(Vec3& vScale);
	void	SetScale(const Vec3& vScale)			{ SetScale(const_cast<Vec3&>(vScale)); }

	void	Rotate(Vec3& vEulers);
	void	Rotate(const Vec3& vEulers)				{ Rotate(const_cast<Vec3&>(vEulers)); }
	void	Rotate(const _float& fXangle, const _float& fYangle, const _float& fZangle)
													{ Rotate(Vec3(fXangle, fYangle, fZangle)); }
	void	RotateAround(const Vec3& vPoint, const Vec3& vAxis, const _float& fAngle);	// Revolution;
	//void	SetRotation(Vec3& vRotation);

	void	Translate(Vec3& vTranslation);
	void	Translate(const Vec3& vTranslation)		{ Translate(const_cast<Vec3&>(vTranslation)); }
	void	SetPosition(Vec3& vPosition)			{ static_cast<Vec3>(m_matWorld.m[STATE_POSITION]) = vPosition; }
	void	SetPosition(const Vec3& vPosition)		{ SetPosition(const_cast<Vec3&>(vPosition)); }
	
	const Matrix	WorldMatrix()					{ return m_matWorld; }
	void	Set_WorldMatrix(const Matrix& matWorld) { ::CopyMemory(&m_matWorld, &matWorld, sizeof(Matrix)); }

	Vec3	GetLocalScale();
	Vec3	GetRotation();
	Vec3	GetPosition()							{ return static_cast<Vec3>(m_matWorld.m[STATE_POSITION]); }

public:
	// RUL�� ũ�Ⱑ �� ���� ����(ũ��)
	// 1, 2, 3 ���� Normalize�ϰ� ũ�⸦ ���ϴ°��� Scaling
	// ȸ���� �ʱ�ȭ : ���� ���� ����� 1, 2, 3 ���� ũ�⸦ ����
	// RUL�� �ʱ�ȭ �� �ٽ� ũ�⸦ ���ϸ� ȸ����� �ʱ�ȭ

	static Vec3 ToEulerAngles(Quaternion q);
	HRESULT	Bind_ShaderResources(class CShader* pShader, const _char* pConstantName);

private:
	Matrix m_matWorld = Matrix::Identity;

public:
	static CTransform* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CComponent* Clone(CGameObject* pGameObject, void* pArg) override;	// Awake
	virtual void Free() override;
};

END