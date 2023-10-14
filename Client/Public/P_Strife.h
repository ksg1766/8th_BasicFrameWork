#pragma once

#include "Client_Defines.h"
#include "GameObject.h"

BEGIN(Client)

class CP_Strife final : public CGameObject
{
	using Super = CGameObject;

private:
	/* 원형을 생성할 때 */
	CP_Strife(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	/* 사본을 생성할 때 */
	CP_Strife(const CP_Strife& rhs); /* 복사 생성자. */
	virtual ~CP_Strife() = default;

public:
	virtual HRESULT Initialize_Prototype()				override;
	virtual HRESULT Initialize(void* pArg)				override;
	virtual void	Tick(const _float& fTimeDelta)		override;
	virtual void	LateTick(const _float& fTimeDelta)	override;
	virtual void	DebugRender()						override;
	virtual HRESULT Render()							override;

	//
	void			SetDissolveAmount(_float fDissolveAmount) { m_fDissolveAmount = fDissolveAmount; }

private:
	HRESULT			Ready_FixedComponents();
	HRESULT			Ready_Scripts();
	HRESULT			Ready_Parts();
	HRESULT			Bind_ShaderResources(); /* 셰이더 전역변수에 값 던진다. */

	vector<CGameObject*> m_vecParts;

	//
	_float			m_fDissolveAmount = 0.f;

public:
	static	CP_Strife* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override; /* 사본객체를 생성할때 원본데이터로부터 복제해올 데이터외에 더 추가해 줘야할 데이터가 있다라면 받아오겠다. */
	virtual void Free() override;
};

END