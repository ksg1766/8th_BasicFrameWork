#pragma once
#include "Client_Defines.h"
#include "MonoBehaviour.h"
#include "RigidDynamic.h"
#include "Transform.h"
#include "NavMeshAgent.h"

BEGIN(Engine)

class CTransform;

END

BEGIN(Client)

class CParticleController : public CMonoBehaviour
{
	using Super = CMonoBehaviour;
public:
	enum class ParticleType { FLY, FLOAT, EXPLODE, TYPE_END};
	typedef struct tagParticleDesc
	{
		ParticleType	eType = ParticleType::TYPE_END;

		_float3			vCenter = _float3(0.f, 0.f, 0.f);
		_float3			vRange = _float3(2.f, 1.f, 2.f);
		_float3			vSpeedMin = _float3(-5.f, 4.f, -5.f), vSpeedMax = _float3(5.f, 6.f, 5.f);
		_float			fScaleMin = 0.1f, fScaleMax = 0.2f;
		_float			fLifeTimeMin = 1.f, fLifeTimeMax = 2.f;
	}PARTICLE_DESC;

private:
	CParticleController(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CParticleController(const CParticleController& rhs);
	virtual ~CParticleController() = default;

public:
	virtual HRESULT Initialize_Prototype()				override;
	virtual HRESULT Initialize(void* pArg)				override;
	virtual void	Tick(const _float& fTimeDelta)		override;
	virtual void	LateTick(const _float& fTimeDelta)	override;
	virtual void	DebugRender()						override;

private:
	void	RandomFly(const _float& fTimeDelta);
	void	RandomFloat(const _float& fTimeDelta);
	void	RandomExplode(const _float& fTimeDelta);

private:
	CTransform*		m_pTransform	= nullptr;
	CRigidDynamic*	m_pRigidBody	= nullptr;

	Vec3			m_vSpeed;
	_float			m_fLifeTIme		= 0.f;

	PARTICLE_DESC	m_tProps;

public:
	static	CParticleController* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CComponent* Clone(CGameObject* pGameObject, void* pArg) override;
	virtual void Free() override;
};

END