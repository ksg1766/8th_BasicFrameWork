#pragma once
#include "Client_Defines.h"
#include "MonoBehaviour.h"
#include "RigidDynamic.h"
#include "Strife_Ammo.h"

BEGIN(Engine)

class CTransform;

END

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
	_bool	IsIdle();
	_bool	IsRun();
	_bool	IsAim();
	_bool	IsJump();
	_bool	IsDash();

	void	GetMoveMessage(const Vec3& vDir)	{ m_vNetTrans += vDir; }
	void	GetJumpMessage(const _bool& IsJump)	{ IsJump ? Jump() : Land();}
	void	GetDashMessage(const _bool& IsDash);// { IsDash ? Dash(m_pTransform->GetForward()) : DashEnd(); }
	//void	GetFireMessage(CStrife_Ammo::AmmoType eAmmoType) { Fire(eAmmoType) :  }

	void	ForceHeight()				{ m_pTransform->Translate(Vec3(0.f, m_pNavMeshAgent->GetHeightOffset(), 0.f)); }
	_float	GetHeightOffset()			{ return m_pNavMeshAgent->GetHeightOffset(); }
	_bool	Walkable(_fvector vPoint)	{ return m_pNavMeshAgent->Walkable(vPoint); }

	void	Fire(CStrife_Ammo::AmmoType eAmmoType);

private:
	void	Input(const _float& fTimeDelta);
	void	Move(const _float& fTimeDelta);
	void	Jump();
	void	Land();
	void	Dash(const Vec3& vDir);
	void	DashEnd();
	//void	Fire(const _float& fTimeDelta, CStrife_Ammo::AmmoType eAmmoType);

	void	LimitAllAxisVelocity();

private:
	CTransform*		m_pTransform = nullptr;
	CRigidDynamic*	m_pRigidBody = nullptr;
	CNavMeshAgent*	m_pNavMeshAgent = nullptr;
	Vec3			m_vPrePos;

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