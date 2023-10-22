#include "..\Public\CameraManager.h"
#include "Level.h"
#include "GameInstance.h"
#include "GameObject.h"

IMPLEMENT_SINGLETON(CCameraManager)

CCameraManager::CCameraManager()
{
}

HRESULT CCameraManager::Reserve_Manager(_uint iNumLevels)
{
	return S_OK;
}

void CCameraManager::Tick(const _float& fTimeDelta)
{
	if (nullptr == m_pCurrentCamera)
		return;

	m_pCurrentCamera->Tick(fTimeDelta);
}

void CCameraManager::LateTick(const _float& fTimeDelta)
{
	if (nullptr == m_pCurrentCamera)
		return;

	m_pCurrentCamera->LateTick(fTimeDelta);

	if (KEY_PRESSING_EX(KEY::CTRL) && KEY_DOWN_EX(KEY::F9))
		ChangeCamera(TEXT("FlyingCamera"));
	else if (KEY_PRESSING_EX(KEY::CTRL) && KEY_DOWN_EX(KEY::F8))
		ChangeCamera(TEXT("MainCamera"));
}

void CCameraManager::DebugRender()
{
	if (nullptr == m_pCurrentCamera)
		return;

	m_pCurrentCamera->DebugRender();
}

HRESULT CCameraManager::AddCamera(const wstring& strName, CGameObject* pCamera)
{
	CameraHash::iterator iter;
	iter = m_hashCamera.find(strName);

	if (iter == m_hashCamera.end())
	{
		m_hashCamera.emplace(strName, pCamera);
		return S_OK;
	}

	return E_FAIL;
}

HRESULT CCameraManager::DeleteCamera(const wstring& strName)
{
	CameraHash::iterator iter;
	iter = m_hashCamera.find(strName);

	if (iter == m_hashCamera.end())
		return E_FAIL;

	m_hashCamera.erase(iter);

	return E_FAIL;
}

HRESULT CCameraManager::ChangeCamera(const wstring& strName)
{
	CameraHash::iterator iter;
	iter = m_hashCamera.find(strName);

	if (iter == m_hashCamera.end())
		return E_FAIL;

	m_pCurrentCamera = iter->second;

	return S_OK;
}

void CCameraManager::Free()
{
	Safe_Release(m_pCurrentCamera);
	Super::Free();
}
