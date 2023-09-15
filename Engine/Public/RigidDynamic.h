#pragma once
#include "RigidBodyBase.h"

BEGIN(Engine)

enum class ForceMode
{
	FORCE,
	IMPULSE,
	VELOCITY_CHANGE,
	ACCELERATION
};

enum class Constraints
{
	FREEZE_X,
	FREEZE_Y,
	FREEZE_Z,
};

class ENGINE_DLL CRigidDynamic :
    public CRigidBody
{
	using Super = CRigidBody;

private:
	CRigidDynamic(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CRigidDynamic(const CRigidDynamic& rhs);
	virtual ~CRigidDynamic() = default;

public:
	virtual HRESULT Initialize_Prototype()		override;
	virtual HRESULT Initialize(void* pArg)		override;
	virtual void	Tick(const _float fTimeDelta)		override;
	virtual void	LateTick(const _float fTimeDelta)	override;
	virtual void	DebugRender()				override;

public:

#pragma region Property
	_bool	UseGravity()	const				{ return m_UseGravity; }
	void	UseGravity(_bool UseGravity)		{ m_UseGravity = UseGravity; }
	_bool	IsKinematic()	const				{ return m_IsKinematic; }
	void	IsKinematic(_bool IsKinematic)		{ m_IsKinematic = IsKinematic; }

	_float	GetMass()	const					{ return m_fMass; }
	_float	GetDrag()	const					{ return m_fDrag; }
	_float	GetAngularDrag()	const			{ return m_fAngularDrag; }

	_bool	IsFrozePosition(Constraints _eAxis) { return m_byConstraints & 1 << (_int)_eAxis; }
	void	FreezePosition(Constraints _eAxis)	{ m_byConstraints ^= 1 << (_int)_eAxis; }		// Switch On/Off
	_bool	IsFrozeRotation(Constraints _eAxis) { return m_byConstraints & 1 << ((_int)_eAxis + 3); }
	void	FreezeRotation(Constraints _eAxis)	{ m_byConstraints ^= 1 << ((_int)_eAxis + 3); }	// Switch On/Off

	// Velocity
	Vec3	GetLinearVelocity()	const			{ return m_vLinearVelocity; }
	void	SetLinearVelocity(const Vec3& vLinearVelocity);
	Vec3	GetAngularVelocity()				{ return m_vAngularVelocity; }
	void	SetAngularVelocity(const Vec3& vAngularVelocity);

	// Force/Torque modifiers
	void	AddForce(const Vec3& vForce, ForceMode eMode);
	void	AddTorque(const Vec3& vTorque, ForceMode eMode);
	void	SetForceAndTorque(const Vec3& vForce, const Vec3& vTorque, ForceMode eMode);

	void	ClearForce(ForceMode eMode);
	void	ClearTorque(ForceMode eMode);
	void	ClearNetPower();

#pragma endregion Property

#pragma region Collision
	virtual	void	OnCollisionEnter(const COLLISION_DESC& desc)override;
	virtual	void	OnCollisionStay(const COLLISION_DESC& desc)	override;
	virtual	void	OnCollisionExit(const COLLISION_DESC& desc)	override;
#pragma endregion Collision

private:
	void	KineticUpdate(_float fTimeDelta);
	void	KinematicUpdate(_float fTimeDelta);

	void	UpdateTransform(_float fTimeDelta);

	void	Sleep()		{ ClearNetPower(); m_IsSleeping = true;	}
	void	WakeUp()	{ m_IsSleeping = false; }

private:
	_bool	m_IsSleeping;
	const _float m_fSleepThreshold;

	// Description
	_bool	m_UseGravity;
	_bool	m_IsKinematic;

	_float	m_fMass;
	_float	m_fDrag;
	_float	m_fAngularDrag;

	// Constraints
	_byte	m_byConstraints;

	Vec3	m_vLinearAcceleration;
	Vec3	m_vAngularAcceleration;

	Vec3	m_vLinearVelocity;
	Vec3	m_vAngularVelocity;

public:
	static CRigidDynamic* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CComponent* Clone(void* pArg)	override;
	virtual void Free()						override;
};

END