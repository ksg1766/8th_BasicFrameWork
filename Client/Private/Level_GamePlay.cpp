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

	return S_OK;
}

HRESULT CLevel_GamePlay::Tick(const _float& fTimeDelta)
{
	return S_OK;
}

HRESULT CLevel_GamePlay::LateTick(const _float& fTimeDelta)
{
	SetWindowText(g_hWnd, TEXT("�����÷��� �����Դϴ�."));
	
	// TODO: �� ���� ���� �� ���� ��ġ�� ã���� ����....
	CGameInstance::GetInstance()->LateTick_Collision(fTimeDelta);
	
	return S_OK;
}

HRESULT CLevel_GamePlay::Ready_Layer_Terrain()
{
	/* ������ü�� �����Ͽ� �纻��ü�� �����ϰ� ���̾ �߰��Ѵ�. */
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
	/* ������ü�� �����Ͽ� �纻��ü�� �����ϰ� ���̾ �߰��Ѵ�. */
	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);

	CGameObject* pGameObject = nullptr;

	pGameObject = pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, LAYERTAG::PLAYER, TEXT("Prototype_GameObject_TempCube"));
	if (nullptr == pGameObject)	return E_FAIL;

	Safe_Release(pGameInstance);

	return S_OK;
}

HRESULT CLevel_GamePlay::Ready_Layer_Default()
{
	/* ������ü�� �����Ͽ� �纻��ü�� �����ϰ� ���̾ �߰��Ѵ�. */
	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);

	CGameObject* pGameObject = nullptr;

	pGameObject = pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, LAYERTAG::DEFAULT, TEXT("Prototype_GameObject_CollisionTest"));
	if (nullptr == pGameObject)	return E_FAIL;
	pGameObject->GetTransform()->Translate(Vec3(-230.f, 100.f, -180.f));

	pGameObject = pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, LAYERTAG::DEFAULT, TEXT("Prototype_GameObject_CollisionTest"));
	if (nullptr == pGameObject)	return E_FAIL;
	pGameObject->GetTransform()->Translate(Vec3(-180.f, 100.f, -130.f));

	pGameObject = pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, LAYERTAG::DEFAULT, TEXT("Prototype_GameObject_CollisionTest"));
	if (nullptr == pGameObject)	return E_FAIL;
	pGameObject->GetTransform()->Translate(Vec3(-130.f, 100.f, -80.f));

	pGameObject = pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, LAYERTAG::DEFAULT, TEXT("Prototype_GameObject_CollisionTest"));
	if (nullptr == pGameObject)	return E_FAIL;
	pGameObject->GetTransform()->Translate(Vec3(-80.f, 100.f, -30.f));

	pGameObject = pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, LAYERTAG::DEFAULT, TEXT("Prototype_GameObject_CollisionTest"));
	if (nullptr == pGameObject)	return E_FAIL;
	pGameObject->GetTransform()->Translate(Vec3(-30.f, 100.f, 20.f));

	pGameObject = pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, LAYERTAG::DEFAULT, TEXT("Prototype_GameObject_CollisionTest"));
	if (nullptr == pGameObject)	return E_FAIL;
	pGameObject->GetTransform()->Translate(Vec3(30.f, 100.f, 20.f));

	pGameObject = pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, LAYERTAG::DEFAULT, TEXT("Prototype_GameObject_CollisionTest"));
	if (nullptr == pGameObject)	return E_FAIL;
	pGameObject->GetTransform()->Translate(Vec3(80.f, 100.f, -30.f));

	pGameObject = pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, LAYERTAG::DEFAULT, TEXT("Prototype_GameObject_CollisionTest"));
	if (nullptr == pGameObject)	return E_FAIL;
	pGameObject->GetTransform()->Translate(Vec3(130.f, 100.f, -80.f));

	pGameObject = pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, LAYERTAG::DEFAULT, TEXT("Prototype_GameObject_CollisionTest"));
	if (nullptr == pGameObject)	return E_FAIL;
	pGameObject->GetTransform()->Translate(Vec3(180.f, 100.f, -130.f));

	pGameObject = pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, LAYERTAG::DEFAULT, TEXT("Prototype_GameObject_CollisionTest"));
	if (nullptr == pGameObject)	return E_FAIL;
	pGameObject->GetTransform()->Translate(Vec3(230.f, 100.f, -180.f));

	Safe_Release(pGameInstance);

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
