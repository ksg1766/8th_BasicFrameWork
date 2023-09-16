#include "..\Public\GameInstance.h"
#include "TimerManager.h"
#include "GraphicDevice.h"
#include "LevelManager.h"
#include "ObjectManager.h"

IMPLEMENT_SINGLETON(CGameInstance)

CGameInstance::CGameInstance()
	: m_pTimerManager(CTimerManager::GetInstance())
	, m_pGraphicDevice(CGraphicDevice::GetInstance())
	, m_pLevelManager(CLevelManager::GetInstance())
	, m_pObjectManager(CObjectManager::GetInstance())
	, m_pComponentManager(CComponentManager::GetInstance())
{
	Safe_AddRef(m_pComponentManager);
	Safe_AddRef(m_pObjectManager);
	Safe_AddRef(m_pLevelManager);
	Safe_AddRef(m_pGraphicDevice);
	Safe_AddRef(m_pTimerManager);
}

HRESULT CGameInstance::Initialize_Engine(_uint iNumLevels, const GRAPHIC_DESC& GraphicDesc, _Inout_ ID3D11Device** ppDevice, _Inout_ ID3D11DeviceContext** ppContext)
{
	/* 그래픽디바이스 초기화 처리. */
	if (FAILED(m_pGraphicDevice->Ready_GraphicDevice(GraphicDesc.hWnd, GraphicDesc.eWinMode, GraphicDesc.iWinSizeX, GraphicDesc.iWinSizeY, ppDevice, ppContext)))
		return E_FAIL;

	/* 사운드디바이스 초기화 처리. */
	/* 입력디바이스 초기화 처리. */

	/* 오브젝트 매니져의 예약 처리. */
	if (FAILED(m_pObjectManager->Reserve_Manager(iNumLevels)))
		return E_FAIL;

	/* 컴포넌트 매니져의 예약 처리. */
	if (FAILED(m_pComponentManager->Reserve_Manager(iNumLevels)))
		return E_FAIL;

	return S_OK;
}

void CGameInstance::Tick(_float fTimeDelta)
{
	m_pObjectManager->Tick(fTimeDelta);
	m_pLevelManager->Tick(fTimeDelta);

	m_pObjectManager->LateTick(fTimeDelta);
	m_pLevelManager->LateTick(fTimeDelta);
}

void CGameInstance::DebugRender()
{
	// m_pObjectManager->DebugRender();
	m_pLevelManager->DebugRender();
}

void CGameInstance::Clear(_uint iLevelIndex)
{
	m_pObjectManager->Clear(iLevelIndex);
	// m_pComponent_Manager->Clear(iLevelIndex);
}

_float CGameInstance::Compute_TimeDelta(const wstring & strTimerTag)
{
	if (nullptr == m_pTimerManager)
		return 0.f;

	return m_pTimerManager->Compute_TimeDelta(strTimerTag);	
}

HRESULT CGameInstance::Add_Timer(const wstring & strTimerTag)
{
	if (nullptr == m_pTimerManager)
		return E_FAIL;

	return m_pTimerManager->Add_Timer(strTimerTag);
}

HRESULT CGameInstance::Clear_BackBuffer_View(_float4 vClearColor)
{
	if (nullptr == m_pGraphicDevice)
		return E_FAIL;
	return m_pGraphicDevice->Clear_BackBuffer_View(vClearColor);
}

HRESULT CGameInstance::Clear_DepthStencil_View()
{

	if (nullptr == m_pGraphicDevice)
		return E_FAIL;
	return m_pGraphicDevice->Clear_DepthStencil_View();
}

HRESULT CGameInstance::Present()
{
	if (nullptr == m_pGraphicDevice)
		return E_FAIL;

	return m_pGraphicDevice->Present();
}

HRESULT CGameInstance::Open_Level(_uint iLevelIndex, CLevel * pNewLevel)
{
	if (nullptr == m_pLevelManager)
		return E_FAIL;
	return m_pLevelManager->Open_Level(iLevelIndex, pNewLevel);
}

HRESULT CGameInstance::Add_Prototype(const wstring & strPrototypeTag, CGameObject * pPrototype)
{
	if (nullptr == m_pObjectManager)
		return E_FAIL;

	return m_pObjectManager->Add_Prototype(strPrototypeTag, pPrototype);	
}

HRESULT CGameInstance::Add_GameObject(_uint iLevelIndex, const LAYERTAG& eLayerTag, const wstring & strPrototypeTag, void * pArg)
{
	if (nullptr == m_pObjectManager)
		return E_FAIL;

	return m_pObjectManager->Add_GameObject(iLevelIndex, eLayerTag, strPrototypeTag, pArg);
}

HRESULT CGameInstance::Add_Prototype(_uint iLevelIndex, const wstring& strPrototypeTag, CComponent * pPrototype)
{
	if (nullptr == m_pComponentManager)
		return E_FAIL;

	return m_pComponentManager->Add_Prototype(iLevelIndex, strPrototypeTag, pPrototype);
}

CComponent * CGameInstance::Clone_Component(_uint iLevelIndex, const wstring& strPrototypeTag, void * pArg)
{
	if (nullptr == m_pComponentManager)
		return nullptr;

	return m_pComponentManager->Clone_Component(iLevelIndex, strPrototypeTag, pArg);
}

void CGameInstance::Release_Engine()
{
	CGameInstance::GetInstance()->DestroyInstance();
	CLevelManager::GetInstance()->DestroyInstance();
	CObjectManager::GetInstance()->DestroyInstance();
	CComponentManager::GetInstance()->DestroyInstance();
	CTimerManager::GetInstance()->DestroyInstance();		
	CGraphicDevice::GetInstance()->DestroyInstance();
}

void CGameInstance::Free()
{
	Safe_Release(m_pComponentManager);
	Safe_Release(m_pObjectManager);
	Safe_Release(m_pLevelManager);
	Safe_Release(m_pGraphicDevice);
	Safe_Release(m_pTimerManager);
}
