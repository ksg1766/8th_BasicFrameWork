#include "stdafx.h"
#include "..\Public\Loader.h"

#include <filesystem>
#include "GameInstance.h"
#include "BackGround.h"
//
#include "BasicTerrain.h"
#include "TempCube.h"
#include "CollisionTest.h"
#include "PlayerController.h"
#include "MonsterController.h"
#include "MonsterStats.h"
#include "MainCameraController.h"
#include "StateMachine.h"
#include "BehaviorTree.h"
#include "TestAIController.h"
#include "FlyingCamera.h"
#include "MainCamera.h"
#include "StaticTest.h"
#include "StaticBase.h"
#include "P_Strife.h"
#include "HellHound.h"
#include "Strife_GunL.h"
#include "Strife_GunR.h"
#include "Strife_Ammo_Default.h"
#include "Strife_Ammo_Static.h"
#include "Particle.h"
#include "ParticleController.h"

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
	/* DX�� ����ϴ� ��� �� ��ü�� �ʱ�ȭ�Ѵ�.  */	
	CoInitializeEx(nullptr, 0);

	/* ���Ӱ� ������ �����尡 ���Ѵ�. */
	CLoader*		pLoader = (CLoader*)pArg;

	pLoader->Loading();

	return 0;
}

HRESULT CLoader::Initialize(LEVELID eNextLevel)
{
	InitializeCriticalSection(&m_Critical_Section);

	m_eNextLevel = eNextLevel;

	/* ���ο� �����带 ������. */
	/* ������ : �� �ڵ带 �о��ִ� �ϳ��� �帧? */
	/* 3 : ������ �����尡 ȣ���ؾ��ϴ� �Լ��� �ּ� */
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
	/* For.Texture */
	m_strLoading = TEXT("�ؽ��ĸ� �ε� �� �Դϴ�.");

	Loading_Components_For_Level_Logo();
	Loading_Scripts_For_Level_Logo();

	/* For.Mesh */
	m_strLoading = TEXT("�޽ø� �ε� �� �Դϴ�.");
	

	/* For.Shader */
	m_strLoading = TEXT("���̴��� �ε� �� �Դϴ�.");

	/* For.GameObject */
	m_strLoading = TEXT("��ü������ �ε� �� �Դϴ�.");
	Loading_GameObjects_For_Level_Logo();

	m_strLoading = TEXT("�ε� ��.");
	m_isFinished = true;

	return S_OK;
}

HRESULT CLoader::Loading_Components_For_Level_Logo()
{
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	/* For.Prototype_Component_Texture_BackGround */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_LOGO, TEXT("Prototype_Component_Texture_BackGround"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/Default%d.jpg"), 2))))
		return E_FAIL;

	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

HRESULT CLoader::Loading_Scripts_For_Level_Logo()
{
	return S_OK;
}

HRESULT CLoader::Loading_GameObjects_For_Level_Logo()
{
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	/* For.Prototype_GameObject_BackGround */
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_BackGround"), CBackGround::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

HRESULT CLoader::Loading_For_Level_GamePlay()
{
	/* For.Texture */
	m_strLoading = TEXT("�ؽ��ĸ� �ε� �� �Դϴ�.");

	Loading_Components_For_Level_GamePlay();
	Loading_Scripts_For_Level_GamePlay();

	/* For.Mesh */
	m_strLoading = TEXT("�޽ø� �ε� �� �Դϴ�.");

	/* For.Shader */
	m_strLoading = TEXT("���̴��� �ε� �� �Դϴ�.");

	/* For.GameObject */
	m_strLoading = TEXT("��ü������ �ε� �� �Դϴ�.");

	Loading_GameObjects_For_Level_GamePlay();

	m_strLoading = TEXT("�ε� ��.");
	m_isFinished = true;

	return S_OK;
}

HRESULT CLoader::Loading_Components_For_Level_GamePlay()
{
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	/* For.Prototype_Component_Texture_Sky */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Sky"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/SkyBox/Sky_%d.dds"), 4))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_Terrain*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Terrain"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/Terrain/Tile0.jpg")))))
		return E_FAIL;
	
	/* For.Prototype_Component_Texture_Strife_Ammo_Default*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Strife_Ammo_Default"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/Effect/Ammo/Strife_Ammo_Default_%d.png"), 4))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_Strife_Ammo_Static*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Strife_Ammo_Static"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/Effect/Ammo/Strife_Ammo_Static_%d.png"), 9))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_Strife_Muzzle_Default*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Strife_Muzzle_Default"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/Effect/Ammo/Strife_Muzzle_%d.png"), 8))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_FlameMask*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_FlameMask"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/Effect/Particles/FlameMask_%d.png"), 4))))
		return E_FAIL;

	/* For.Prototype_Component_StateMachine */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_StateMachine"),
		CStateMachine::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_Component_StateMachine */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_BehaviorTree"),
		CBehaviorTree::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_Component_NavMeshAgent */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_NavMeshAgent"),
		CNavMeshAgent::Create(m_pDevice, m_pContext, TEXT("../Bin/LevelData/NavMesh/NavigationMesh.dat")))))
		return E_FAIL;

	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

HRESULT CLoader::Loading_Scripts_For_Level_GamePlay()
{
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	/* For.Prototype_Component_PlayerController*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_PlayerController"),
		CPlayerController::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_Component_MonsterController*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_MonsterController"),
		CMonsterController::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_Component_ParticleController*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_ParticleController"),
		CParticleController::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_Component_MonsterStats*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_MonsterStats"),
		CMonsterStats::Create(m_pDevice, m_pContext))))
		return E_FAIL;


	/* For.Prototype_Component_MainCameraController*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_MainCameraController"),
		CMainCameraController::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_Component_TestAIController*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_TestAIController"),
		CTestAIController::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

HRESULT CLoader::Loading_GameObjects_For_Level_GamePlay()
{
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	/* For.Prototype_GameObject_Terrain */
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_BasicTerrain"), CBasicTerrain::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_GameObject_TempCube */
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_TempCube"), CTempCube::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_GameObject_CollisionTest */
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_CollisionTest"), CCollisionTest::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_GameObject_FlyingCamera */
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_FlyingCamera"), CFlyingCamera::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_GameObject_FlyingCamera */
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_MainCamera"), CMainCamera::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_GameObject_StaticTest */
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_StaticTest"), CStaticTest::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_GameObject_P_Strife */
  	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_P_Strife"), CP_Strife::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	
	/* For.Prototype_GameObject_HellHound */
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_HellHound"), CHellHound::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	
	/* For.Prototype_GameObject_Strife_GunL */
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Strife_GunL"), CStrife_GunL::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	
	/* For.Prototype_GameObject_Strife_GunR */
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Strife_GunR"), CStrife_GunR::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_GameObject_Strife_Ammo_Default */
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Strife_Ammo_Default"), CStrife_Ammo_Default::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	
	/* For.Prototype_GameObject_Strife_Ammo_Static */
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Strife_Ammo_Static"), CStrife_Ammo_Static::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_GameObject_Particle */
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Particle"), CParticle::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_GameObject_Static */
	wstring strStaticFilePath = TEXT("../Bin/Resources/Models/Static/");
	for (const auto& entry : filesystem::directory_iterator(strStaticFilePath))
	{
		const wstring& strFileName = entry.path().stem();

		if (strFileName == TEXT("Strife_GunL") || strFileName == TEXT("Strife_GunR"))
			continue;

		if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_") + strFileName, CStaticBase::Create(m_pDevice, m_pContext))))
			return E_FAIL;
	}

	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

HRESULT CLoader::Loading_For_Level_GameTool()
{
	/* For.Texture */
	m_strLoading = TEXT("�ؽ��ĸ� �ε� �� �Դϴ�.");

	Loading_Components_For_Level_GameTool();
	Loading_Scripts_For_Level_GameTool();

	/* For.Mesh */
	m_strLoading = TEXT("�޽ø� �ε� �� �Դϴ�.");

	/* For.Shader */
	m_strLoading = TEXT("���̴��� �ε� �� �Դϴ�.");

	/* For.GameObject */
	m_strLoading = TEXT("��ü������ �ε� �� �Դϴ�.");

	Loading_GameObjects_For_Level_GameTool();

	m_strLoading = TEXT("�ε� ��.");
	m_isFinished = true;

	return S_OK;
}

HRESULT CLoader::Loading_Components_For_Level_GameTool()
{
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	/* For.Prototype_Component_Texture_Sky */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMETOOL, TEXT("Prototype_Component_Texture_Sky"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/SkyBox/Sky_%d.dds"), 4))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_Terrain*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMETOOL, TEXT("Prototype_Component_Texture_Terrain"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/Terrain/Tile0.jpg")))))
		return E_FAIL;

	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

HRESULT CLoader::Loading_Scripts_For_Level_GameTool()
{
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);



	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

HRESULT CLoader::Loading_GameObjects_For_Level_GameTool()
{
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	/* For.Prototype_GameObject_Terrain*/
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_BasicTerrain"), CBasicTerrain::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_GameObject_FlyingCamera */
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_FlyingCamera"), CFlyingCamera::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_GameObject_FlyingCamera */
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_StaticTest"), CStaticTest::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_GameObject_Static */
	wstring strStaticFilePath = TEXT("../Bin/Resources/Models/Static/");
	for (const auto& entry : filesystem::directory_iterator(strStaticFilePath))
	{
		const wstring& strFileName = entry.path().stem();

		if (strFileName == TEXT("Strife_GunL") || strFileName == TEXT("Strife_GunR"))
			continue;

		if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_") + strFileName, CStaticBase::Create(m_pDevice, m_pContext))))
			return E_FAIL;
	}

	/* For.Prototype_GameObject_P_Strife */
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_P_Strife"), CP_Strife::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_GameObject_HellHound */
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Strife_GunL"), CStrife_GunL::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_GameObject_HellHound */
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Strife_GunR"), CStrife_GunR::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_GameObject_HellHound */
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_HellHound"), CHellHound::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_GameObject_Strife_Ammo_Default */
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Strife_Ammo_Default"), CStrife_Ammo_Default::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	RELEASE_INSTANCE(CGameInstance);

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