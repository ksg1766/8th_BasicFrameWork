#include "stdafx.h"
#include "ParticleController.h"
#include "GameInstance.h"
#include "GameObject.h"
#include "DebugDraw.h"

CParticleController::CParticleController(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:Super(pDevice, pContext)
{
}

CParticleController::CParticleController(const CParticleController& rhs)
	:Super(rhs)
{
}

HRESULT CParticleController::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CParticleController::Initialize(void* pArg)
{
	m_pTransform = GetTransform();
	//m_pRigidBody = static_cast<CRigidDynamic*>(m_pGameObject->GetRigidBody());
	
	if (nullptr == pArg)
		return E_FAIL;

	m_tProps = *reinterpret_cast<PARTICLE_DESC*>(pArg);

	random_device		RandomDevice;

	mt19937_64							RandomNumber(RandomDevice());
	uniform_real_distribution<_float>	RandomX(m_tProps.vRange.x * -0.5f, m_tProps.vRange.x * 0.5f);
	uniform_real_distribution<_float>	RandomY(m_tProps.vRange.y * -0.5f, m_tProps.vRange.y * 0.5f);
	uniform_real_distribution<_float>	RandomZ(m_tProps.vRange.z * -0.5f, m_tProps.vRange.z * 0.5f);

	uniform_real_distribution<_float>	RandomSpeedX(m_tProps.vSpeedMin.x, m_tProps.vSpeedMax.x);
	uniform_real_distribution<_float>	RandomSpeedY(m_tProps.vSpeedMin.y, m_tProps.vSpeedMax.y);
	uniform_real_distribution<_float>	RandomSpeedZ(m_tProps.vSpeedMin.z, m_tProps.vSpeedMax.z);
	uniform_real_distribution<_float>	RandomLifeTime(m_tProps.fLifeTimeMin, m_tProps.fLifeTimeMax);

	uniform_real_distribution<_float>	RandomScale(m_tProps.fScaleMin, m_tProps.fScaleMax);

	_float		fScale = RandomScale(RandomNumber);

	m_vSpeed.x = RandomSpeedX(RandomDevice);
	m_vSpeed.y = RandomSpeedY(RandomDevice);
	m_vSpeed.z = RandomSpeedZ(RandomDevice);
	m_fLifeTIme = RandomLifeTime(RandomDevice);

	m_pTransform->SetScale(Vec3(fScale));
	m_pTransform->SetPosition(Vec3(m_tProps.vCenter.x + RandomX(RandomNumber), m_tProps.vCenter.y + RandomY(RandomNumber), m_tProps.vCenter.z + RandomZ(RandomNumber)));

	m_pTransform->WorldMatrix().Backward(m_vSpeed);

	//m_pRigidBody->AddForce(m_vSpeed, ForceMode::IMPULSE);
	//m_pRigidBody->SetMaterialDrag(0.001f);

	return S_OK;
}

void CParticleController::Tick(const _float& fTimeDelta)
{
	switch (m_tProps.eType)
	{
	case ParticleType::FLY:
		RandomFly(fTimeDelta);
		break;
	case ParticleType::FLOAT:
		RandomFloat(fTimeDelta);
		break;
	case ParticleType::EXPLODE:
		RandomExplode(fTimeDelta);
		break;
	}
}

void CParticleController::LateTick(const _float& fTimeDelta)
{
}

void CParticleController::DebugRender()
{
}

void CParticleController::RandomFly(const _float& fTimeDelta)
{
	m_fLifeTIme -= fTimeDelta;

	if (m_fLifeTIme > 0.f)
	{
		//m_pTransform->Translate(m_vSpeed * fTimeDelta);
	}
	else
	{
		m_pGameInstance->DeleteObject(m_pGameObject);
	}
}

void CParticleController::RandomFloat(const _float& fTimeDelta)
{
}

void CParticleController::RandomExplode(const _float& fTimeDelta)
{
}

CParticleController* CParticleController::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CParticleController* pInstance = new CParticleController(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CParticleController");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CComponent* CParticleController::Clone(CGameObject* pGameObject, void* pArg)
{
	CParticleController* pInstance = new CParticleController(*this);
	pInstance->m_pGameObject = pGameObject;

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed To Cloned : CParticleController");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CParticleController::Free()
{
	Super::Free();
}
