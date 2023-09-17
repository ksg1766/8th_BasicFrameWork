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
	virtual void	Tick(const _float& fTimeDelta)		override;

	CTransform* Get_Parent() { return m_pParent; }
	void	Set_Parent(CTransform* pParent) { m_pParent = pParent; m_pParent->Set_Child(this); }

	void	Scale(Vec3& vScale);
	void	Scale(const Vec3& vScale);

	void	Rotate(Vec3& vEulers);
	void	Rotate(const Vec3& vEulers);
	void	Rotate(const _float& fXangle, const _float& fYangle, const _float& fZangle);
	void	RotateAround(const Vec3& vPoint, const Vec3& vAxis, const _float& fAngle);	// Revolution;

	void	Translate(Vec3& vTranslation);
	void	Translate(const Vec3& vTranslation);

	const _matrix	WorldMatrix();
	void	Set_WorldMatrix(_matrix& matWorld);

	Vec3	LocalScale();
	vector<CTransform*>& Get_Child() { return m_pChild; }

public:
	CTransform* m_pParent;

	// RUL�� ũ�Ⱑ �� ���� ����(ũ��)
	// 1, 2, 3 ���� Normalize�ϰ� ũ�⸦ ���ϴ°��� Scaling
	// ȸ���� �ʱ�ȭ : ���� ���� ����� 1, 2, 3 ���� ũ�⸦ ����
	// RUL�� �ʱ�ȭ �� �ٽ� ũ�⸦ ���ϸ� ȸ����� �ʱ�ȭ

	//Vec3			m_vInfo[INFO_END];
	Vec3			m_vLocalScale;

private:
	void					Set_Child(CTransform* pChild) { m_pChild.push_back(pChild); }
	vector<CTransform*>		m_pChild;

public:
	static CTransform* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CComponent* Clone(void* pArg) override;	// Awake
	virtual void Free() override;
};

END