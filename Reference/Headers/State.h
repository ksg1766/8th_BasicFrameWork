#pragma once
#include "Base.h"
#include "Hasher.h"

BEGIN(Engine)

class CGameObject;
class ENGINE_DLL CState : public CBase
{
	using Super = CBase;
public:
	typedef struct tagStateAnimations
	{
		vector<wstring>	vecAnimations;
	}STATEANIMS;

protected:
	CState();
	CState(const CState& rhs);
	virtual ~CState() = default;
	
public:
	virtual HRESULT	Initialize(const wstring& strStateName, CGameObject* pGameObject, const STATEANIMS& tStateAnim);
	virtual HRESULT	Enter(_int = 0);

	virtual void	Tick(const _float& fTimeDelta)		PURE;
	virtual void	LateTick(const _float& fTimeDelta)	PURE;

	virtual void	Exit()								PURE;

	const wstring&	GetName()							{ return m_strStateName; };

protected:
	CGameObject*	m_pGameObject = nullptr;
	wstring			m_strStateName = TEXT("");
	
	vector<_int>	m_vecAnimaIndices;

public:
	virtual void Free() override;
};

END