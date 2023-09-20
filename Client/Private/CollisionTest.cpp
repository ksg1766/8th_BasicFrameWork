#include "stdafx.h"
#include "..\Public\CollisionTest.h"
#include "GameInstance.h"

CCollisionTest::CCollisionTest(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: CGameObject(pDevice, pContext)
{
}

CCollisionTest::CCollisionTest(const CCollisionTest& rhs)
	: CGameObject(rhs)
{
}

HRESULT CCollisionTest::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CCollisionTest::Initialize(void * pArg)
{
	if (FAILED(Ready_FixedComponents()))
		return E_FAIL;

	if (FAILED(Ready_Scripts()))
		return E_FAIL;
	
	GetTransform()->SetScale(Vec3(40.f, 40.f, 40.f));
	GetTransform()->Translate(Vec3(0.f, 0.f, 200.f));

	return S_OK;
}

void CCollisionTest::Tick(const _float& fTimeDelta)
{
	Super::Tick(fTimeDelta);

	//GetRigidBody()->Tick(fTimeDelta);
}

void CCollisionTest::LateTick(const _float& fTimeDelta)
{
	Super::LateTick(fTimeDelta);

	GetRenderer()->Add_RenderGroup(CRenderer::RG_NONBLEND, this);
}

HRESULT CCollisionTest::Render()
{
	if (nullptr == GetBuffer() || nullptr == GetShader())
		return E_FAIL;

	if (FAILED(Bind_ShaderResources()))
		return E_FAIL;

	GetShader()->Begin(1);

	GetBuffer()->Render();

#ifdef _DEBUG

#endif

	return S_OK;
}

HRESULT CCollisionTest::Ready_FixedComponents()
{
	/* Com_Transform */
	if (FAILED(Super::AddComponent(LEVEL_STATIC, ComponentType::Transform, TEXT("Prototype_Component_Transform"))))
		return E_FAIL;

	/* Com_Renderer */
	if (FAILED(Super::AddComponent(LEVEL_STATIC, ComponentType::Renderer, TEXT("Prototype_Component_Renderer"))))
		return E_FAIL;

	/* Com_Shader */
	if (FAILED(Super::AddComponent(LEVEL_STATIC, ComponentType::Shader, TEXT("Prototype_Component_Shader_VtxCubeTex"))))
		return E_FAIL;

	/* Com_VIBuffer */
	if (FAILED(Super::AddComponent(LEVEL_STATIC, ComponentType::Buffer, TEXT("Prototype_Component_VIBuffer_Cube"))))
		return E_FAIL;

	/* Com_Texture */
	if (FAILED(Super::AddComponent(LEVEL_GAMEPLAY, ComponentType::Texture, TEXT("Prototype_Component_Texture_Sky"))))
		return E_FAIL;

	/* Com_RigidBody */
	if (FAILED(Super::AddComponent(LEVEL_STATIC, ComponentType::RigidBody, TEXT("Prototype_Component_RigidDynamic")))
		|| FAILED(GetRigidBody()->InitializeCollider()))
		return E_FAIL;

	//// TODO: 함수로 뺼 것.
	//m_pSphereCollider = dynamic_cast<CSphereCollider*>(CComponentManager::GetInstance()
	//	->Clone_Component(m_pGameObject, 0, TEXT("Prototype_Component_SphereCollider"), nullptr));
	//m_pOBBCollider = dynamic_cast<COBBCollider*>(CComponentManager::GetInstance()
	//	->Clone_Component(m_pGameObject, 0, TEXT("Prototype_Component_OBBCollider"), nullptr));

	//if (FAILED(Super::AddComponent(LEVEL_GAMEPLAY, ComponentType::Collider, TEXT("Prototype_Component_SphereCollider"))))
	//	return E_FAIL;
	//Super::AddComponent(LEVEL_GAMEPLAY, ComponentType::Collider, TEXT("Prototype_Component_OBBCollider"));
	
	return S_OK;
}

HRESULT CCollisionTest::Ready_Scripts()
{
	/* Com_PlayerController */
	if (FAILED(Super::AddComponent(LEVEL_GAMEPLAY, ComponentType::Script, TEXT("Prototype_Component_TestAIController"))))
		return E_FAIL;

	return S_OK;
}

HRESULT CCollisionTest::Bind_ShaderResources()
{
	Matrix		ViewMatrix, ProjMatrix;
	ViewMatrix = XMMatrixLookAtLH(Vec4(0.f, 500.f, -500.f, 1.f), Vec4(0.f, 0.f, 0.f, 1.f), Vec4(0.0f, 1.f, 0.f, 0.f));
	ProjMatrix = XMMatrixPerspectiveFovLH(XMConvertToRadians(60.0f), g_iWinSizeX / (_float)g_iWinSizeY, 0.1f, 1000.f);

	if (FAILED(GetTransform()->Bind_ShaderResources(GetShader(), "g_WorldMatrix")))
		return E_FAIL;
	if (FAILED(GetShader()->Bind_Matrix("g_ViewMatrix", &ViewMatrix)))
		return E_FAIL;
	if (FAILED(GetShader()->Bind_Matrix("g_ProjMatrix", &ProjMatrix)))
		return E_FAIL;

	if (FAILED(GetTexture()->Bind_ShaderResource(GetShader(), "g_DiffuseTexture", 1)))
		return E_FAIL;

	return S_OK;
}

CCollisionTest* CCollisionTest::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
{
	CCollisionTest*		pInstance = new CCollisionTest(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed To Created : CCollisionTest");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CCollisionTest::Clone(void * pArg)
{
	CCollisionTest*		pInstance = new CCollisionTest(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed To Cloned : CCollisionTest");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CCollisionTest::Free()
{
	Super::Free();
}
