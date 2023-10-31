#pragma once

#include "Client_Defines.h"
#include "GameObject.h"

BEGIN(Client)

class CHellHound final : public CGameObject
{
	using Super = CGameObject;
private:
	/* 원형을 생성할 때 */
	CHellHound(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	/* 사본을 생성할 때 */
	CHellHound(const CHellHound& rhs); /* 복사 생성자. */
	virtual ~CHellHound() = default;

public:
	virtual HRESULT Initialize_Prototype()				override;
	virtual HRESULT Initialize(void* pArg)				override;
	virtual void	Tick(const _float& fTimeDelta)		override;
	virtual void	LateTick(const _float& fTimeDelta)	override;
	virtual void	DebugRender()						override;
	virtual HRESULT Render()							override;
	virtual HRESULT RenderInstance()					override;

	//	뭐야 이거 지워도 되는건가.
	void			SetDissolveAmount(_float fDissolveAmount) { m_fDissolveAmount = fDissolveAmount; }

private:
	HRESULT			Bind_ShaderResources(); /* 셰이더 전역변수에 값 던진다. */
	HRESULT			Ready_FixedComponents();
	HRESULT			Ready_Scripts();

	//
	_float			m_fDissolveAmount = 0.f;

public:
	// TODO: Temp (이런 데이터는 BlakcBoard로 옮기자. 디졸브 테스트를 위한 임시 코드)
	_bool			m_IsZeroHP = false;

public:
	static	CHellHound* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override; /* 사본객체를 생성할때 원본데이터로부터 복제해올 데이터외에 더 추가해 줘야할 데이터가 있다라면 받아오겠다. */
	virtual void Free() override;
};

END