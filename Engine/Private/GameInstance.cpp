#include "..\Public\GameInstance.h"
#include "TimerManager.h"
#include "GraphicDevice.h"
#include "InputDevice.h"
#include "QuadTree.h"
#include "KeyManager.h"
#include "LevelManager.h"
#include "ObjectManager.h"
#include "CollisionManager.h"
#include "EventManager.h"
#include "PoolManager.h"

IMPLEMENT_SINGLETON(CGameInstance)

CGameInstance::CGameInstance()
	: m_pGraphicDevice(CGraphicDevice::GetInstance())
	, m_pTimerManager(CTimerManager::GetInstance())
	, m_pInputDevice(CInputDevice::GetInstance())
	, m_pKeyManager(CKeyManager::GetInstance())
	, m_pEventManager(CEventManager::GetInstance())
	, m_pComponentManager(CComponentManager::GetInstance())
	, m_pObjectManager(CObjectManager::GetInstance())
	, m_pLevelManager(CLevelManager::GetInstance())
	, m_pQuadTree(CQuadTree::GetInstance())
	, m_pCollisionManager(CCollisionManager::GetInstance())
	, m_pPoolManager(CPoolManager::GetInstance())
	, m_pPipeLine(CPipeLine::GetInstance())
{
	Safe_AddRef(m_pGraphicDevice);
	Safe_AddRef(m_pTimerManager);
	Safe_AddRef(m_pInputDevice);
	Safe_AddRef(m_pKeyManager);
	Safe_AddRef(m_pEventManager);
	Safe_AddRef(m_pComponentManager);
	Safe_AddRef(m_pObjectManager);
	Safe_AddRef(m_pLevelManager);
	Safe_AddRef(m_pQuadTree);
	Safe_AddRef(m_pCollisionManager);
	Safe_AddRef(m_pPoolManager);
	Safe_AddRef(m_pPipeLine);
}

HRESULT CGameInstance::Initialize_Engine(_uint iNumLevels, const GRAPHIC_DESC& GraphicDesc, _Inout_ ID3D11Device** ppDevice, _Inout_ ID3D11DeviceContext** ppContext, _In_ HINSTANCE hInst)
{
	/* 그래픽 디바이스 초기화 처리. */
	if (FAILED(m_pGraphicDevice->Ready_GraphicDevice(GraphicDesc.hWnd, GraphicDesc.eWinMode, GraphicDesc.iWinSizeX, GraphicDesc.iWinSizeY, ppDevice, ppContext)))
		return E_FAIL;

	/* 사운드 디바이스 초기화 처리. */


	/* 입력 디바이스 초기화 처리. */
	if (FAILED(m_pInputDevice->Ready_InputDevice(hInst, GraphicDesc.hWnd)))
		return E_FAIL;

	/* 키 매니져의 예약 처리. */
	if (FAILED(m_pKeyManager->Reserve_Manager(GraphicDesc.hWnd)))
		return E_FAIL;

	/* 컴포넌트 매니져의 예약 처리. */
	if (FAILED(m_pComponentManager->Reserve_Manager(iNumLevels)))
		return E_FAIL;

	/* 오브젝트 매니져의 예약 처리. */
	if (FAILED(m_pObjectManager->Reserve_Manager(iNumLevels)))
		return E_FAIL;

	/* 콜리전 매니져의 예약 처리. */
	if (FAILED(m_pCollisionManager->Reserve_Manager(iNumLevels)))
		return E_FAIL;
	
	/* 콜리전 매니져의 예약 처리. */
	if (FAILED(m_pPipeLine->Initialize()))
		return E_FAIL;

	return S_OK;
}

void CGameInstance::Tick(const _float& fTimeDelta)
{
	m_pInputDevice->Update_InputDevice();
	m_pKeyManager->Tick(fTimeDelta);
	m_pObjectManager->Tick(fTimeDelta);
	m_pLevelManager->Tick(fTimeDelta);

	m_pPipeLine->Tick();

	m_pObjectManager->LateTick(fTimeDelta);
	m_pLevelManager->LateTick(fTimeDelta);
	//m_pCollisionManager->LateTick_Collision(fTimeDelta);
}

void CGameInstance::DebugRender()
{
	// m_pObjectManager->DebugRender();
	m_pLevelManager->DebugRender();
#ifdef _DEBUG
	//if (2 == m_pLevelManager->GetCurrentLevelIndex())
	//	m_pQuadTree->Render_QuadTree();
#endif // DEBUG

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

HRESULT CGameInstance::Build_QuadTree(_uint iNumLevels)
{
	if (nullptr == m_pQuadTree)
		return E_FAIL;

	return m_pQuadTree->Build_QuadTree(iNumLevels);
}

void CGameInstance::Update_QuadTree()
{
	m_pQuadTree->Update_QuadTree();
}

void CGameInstance::Render_QuadTree()
{
	m_pQuadTree->Render_QuadTree();
}

HRESULT CGameInstance::Open_Level(_uint iLevelIndex, CLevel * pNewLevel)
{
	if (nullptr == m_pLevelManager)
		return E_FAIL;

	return m_pLevelManager->Open_Level(iLevelIndex, pNewLevel);
}

_uint CGameInstance::GetCurrentLevelIndex()
{
	return m_pLevelManager->GetCurrentLevelIndex();
}


void CGameInstance::SetCurrentLevelIndex(_uint iLevelIndex)
{
	m_pLevelManager->SetCurrentLevelIndex(iLevelIndex);
}

HRESULT CGameInstance::Add_Prototype(const wstring & strPrototypeTag, CGameObject * pPrototype)
{
	if (nullptr == m_pObjectManager)
		return E_FAIL;

	return m_pObjectManager->Add_Prototype(strPrototypeTag, pPrototype);	
}

CGameObject* CGameInstance::Add_GameObject(_uint iLevelIndex, const LAYERTAG& eLayerTag, const wstring & strPrototypeTag, void * pArg)
{
	if (nullptr == m_pObjectManager)
		return nullptr;

	return m_pObjectManager->Add_GameObject(iLevelIndex, eLayerTag, strPrototypeTag, pArg);
}

map<LAYERTAG, CLayer*>* CGameInstance::GetCurrentLevelLayers()
{
	if (nullptr == m_pObjectManager)
		return nullptr;

	return m_pObjectManager->GetCurrentLevelLayers();
}

HRESULT CGameInstance::Add_Prototype(_uint iLevelIndex, const wstring& strPrototypeTag, CComponent * pPrototype)
{
	if (nullptr == m_pComponentManager)
		return E_FAIL;

	return m_pComponentManager->Add_Prototype(iLevelIndex, strPrototypeTag, pPrototype);
}

CComponent * CGameInstance::Clone_Component(CGameObject* pGameObject, _uint iLevelIndex, const wstring& strPrototypeTag, void * pArg)
{
	if (nullptr == m_pComponentManager)
		return nullptr;

	return m_pComponentManager->Clone_Component(pGameObject, iLevelIndex, strPrototypeTag, pArg);
}

void CGameInstance::FinalTick()
{
	m_pEventManager->FinalTick();
}

CGameObject* CGameInstance::CreateObject(const wstring& strPrototypeTag, const LAYERTAG& eLayer, void* pArg)
{
	return m_pEventManager->CreateObject(strPrototypeTag, eLayer, pArg);
}

void CGameInstance::DeleteObject(CGameObject* pObj)
{
	m_pEventManager->DeleteObject(pObj);
}

void CGameInstance::LevelChange(CLevel* pLevel, _uint iLevelId)
{
	m_pEventManager->LevelChange(pLevel, iLevelId);
}

void CGameInstance::LateTick_Collision(const _float& fTimeDelta)
{
	m_pCollisionManager->LateTick_Collision(fTimeDelta);
}

HRESULT CGameInstance::Reserve_Pool(const wstring& strObjectName, const _uint& iReserveCount, void* pArg)
{
	if (nullptr == m_pPoolManager)
		return E_FAIL;

	return m_pPoolManager->Reserve_Pool(strObjectName, iReserveCount, pArg);
}

_bool CGameInstance::Key_Down(_ubyte byKeyID)
{
	return m_pInputDevice->Key_Down(byKeyID);
}

_bool CGameInstance::Key_Pressing(_ubyte byKeyID)
{
	return m_pInputDevice->Key_Pressing(byKeyID);
}

_bool CGameInstance::Key_Up(_ubyte byKeyID)
{
	return m_pInputDevice->Key_Up(byKeyID);
}

_bool CGameInstance::Mouse_Down(MOUSEKEYSTATE eMouseKeyID)
{
	return m_pInputDevice->Mouse_Down(eMouseKeyID);
}

_bool CGameInstance::Mouse_Pressing(MOUSEKEYSTATE eMouseKeyID)
{
	return m_pInputDevice->Mouse_Pressing(eMouseKeyID);
}

_bool CGameInstance::Mouse_Up(MOUSEKEYSTATE eMouseKeyID)
{
	return m_pInputDevice->Mouse_Up(eMouseKeyID);
}

_ubyte CGameInstance::Get_DIKeyState(_ubyte eKeyID)
{
	return m_pInputDevice->Get_DIKeyState(eKeyID);
}

_ubyte CGameInstance::Get_DIMouseState(MOUSEKEYSTATE eMouseKeyID)
{
	return m_pInputDevice->Get_DIMouseState(eMouseKeyID);
}

_long CGameInstance::Get_DIMouseMove(MOUSEMOVESTATE eMouseMoveID)
{
	return m_pInputDevice->Get_DIMouseMove(eMouseMoveID);
}

_bool CGameInstance::Get_AnyKeyDown()
{
	return m_pInputDevice->Get_AnyKeyDown();
}

KEYSTATE CGameInstance::GetKeyState(KEY _eKey)
{
	return m_pKeyManager->GetKeyState(_eKey);
}

const POINT& CGameInstance::GetMousePos()
{
	return m_pKeyManager->GetMousePos();
}

HRESULT CGameInstance::Bind_TransformToShader(CShader* pShader, const _char* pConstantName, CPipeLine::TRANSFORMSTATE eState)
{
	return m_pPipeLine->Bind_TransformToShader(pShader, pConstantName, eState);
}

_float4 CGameInstance::Get_CamPosition_Float4() const
{
	return m_pPipeLine->Get_CamPosition_Float4();
}

_vector CGameInstance::Get_CamPosition_Vector() const
{
	return m_pPipeLine->Get_CamPosition_Vector();
}


void CGameInstance::Release_Engine()
{
	CGameInstance::GetInstance()->DestroyInstance();
	CLevelManager::GetInstance()->DestroyInstance();
	CObjectManager::GetInstance()->DestroyInstance();
	CComponentManager::GetInstance()->DestroyInstance();
	CTimerManager::GetInstance()->DestroyInstance();		
	CGraphicDevice::GetInstance()->DestroyInstance();
	CQuadTree::GetInstance()->DestroyInstance();
	CInputDevice::GetInstance()->DestroyInstance();
	CKeyManager::GetInstance()->DestroyInstance();
	CCollisionManager::GetInstance()->DestroyInstance();
	CEventManager::GetInstance()->DestroyInstance();
	CPoolManager::GetInstance()->DestroyInstance();
	CPipeLine::GetInstance()->DestroyInstance();
}

void CGameInstance::Free()
{
	Safe_Release(m_pComponentManager);
	Safe_Release(m_pObjectManager);
	Safe_Release(m_pLevelManager);
	Safe_Release(m_pGraphicDevice);
	Safe_Release(m_pTimerManager);
	Safe_Release(m_pInputDevice);
	Safe_Release(m_pKeyManager);
	Safe_Release(m_pQuadTree);
	Safe_Release(m_pCollisionManager);
	Safe_Release(m_pEventManager);
	Safe_Release(m_pPoolManager);
	Safe_Release(m_pPipeLine);
}
