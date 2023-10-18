#pragma once

#include "Client_Defines.h"
#include "Strife_Ammo.h"

BEGIN(Client)

class CStrife_Ammo_Default final : public CStrife_Ammo
{
	using Super = CStrife_Ammo;

private:
	CStrife_Ammo_Default(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CStrife_Ammo_Default(const CStrife_Ammo_Default& rhs);
	virtual ~CStrife_Ammo_Default() = default;

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

public:
	virtual	void	OnCollisionEnter(const CGameObject* pOther)	override;
	virtual	void	OnCollisionStay(const CGameObject* pOther)	override;
	virtual	void	OnCollisionExit(const CGameObject* pOther)	override;

public:
	static	CStrife_Ammo_Default* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END