#pragma once
#include "Base.h"
#include "Client_Defines.h"

BEGIN(Engine)

class CGameInstance;
class CGameObject;

END

BEGIN(Client)

class CViewMediator : public CBase
{
	using Super = CBase;

public:
	CViewMediator();
	virtual ~CViewMediator();

public:
	void	OnNotifiedSelected(CGameObject* pGameObject);
	void	OnNotifiedPickingOn(class CView* pSender);

	void	SetPrefabsView(class CPrefabsView* pPrefabsView);
	void	SetLayersView(class CLayersView* pLayersView);
	void	SetTransformView(class CTransformView* pTransformView);
	void	SetSaveLoadView(class CSaveLoadView* pSaveLoadView);
	void	SetAnimationView(class CAnimationView* pAnimationView);

protected:
	CGameInstance*			m_pGameInstance = nullptr;

	_bool					m_IsPickingActivated = false;

	class CPrefabsView*		m_pPrefabsView = nullptr;
	class CLayersView*		m_pLayersView = nullptr;
	class CTransformView*	m_pTransformView = nullptr;
	class CSaveLoadView*	m_pSaveLoadView = nullptr;
	class CAnimationView*	m_pAnimationView = nullptr;

public:
	virtual void Free() override;
};

END