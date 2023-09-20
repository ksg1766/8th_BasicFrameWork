#include "stdafx.h"
#include "..\Public\Loader.h"

#include "GameInstance.h"
#include "BackGround.h"
//
#include "BasicTerrain.h"
#include "TempCube.h"
#include "CollisionTest.h"
#include "PlayerController.h"
#include "TestAIController.h"

CLoader::CLoader(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: m_pDevice(pDevice)
	, m_pContext(pContext)
{
	Safe_AddRef(m_pDevice);
	Safe_AddRef(m_pContext);
}

// unsigned(__stdcall* _beginthreadex_proc_type)(void*);

_uint APIENTRY ThreadEntry(void* pArg)
{
	/* DX가 사용하는 모든 컴 객체를 초기화한다.  */	
	CoInitializeEx(nullptr, 0);

	/* 새롭게 생성된 스레드가 일한다. */
	CLoader*		pLoader = (CLoader*)pArg;

	pLoader->Loading();

	return 0;
}

HRESULT CLoader::Initialize(LEVELID eNextLevel)
{
	InitializeCriticalSection(&m_Critical_Section);

	m_eNextLevel = eNextLevel;

	/* 새로운 스레드를 만들자. */
	/* 스레드 : 내 코드를 읽어주는 하나의 흐름? */
	/* 3 : 생성한 스레드가 호출해야하는 함수의 주소 */
	m_hThread = (HANDLE)_beginthreadex(nullptr, 0, ThreadEntry, this, 0, nullptr);
	if (0 == m_hThread)
		return E_FAIL;

	return S_OK;
}

_int CLoader::Loading()
{
	EnterCriticalSection(&m_Critical_Section);

	HRESULT		hr = 0;

	switch (m_eNextLevel)
	{
	case LEVEL_LOGO:
		hr = Loading_For_Level_Logo();
		break;
	case LEVEL_GAMEPLAY:
		hr = Loading_For_Level_GamePlay();
		break;
	case LEVEL_GAMETOOL:
		hr = Loading_For_Level_GameTool();
		break;
	}

	if (FAILED(hr))
		return -1;	

	LeaveCriticalSection(&m_Critical_Section);

	return 0;	
}

HRESULT CLoader::Loading_For_Level_Logo()
{
	CGameInstance*		pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);

	/* For.Texture */
	m_strLoading = TEXT("텍스쳐를 로딩 중 입니다.");

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_LOGO, TEXT("Prototype_Component_Texture_BackGround"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/Default%d.jpg"), 2))))
		return E_FAIL;
	

	/* For.Mesh */
	m_strLoading = TEXT("메시를 로딩 중 입니다.");
	

	/* For.Shader */
	m_strLoading = TEXT("셰이더를 로딩 중 입니다.");

	/* For.GameObject */
	m_strLoading = TEXT("객체원형을 로딩 중 입니다.");

	/* For.Prototype_GameObject_BackGround */
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_BackGround"), CBackGround::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	
	Safe_Release(pGameInstance);

	m_strLoading = TEXT("로딩 끝.");
	m_isFinished = true;

	return S_OK;
}

HRESULT CLoader::Loading_For_Level_GamePlay()
{
	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);

	/* For.Texture */
	m_strLoading = TEXT("텍스쳐를 로딩 중 입니다.");

	/* For.Prototype_Component_Texture_Sky */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Sky"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/SkyBox/Sky_%d.dds"), 4))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_Terrain*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Terrain"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/Terrain/Tile0.jpg")))))
		return E_FAIL;

	/* For.Prototype_Component_PlayerController*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_PlayerController"),
		CPlayerController::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	
	/* For.Prototype_Component_TestAIController*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_TestAIController"),
		CTestAIController::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	///* For.Prototype_Component_SphereCollider */
	//if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_SphereCollider"),
	//	CSphereCollider::Create(m_pDevice, m_pContext))))
	//	return E_FAIL;

	///* For.Prototype_Component_OBBCollider */
	//if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_OBBCollider"),
	//	COBBCollider::Create(m_pDevice, m_pContext))))
	//	return E_FAIL;

	/* For.Mesh */
	m_strLoading = TEXT("메시를 로딩 중 입니다.");

	/* For.Shader */
	m_strLoading = TEXT("셰이더를 로딩 중 입니다.");

	/* For.GameObject */
	m_strLoading = TEXT("객체원형을 로딩 중 입니다.");

	/* For.Prototype_GameObject_Terrain*/
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_BasicTerrain"), CBasicTerrain::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_GameObject_TempCube */
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_TempCube"), CTempCube::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_GameObject_CollisionTest */
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_CollisionTest"), CCollisionTest::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	Safe_Release(pGameInstance);

	m_strLoading = TEXT("로딩 끝.");
	m_isFinished = true;

	return S_OK;
}

HRESULT CLoader::Loading_For_Level_GameTool()
{
	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);

	/* For.Texture */
	m_strLoading = TEXT("텍스쳐를 로딩 중 입니다.");

	/* For.Prototype_Component_Texture_Sky */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMETOOL, TEXT("Prototype_Component_Texture_Sky"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/SkyBox/Sky_%d.dds"), 4))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_Terrain*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMETOOL, TEXT("Prototype_Component_Texture_Terrain"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/Terrain/Tile0.jpg")))))
		return E_FAIL;

	/* For.Mesh */
	m_strLoading = TEXT("메시를 로딩 중 입니다.");

	/* For.Shader */
	m_strLoading = TEXT("셰이더를 로딩 중 입니다.");

	/* For.GameObject */
	m_strLoading = TEXT("객체원형을 로딩 중 입니다.");

	/* For.Prototype_GameObject_Terrain*/
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_BasicTerrain"), CBasicTerrain::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	Safe_Release(pGameInstance);

	m_strLoading = TEXT("로딩 끝.");
	m_isFinished = true;

	return S_OK;
}

CLoader * CLoader::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext, LEVELID eNextLevel)
{
	CLoader*	pInstance = new CLoader(pDevice, pContext);

	if (FAILED(pInstance->Initialize(eNextLevel)))
	{
		MSG_BOX("Failed to Created : CLoader");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CLoader::Free()
{
	__super::Free();

	WaitForSingleObject(m_hThread, INFINITE);	

	DeleteCriticalSection(&m_Critical_Section);

	CloseHandle(m_hThread);

	Safe_Release(m_pDevice);
	Safe_Release(m_pContext);
}