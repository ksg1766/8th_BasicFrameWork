#include "stdafx.h"
#include "..\Public\Level_GamePlay.h"

#include "GameInstance.h"
#include "GameObject.h"

CLevel_GamePlay::CLevel_GamePlay(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: CLevel(pDevice, pContext)
{
}

HRESULT CLevel_GamePlay::Initialize()
{
	m_pGameInstance = GET_INSTANCE(CGameInstance);

	if (FAILED(Ready_Layer_Terrain()))
		return E_FAIL;

	if (FAILED(Ready_Layer_Ground()))
		return E_FAIL;
	
	if (FAILED(Ready_Layer_Default()))
		return E_FAIL;
	
	if (FAILED(Ready_Layer_Player()))
		return E_FAIL;

	if (FAILED(Ready_Layer_UnitGround()))
		return E_FAIL;

	if (FAILED(Ready_Layer_UnitAir()))
		return E_FAIL;

	if (FAILED(Ready_Layer_Equipment()))
		return E_FAIL;

	if (FAILED(Ready_Layer_Camera()))
		return E_FAIL;

	if (FAILED(Ready_Layer_Wall()))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Build_QuadTree(LEVELID::LEVEL_GAMEPLAY)))
		return E_FAIL;

	return S_OK;
}

HRESULT CLevel_GamePlay::Tick(const _float& fTimeDelta)
{
	m_pGameInstance->Update_QuadTree();

	return S_OK;
}

HRESULT CLevel_GamePlay::LateTick(const _float& fTimeDelta)
{
	//SetWindowText(g_hWnd, TEXT("게임플레이 레벨입니다."));
	
	// TODO: 꼭 영기 말고 더 좋은 위치를 찾도록 하자....
	m_pGameInstance->LateTick_Collision(fTimeDelta);

#ifndef DEBUG
	m_pGameInstance->Render_QuadTree();
#endif // !DEBUG


	return S_OK;
}

HRESULT CLevel_GamePlay::Ready_Layer_Terrain()
{
	/* 원형객체를 복제하여 사본객체를 생성하고 레이어에 추가한다. */
	CGameObject* pGameObject = nullptr;
	LAYERTAG	eLayerTag = LAYERTAG::TERRAIN;

	/*pGameObject = m_pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, eLayerTag, TEXT("Prototype_GameObject_BasicTerrain"));
	if (nullptr == pGameObject)	return E_FAIL;*/

	return S_OK;
}

HRESULT CLevel_GamePlay::Ready_Layer_Ground()
{
	/* 원형객체를 복제하여 사본객체를 생성하고 레이어에 추가한다. */
	CGameObject* pGameObject = nullptr;
	LAYERTAG	eLayerTag = LAYERTAG::GROUND;

	return S_OK;
}

HRESULT CLevel_GamePlay::Ready_Layer_Player()
{
	/* 원형객체를 복제하여 사본객체를 생성하고 레이어에 추가한다. */
	CGameObject* pGameObject = nullptr;
	LAYERTAG	eLayerTag = LAYERTAG::PLAYER;

	/*pGameObject = m_pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, eLayerTag, TEXT("Prototype_GameObject_TempCube"));
	if (nullptr == pGameObject)	return E_FAIL;*/
	
	pGameObject = m_pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, eLayerTag, TEXT("Prototype_GameObject_P_Strife"));
	if (nullptr == pGameObject)	return E_FAIL;

	return S_OK;
}

HRESULT CLevel_GamePlay::Ready_Layer_Equipment()
{
	/* 원형객체를 복제하여 사본객체를 생성하고 레이어에 추가한다. */
	CGameObject* pGameObject = nullptr;
	LAYERTAG	eLayerTag = LAYERTAG::EQUIPMENT;
	
	return S_OK;
}

HRESULT CLevel_GamePlay::Ready_Layer_Default()
{
	/* 원형객체를 복제하여 사본객체를 생성하고 레이어에 추가한다. */
	CGameObject* pGameObject = nullptr;
	LAYERTAG	eLayerTag = LAYERTAG::DEFAULT;

	return S_OK;
}

HRESULT CLevel_GamePlay::Ready_Layer_UnitGround()
{
	/* 원형객체를 복제하여 사본객체를 생성하고 레이어에 추가한다. */
	CGameObject* pGameObject = nullptr;
	LAYERTAG	eLayerTag = LAYERTAG::UNIT_GROUND;

	for (_uint i = 0; i < 1; ++i)
	{
		pGameObject = m_pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, eLayerTag, TEXT("Prototype_GameObject_CollisionTest"));
		if (nullptr == pGameObject)	return E_FAIL;
		pGameObject->GetTransform()->Translate(Vec3(-230.f + (_float)i, 100.f, -180.f - (_float)i));

		pGameObject = m_pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, eLayerTag, TEXT("Prototype_GameObject_CollisionTest"));
		if (nullptr == pGameObject)	return E_FAIL;
		pGameObject->GetTransform()->Translate(Vec3(-180.f + (_float)i, 100.f, -130.f - (_float)i));

		pGameObject = m_pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, eLayerTag, TEXT("Prototype_GameObject_CollisionTest"));
		if (nullptr == pGameObject)	return E_FAIL;
		pGameObject->GetTransform()->Translate(Vec3(-130.f + (_float)i, 100.f, -80.f - (_float)i));

		pGameObject = m_pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, eLayerTag, TEXT("Prototype_GameObject_CollisionTest"));
		if (nullptr == pGameObject)	return E_FAIL;
		pGameObject->GetTransform()->Translate(Vec3(-80.f + (_float)i, 100.f, -30.f - (_float)i));

		pGameObject = m_pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, eLayerTag, TEXT("Prototype_GameObject_CollisionTest"));
		if (nullptr == pGameObject)	return E_FAIL;
		pGameObject->GetTransform()->Translate(Vec3(-30.f + (_float)i, 100.f, 20.f - (_float)i));

		pGameObject = m_pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, eLayerTag, TEXT("Prototype_GameObject_CollisionTest"));
		if (nullptr == pGameObject)	return E_FAIL;
		pGameObject->GetTransform()->Translate(Vec3(30.f - (_float)i, 100.f, 20.f - (_float)i));

		pGameObject = m_pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, eLayerTag, TEXT("Prototype_GameObject_CollisionTest"));
		if (nullptr == pGameObject)	return E_FAIL;
		pGameObject->GetTransform()->Translate(Vec3(80.f - (_float)i, 100.f, -30.f - (_float)i));

		pGameObject = m_pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, eLayerTag, TEXT("Prototype_GameObject_CollisionTest"));
		if (nullptr == pGameObject)	return E_FAIL;
		pGameObject->GetTransform()->Translate(Vec3(130.f - (_float)i, 100.f, -80.f - (_float)i));

		pGameObject = m_pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, eLayerTag, TEXT("Prototype_GameObject_CollisionTest"));
		if (nullptr == pGameObject)	return E_FAIL;
		pGameObject->GetTransform()->Translate(Vec3(180.f - (_float)i, 100.f, -130.f - (_float)i));

		pGameObject = m_pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, eLayerTag, TEXT("Prototype_GameObject_CollisionTest"));
		if (nullptr == pGameObject)	return E_FAIL;
		pGameObject->GetTransform()->Translate(Vec3(230.f - (_float)i, 100.f, -180.f - (_float)i));
	}

	for (_int i = 0; i < 150; ++i)
	{
		pGameObject = m_pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, eLayerTag, TEXT("Prototype_GameObject_HellHound"));
		if (nullptr == pGameObject)	return E_FAIL;
		_int iRandomPosX = (rand() * i) % 1024 - 512;
		_int iRandomPosZ = (rand() * i) % 1024 - 512;
		_int iRandomAnimIndex = (abs(rand()) * i) % 344;
		pGameObject->GetModel()->SetNextAnimationIndex(iRandomAnimIndex);
		pGameObject->GetTransform()->Translate(Vec3(iRandomPosX, 0.f, iRandomPosZ));
	}

	return S_OK;
}

HRESULT CLevel_GamePlay::Ready_Layer_UnitAir()
{
	return S_OK;
}

HRESULT CLevel_GamePlay::Ready_Layer_Camera()
{
	CGameObject* pGameObject = nullptr;
	LAYERTAG	eLayerTag = LAYERTAG::CAMERA;

	pGameObject = m_pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, eLayerTag, TEXT("Prototype_GameObject_FlyingCamera"));
	if (nullptr == pGameObject)	return E_FAIL;
	pGameObject->GetTransform()->Translate(Vec3(0.f, 400.f, 0.f));

	return S_OK;
}

HRESULT CLevel_GamePlay::Ready_Layer_Wall()
{
	CGameObject* pGameObject = nullptr;
	LAYERTAG	eLayerTag = LAYERTAG::WALL;

	pGameObject = m_pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, eLayerTag, TEXT("Prototype_GameObject_StaticTest"));
	if (nullptr == pGameObject)	return E_FAIL;
	pGameObject->GetTransform()->Translate(Vec3(200.f, 300.f, 100.f));

	pGameObject = m_pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, eLayerTag, TEXT("Prototype_GameObject_StaticTest"));
	if (nullptr == pGameObject)	return E_FAIL;
	pGameObject->GetTransform()->Translate(Vec3(-200.f, 300.f, 100.f));

	pGameObject = m_pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, eLayerTag, TEXT("Prototype_GameObject_StaticTest"));
	if (nullptr == pGameObject)	return E_FAIL;
	pGameObject->GetTransform()->Translate(Vec3(250.f, 300.f, 50.f));

	pGameObject = m_pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, eLayerTag, TEXT("Prototype_GameObject_StaticTest"));
	if (nullptr == pGameObject)	return E_FAIL;
	pGameObject->GetTransform()->Translate(Vec3(-250.f, 300.f, 50.f));

	pGameObject = m_pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, eLayerTag, TEXT("Prototype_GameObject_StaticTest"));
	if (nullptr == pGameObject)	return E_FAIL;
	pGameObject->GetTransform()->Translate(Vec3(300.f, 300.f, 0.f));

	pGameObject = m_pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, eLayerTag, TEXT("Prototype_GameObject_StaticTest"));
	if (nullptr == pGameObject)	return E_FAIL;
	pGameObject->GetTransform()->Translate(Vec3(-300.f, 300.f, 0.f));
	//pGameObject->GetTransform()->Translate(Vec3(10.f, 0.f, 100.f));

	return S_OK;
}

CLevel_GamePlay * CLevel_GamePlay::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
{
	CLevel_GamePlay*	pInstance = new CLevel_GamePlay(pDevice, pContext);

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Failed to Created : CLevel_GamePlay");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CLevel_GamePlay::Free()
{
	Super::Free();

	RELEASE_INSTANCE(CGameInstance);
}
