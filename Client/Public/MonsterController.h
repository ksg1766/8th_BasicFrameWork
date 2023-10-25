#pragma once
#include "Client_Defines.h"
#include "MonoBehaviour.h"
#include "RigidDynamic.h"

BEGIN(Engine)

class CTransform;
class CNavMeshAgent;

END

BEGIN(Client)

class CMonsterController : public CMonoBehaviour
{
	using Super = CMonoBehaviour;

private:
	CMonsterController(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CMonsterController(const CMonsterController& rhs);
	virtual ~CMonsterController() = default;

public:
	virtual HRESULT Initialize_Prototype()				override;
	virtual HRESULT Initialize(void* pArg)				override;
	virtual void	Tick(const _float& fTimeDelta)		override;
	virtual void	LateTick(const _float& fTimeDelta)	override;
	virtual void	DebugRender()						override;

public:
	void	GetMoveMessage(const Vec3& vDir)						{ m_vNetMove += vDir; }
	void	GetTranslateMessage(const Vec3& vDir)					{ m_vNetTrans += vDir; }
	void	GetAttackMessage(const _bool& IsAttack)					{  }
	void	GetChaseMessage(const _bool& IsChase)					{  }

	void	ForceHeight()				{ m_pTransform->Translate(Vec3(0.f, m_pNavMeshAgent->GetHeightOffset(), 0.f)); }
	_float	GetHeightOffset()			{ return m_pNavMeshAgent->GetHeightOffset(); }
	_bool	Walkable(_fvector vPoint)	{ return m_pNavMeshAgent->Walkable(vPoint); }

	void	Look(const Vec3& vPoint, const _float& fTimeDelta);

private:
	void	Input(const _float& fTimeDelta);
	void	Move(const _float& fTimeDelta);
	void	Translate(const _float& fTimeDelta);
	void	Attack();
	void	Chase();

	void	LimitAllAxisVelocity();

private:
	CTransform*		m_pTransform = nullptr;
	CRigidDynamic*	m_pRigidBody = nullptr;
	CNavMeshAgent*	m_pNavMeshAgent = nullptr;
	Vec3			m_vPrePos;

	Vec3			m_vNetMove;
	Vec3			m_vNetTrans;

	Vec3			m_vMaxLinearSpeed;
	Vec3			m_vLinearSpeed;

	Vec3			m_vMaxAngularSpeed;
	Vec3			m_vAngularSpeed;

public:
	static	CMonsterController* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CComponent* Clone(CGameObject* pGameObject, void* pArg) override;
	virtual void Free() override;
};

END