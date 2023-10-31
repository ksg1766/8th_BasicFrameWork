#pragma once

#include "Client_Defines.h"
#include "GameObject.h"

BEGIN(Client)

class CHellHound final : public CGameObject
{
	using Super = CGameObject;
private:
	/* ������ ������ �� */
	CHellHound(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	/* �纻�� ������ �� */
	CHellHound(const CHellHound& rhs); /* ���� ������. */
	virtual ~CHellHound() = default;

public:
	virtual HRESULT Initialize_Prototype()				override;
	virtual HRESULT Initialize(void* pArg)				override;
	virtual void	Tick(const _float& fTimeDelta)		override;
	virtual void	LateTick(const _float& fTimeDelta)	override;
	virtual void	DebugRender()						override;
	virtual HRESULT Render()							override;
	virtual HRESULT RenderInstance()					override;

	//	���� �̰� ������ �Ǵ°ǰ�.
	void			SetDissolveAmount(_float fDissolveAmount) { m_fDissolveAmount = fDissolveAmount; }

private:
	HRESULT			Bind_ShaderResources(); /* ���̴� ���������� �� ������. */
	HRESULT			Ready_FixedComponents();
	HRESULT			Ready_Scripts();

	//
	_float			m_fDissolveAmount = 0.f;

public:
	// TODO: Temp (�̷� �����ʹ� BlakcBoard�� �ű���. ������ �׽�Ʈ�� ���� �ӽ� �ڵ�)
	_bool			m_IsZeroHP = false;

public:
	static	CHellHound* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override; /* �纻��ü�� �����Ҷ� ���������ͷκ��� �����ؿ� �����Ϳܿ� �� �߰��� ����� �����Ͱ� �ִٶ�� �޾ƿ��ڴ�. */
	virtual void Free() override;
};

END