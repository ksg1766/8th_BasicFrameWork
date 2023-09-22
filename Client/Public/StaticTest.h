#pragma once

#include "Client_Defines.h"
#include "GameObject.h"

BEGIN(Client)

class CStaticTest final : public CGameObject
{
	using Super = CGameObject;
private:
	CStaticTest(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CStaticTest(const CStaticTest& rhs);
	virtual ~CStaticTest() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual void	Tick(const _float& fTimeDelta);
	virtual void	LateTick(const _float& fTimeDelta);
	virtual HRESULT Render();

private:
	HRESULT			Ready_FixedComponents();
	HRESULT			Ready_Scripts();
	HRESULT			Bind_ShaderResources(); /* ���̴� ���������� �� ������. */

public:
	static	CStaticTest* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;
};

END