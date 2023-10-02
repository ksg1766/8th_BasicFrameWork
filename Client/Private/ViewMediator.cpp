#include "stdafx.h"
#include "ViewMediator.h"
#include "PrefabsView.h"
#include "LayersView.h"
#include "TransformView.h"

CViewMediator::CViewMediator()
{
}

CViewMediator::~CViewMediator()
{
}

void CViewMediator::OnNotifiedSelected(CGameObject* pGameObject)
{
	m_pTransformView->SetObject(pGameObject);
}

//void CViewMediator::OnNotifiedTransform(CTransform* pTransform)
//{
//	//Safe_Release(m_pTransformView);
//	//m_pTransformView->SetTransform(pTransform);
//}

void CViewMediator::SetPrefabsView(CPrefabsView* pPrefabsView)
{ 
	m_pPrefabsView = pPrefabsView;
	m_pPrefabsView->AddRef();
	m_pPrefabsView->SetMediator(this);
}

void CViewMediator::SetLayersView(CLayersView* pLayersView)
{ 
	m_pLayersView = pLayersView;
	m_pLayersView->AddRef();
	m_pLayersView->SetMediator(this);
}

void CViewMediator::SetTransformView(CTransformView* pTransformView)
{ 
	m_pTransformView = pTransformView;
	m_pTransformView->AddRef();
	m_pTransformView->SetMediator(this);
}

void CViewMediator::Free()
{
	Safe_Release(m_pPrefabsView);
	Safe_Release(m_pLayersView);
	Safe_Release(m_pTransformView);
}
