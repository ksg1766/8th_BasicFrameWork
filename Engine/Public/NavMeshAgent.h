#pragma once
#include "Component.h"

BEGIN(Engine)

class CNavMeshSurface;
class ENGINE_DLL CNavMeshAgent final : public CComponent
{
	using Super = CComponent;
public:
	typedef struct tagNavigationDesc
	{
		CNavMeshSurface* pNavMeshSurface = nullptr;
		_int			iCurrentIndex = { -1 };
	}NAVIGATION_DESC;

private:
	CNavMeshAgent(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CNavMeshAgent(const CNavMeshAgent& rhs);
	virtual ~CNavMeshAgent() = default;

public:
	virtual HRESULT		Initialize_Prototype(const wstring& strNavigationData);
	virtual HRESULT		Initialize(void* pArg)	override;
	virtual void		DebugRender()			override;
	
public:
	_bool	IsMove(_fvector vPoint);

private:
	HRESULT SetUp_Neighbors();

private:
	_int					m_iCurrentIndex = { -1 };
	CNavMeshSurface*		m_pNavMeshSurface = nullptr;
	static vector<class CCell*>	m_Cells;

#ifdef _DEBUG
	_bool	m_bRender = false;
	class CShader* m_pShader = { nullptr };
#endif

public:
	static CNavMeshAgent* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strNavigationData);
	virtual CComponent* Clone(CGameObject* pGameObject, void* pArg) override;
	virtual void Free() override;
};

END