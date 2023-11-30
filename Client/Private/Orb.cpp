#include "stdafx.h"
#include "..\Public\Orb.h"
#include "GameInstance.h"
#include "SphereSwirl.h"

COrb::COrb(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: Super(pDevice, pContext)
{
}

COrb::COrb(const COrb& rhs)
	: Super(rhs)
{
}

HRESULT COrb::Initialize_Prototype()
{
	return S_OK;
}

HRESULT COrb::Initialize(void* pArg)
{
	if (FAILED(Ready_FixedComponents()))
		return E_FAIL;

	if (FAILED(Ready_Scripts()))
		return E_FAIL;

	GetTransform()->SetScale(Vec3(4.f, 4.f, 4.f));

	return S_OK;
}

void COrb::Tick(const _float& fTimeDelta)
{
	Super::Tick(fTimeDelta);
	m_fFrameTime += fTimeDelta;

	if (!m_bSpark)
	{
		m_pGameInstance->CreateObject(TEXT("Prototype_GameObject_Lightning_Spark"), LAYERTAG::IGNORECOLLISION)->GetTransform()->Translate(GetTransform()->GetPosition() - 2.f * Vec3::UnitY);

		m_pSphereSwirl = m_pGameInstance->CreateObject(TEXT("Prototype_GameObject_SphereSwirl"), LAYERTAG::IGNORECOLLISION);
		m_pSphereSwirl->GetTransform()->Translate(GetTransform()->GetPosition());

		m_bSpark = true;
	}

	if (m_fFrameTime < 0.4f)
	{
		GetTransform()->SetScale(Vec3(10.f * m_fFrameTime, 10.f * m_fFrameTime, 10.f * m_fFrameTime));
		m_pSphereSwirl->GetTransform()->SetScale(Vec3(10.f * m_fFrameTime, 10.f * m_fFrameTime, 10.f * m_fFrameTime));
	}
	else
	{
		if (!m_bSpawned)
		{
			GetTransform()->SetScale(Vec3(4.f, 4.f, 4.f));
			m_pSphereSwirl->GetTransform()->SetScale(Vec3(4.f, 4.f, 4.f));
			m_bSpawned = true;
		}
	}

	if (m_fFrameTime > 10.f)
	{
		GetTransform()->SetScale(Vec3(1.f / 2.5f * (m_fFrameTime - 9.f), 1.f / 2.5f * (m_fFrameTime - 9.f), 1.f / 2.5f * (m_fFrameTime - 9.f)));
		m_pSphereSwirl->GetTransform()->SetScale(Vec3(1.f / 2.5f * (m_fFrameTime - 9.f), 1.f / 2.5f * (m_fFrameTime - 9.f), 1.f / 2.5f * (m_fFrameTime - 9.f)));
	}

	if (m_fFrameTime > 11.5f)
	{
		m_pGameInstance->DeleteObject(this);
		m_pGameInstance->DeleteObject(m_pSphereSwirl);
	}
}

void COrb::LateTick(const _float& fTimeDelta)
{
	Super::LateTick(fTimeDelta);

	GetRenderer()->Add_RenderGroup(CRenderer::RG_NONBLEND, this);
}

void COrb::DebugRender()
{
}

HRESULT COrb::Render()
{
	if (nullptr == GetModel() || nullptr == GetShader())
		return E_FAIL;

	if (FAILED(Bind_ShaderResources()))
		return E_FAIL;

	if (FAILED(GetModel()->Render()))
		return E_FAIL;

	if (m_fFrameTime < 0.4f)
	{
		GetShader()->SetPassIndex(5);
		if (FAILED(m_pWispModel->Render()))
			return E_FAIL;

		/*GetShader()->SetPassIndex(3);
		for (_int i = 0; i < 4; ++i)
		{
			if (FAILED(m_pSparkModels[i]->Render()))
				return E_FAIL;

			GetTransform()->RotateYAxisFixed(Vec3(0.f, 90.f, 0.f));
		}*/
		GetShader()->SetPassIndex(4);
	}

#ifdef _DEBUG
	Super::DebugRender();
#endif

	return S_OK;
}

HRESULT COrb::Ready_FixedComponents()
{
	/* Com_Shader */
	if (FAILED(Super::AddComponent(LEVEL_STATIC, ComponentType::Shader, TEXT("Prototype_Component_Shader_Sphere"))))
		return E_FAIL;
	GetShader()->SetPassIndex(4);

	/* Com_Model */
	if (FAILED(Super::AddComponent(LEVEL_STATIC, ComponentType::Model, TEXT("Prototype_Component_Model_") + GetObjectTag())))
		return E_FAIL;

	m_pWispModel = static_cast<CModel*>(m_pGameInstance->Clone_Component(this, LEVEL_STATIC, TEXT("Prototype_Component_Model_Wisp")));
	/*m_pSparkModels[0] = static_cast<CModel*>(m_pGameInstance->Clone_Component(this, LEVEL_STATIC, TEXT("Prototype_Component_Model_Lightning_Spark0")));
	m_pSparkModels[1] = static_cast<CModel*>(m_pGameInstance->Clone_Component(this, LEVEL_STATIC, TEXT("Prototype_Component_Model_Lightning_Spark1")));
	m_pSparkModels[2] = static_cast<CModel*>(m_pGameInstance->Clone_Component(this, LEVEL_STATIC, TEXT("Prototype_Component_Model_Lightning_Spark2")));
	m_pSparkModels[3] = static_cast<CModel*>(m_pGameInstance->Clone_Component(this, LEVEL_STATIC, TEXT("Prototype_Component_Model_Lightning_Spark3")));*/

	/* Com_Transform */
	if (FAILED(Super::AddComponent(LEVEL_STATIC, ComponentType::Transform, TEXT("Prototype_Component_Transform"))))
		return E_FAIL;

	/* Com_Renderer */
	if (FAILED(Super::AddComponent(LEVEL_STATIC, ComponentType::Renderer, TEXT("Prototype_Component_Renderer"))))
		return E_FAIL;

	return S_OK;
}

HRESULT COrb::Ready_Scripts()
{
	return S_OK;
}

HRESULT COrb::Bind_ShaderResources()
{
	/* ���̴� ���������� ������ �� ������ ������. */
	if (FAILED(GetTransform()->Bind_ShaderResources(GetShader(), "g_WorldMatrix"))||
		FAILED(m_pGameInstance->Bind_TransformToShader(GetShader(), "g_ViewMatrix", CPipeLine::D3DTS_VIEW)) ||
		FAILED(m_pGameInstance->Bind_TransformToShader(GetShader(), "g_ProjMatrix", CPipeLine::D3DTS_PROJ)))
		return E_FAIL;

	if(FAILED(GetShader()->Bind_RawValue("g_fFrameTime", &m_fFrameTime, sizeof(_float))))
		return E_FAIL;

	return S_OK;
}

COrb* COrb::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	COrb* pInstance = new COrb(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : COrb");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* COrb::Clone(void* pArg)
{
	COrb* pInstance = new COrb(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : COrb");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void COrb::Free()
{
	Safe_Release(m_pWispModel);
	for(_int i = 0; i < 4; ++i)
		Safe_Release(m_pSparkModels[i]);

	Super::Free();
}
