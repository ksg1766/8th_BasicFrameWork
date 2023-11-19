#pragma once

#include "Client_Defines.h"
#include "GameObject.h"

BEGIN(Client)

class CTremorCrystal : public CGameObject
{
	using Super = CGameObject;

public:
	typedef struct tagEffectDescription
	{
		_float		fLifeTime = 0.f;

	}EFFECT_DESC;

private:
	/* ������ ������ �� */
	CTremorCrystal(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	/* �纻�� ������ �� */
	CTremorCrystal(const CTremorCrystal& rhs); /* ���� ������. */
	virtual ~CTremorCrystal() = default;

public:
	virtual HRESULT Initialize_Prototype()				override;
	virtual HRESULT Initialize(void* pArg)				override;
	virtual void	Tick(const _float& fTimeDelta)		override;
	virtual void	LateTick(const _float& fTimeDelta)	override;
	virtual void	DebugRender()						override;
	virtual HRESULT Render()							override;

private:
	HRESULT			Ready_FixedComponents();
	HRESULT			Ready_Scripts();
	HRESULT			Bind_ShaderResources(); /* ���̴� ���������� �� ������. */

private:
	_float			m_fLifeTime = 0.f;

public:
	static	CTremorCrystal* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg = nullptr) override; /* �纻��ü�� �����Ҷ� ���������ͷκ��� �����ؿ� �����Ϳܿ� �� �߰��� ����� �����Ͱ� �ִٶ�� �޾ƿ��ڴ�. */
	virtual void Free() override;
};

END