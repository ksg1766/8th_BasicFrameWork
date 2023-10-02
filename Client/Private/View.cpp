#include "stdafx.h"
#include "View.h"
#include "GameInstance.h"

CView::CView(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: m_pDevice(pDevice)
	, m_pContext(pContext)
{
	m_pDevice->AddRef();
	m_pContext->AddRef();
}

HRESULT CView::Initialize(void* pArg)
{
	m_pGameInstance = GET_INSTANCE(CGameInstance);

	return S_OK;
}

void CView::SetMediator(CViewMediator* pMediator)
{ 
	m_pMediator = pMediator;
	m_pMediator->AddRef();
}

void CView::Free()
{
	Safe_Release(m_pDevice);
	Safe_Release(m_pContext);
	Safe_Release(m_pMediator);
	RELEASE_INSTANCE(CGameInstance);
}
