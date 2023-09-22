#include "stdafx.h"
#include "..\Public\Level_GamePlay.h"
#include "ImGUIManager.h"

#include "GameInstance.h"
#include "GameObject.h"

CLevel_GamePlay::CLevel_GamePlay(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: CLevel(pDevice, pContext)
{
}

HRESULT CLevel_GamePlay::Initialize()
{
	if (FAILED(Ready_Layer_Terrain()))
		return E_FAIL;

	if (FAILED(Ready_Layer_Player()))
		return E_FAIL;

	if (FAILED(Ready_Layer_Default()))
		return E_FAIL;
	
	if (FAILED(Ready_Layer_Camera()))
		return E_FAIL;

	return S_OK;
}

HRESULT CLevel_GamePlay::Tick(const _float& fTimeDelta)
{
	return S_OK;
}

HRESULT CLevel_GamePlay::LateTick(const _float& fTimeDelta)
{
	SetWindowText(g_hWnd, TEXT("게임플레이 레벨입니다."));
	
	// TODO: 꼭 영기 말고 더 좋은 위치를 찾도록 하자....
	CGameInstance::GetInstance()->LateTick_Collision(fTimeDelta);
	
	return S_OK;
}

HRESULT CLevel_GamePlay::Ready_Layer_Terrain()
{
	/* 원형객체를 복제하여 사본객체를 생성하고 레이어에 추가한다. */
	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);

	CGameObject* pGameObject = nullptr;

	pGameObject = pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, LAYERTAG::TERRAIN, TEXT("Prototype_GameObject_BasicTerrain"));
	if (nullptr == pGameObject)	return E_FAIL;

	Safe_Release(pGameInstance);

	return S_OK;
}

HRESULT CLevel_GamePlay::Ready_Layer_Player()
{
	/* 원형객체를 복제하여 사본객체를 생성하고 레이어에 추가한다. */
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);
	Safe_AddRef(pGameInstance);

	CGameObject* pGameObject = nullptr;

	pGameObject = pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, LAYERTAG::PLAYER, TEXT("Prototype_GameObject_TempCube"));
	if (nullptr == pGameObject)	return E_FAIL;

	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

HRESULT CLevel_GamePlay::Ready_Layer_Default()
{
	/* 원형객체를 복제하여 사본객체를 생성하고 레이어에 추가한다. */
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	CGameObject* pGameObject = nullptr;

	for (_uint i = 0; i < 1; ++i)
	{
		pGameObject = pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, LAYERTAG::DEFAULT, TEXT("Prototype_GameObject_CollisionTest"));
		if (nullptr == pGameObject)	return E_FAIL;
		pGameObject->GetTransform()->Translate(Vec3(-230.f + (_float)i, 100.f, -180.f - (_float)i));

		pGameObject = pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, LAYERTAG::DEFAULT, TEXT("Prototype_GameObject_CollisionTest"));
		if (nullptr == pGameObject)	return E_FAIL;
		pGameObject->GetTransform()->Translate(Vec3(-180.f + (_float)i, 100.f, -130.f - (_float)i));

		pGameObject = pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, LAYERTAG::DEFAULT, TEXT("Prototype_GameObject_CollisionTest"));
		if (nullptr == pGameObject)	return E_FAIL;
		pGameObject->GetTransform()->Translate(Vec3(-130.f + (_float)i, 100.f, -80.f - (_float)i));

		pGameObject = pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, LAYERTAG::DEFAULT, TEXT("Prototype_GameObject_CollisionTest"));
		if (nullptr == pGameObject)	return E_FAIL;
		pGameObject->GetTransform()->Translate(Vec3(-80.f + (_float)i, 100.f, -30.f - (_float)i));

		pGameObject = pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, LAYERTAG::DEFAULT, TEXT("Prototype_GameObject_CollisionTest"));
		if (nullptr == pGameObject)	return E_FAIL;
		pGameObject->GetTransform()->Translate(Vec3(-30.f + (_float)i, 100.f, 20.f - (_float)i));

		pGameObject = pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, LAYERTAG::DEFAULT, TEXT("Prototype_GameObject_CollisionTest"));
		if (nullptr == pGameObject)	return E_FAIL;
		pGameObject->GetTransform()->Translate(Vec3(30.f - (_float)i, 100.f, 20.f - (_float)i));

		pGameObject = pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, LAYERTAG::DEFAULT, TEXT("Prototype_GameObject_CollisionTest"));
		if (nullptr == pGameObject)	return E_FAIL;
		pGameObject->GetTransform()->Translate(Vec3(80.f - (_float)i, 100.f, -30.f - (_float)i));

		pGameObject = pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, LAYERTAG::DEFAULT, TEXT("Prototype_GameObject_CollisionTest"));
		if (nullptr == pGameObject)	return E_FAIL;
		pGameObject->GetTransform()->Translate(Vec3(130.f - (_float)i, 100.f, -80.f - (_float)i));

		pGameObject = pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, LAYERTAG::DEFAULT, TEXT("Prototype_GameObject_CollisionTest"));
		if (nullptr == pGameObject)	return E_FAIL;
		pGameObject->GetTransform()->Translate(Vec3(180.f - (_float)i, 100.f, -130.f - (_float)i));

		pGameObject = pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, LAYERTAG::DEFAULT, TEXT("Prototype_GameObject_CollisionTest"));
		if (nullptr == pGameObject)	return E_FAIL;
		pGameObject->GetTransform()->Translate(Vec3(230.f - (_float)i, 100.f, -180.f - (_float)i));
	}

	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

HRESULT CLevel_GamePlay::Ready_Layer_Camera()
{
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	CGameObject* pGameObject = nullptr;

	pGameObject = pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, LAYERTAG::CAMERA, TEXT("Prototype_GameObject_FlyingCamera"));
	if (nullptr == pGameObject)	return E_FAIL;
	pGameObject->GetTransform()->Translate(Vec3(0.f, 400.f, 0.f));


	RELEASE_INSTANCE(CGameInstance);

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


}
