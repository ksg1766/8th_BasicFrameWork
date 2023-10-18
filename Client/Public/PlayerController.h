#pragma once

#include "Client_Defines.h"
#include "MonoBehaviour.h"
#include "RigidDynamic.h"
#include "Strife_Ammo.h"

BEGIN(Client)

class CPlayerController : public CMonoBehaviour
{
	using Super = CMonoBehaviour;

private:
	CPlayerController(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CPlayerController(const CPlayerController& rhs);
	virtual ~CPlayerController() = default;

public:
	virtual HRESULT Initialize_Prototype()				override;
	virtual HRESULT Initialize(void* pArg)				override;
	virtual void	Tick(const _float& fTimeDelta)		override;
	virtual void	LateTick(const _float& fTimeDelta)	override;
	virtual void	DebugRender()						override;

public:
	_bool	Idle();
	_bool	Run();
	_bool	Aim();
	_bool	Jump();
	_bool	Dash();

	void	GetMoveMessage(const Vec3& vDir) { m_vNetTrans += vDir; }
	void	Fire(const _float& fTimeDelta, CStrife_Ammo::AmmoType eAmmoType);

private:
	void	Input(const _float& fTimeDelta);
	void	Move(const _float& fTimeDelta);
	void	LimitAllAxisVelocity();

private:
	CTransform*		m_pTransform = nullptr;
	CRigidDynamic*	m_pRigidBody = nullptr;

	Vec3			m_vNetTrans;

	Vec3			m_vMaxLinearSpeed;
	Vec3			m_vLinearSpeed;

	Vec3			m_vMaxAngularSpeed;
	Vec3			m_vAngularSpeed;

	_bool			m_bFireLR = true;

public:
	static	CPlayerController* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CComponent* Clone(CGameObject* pGameObject, void* pArg) override;
	virtual void Free() override;
};

END