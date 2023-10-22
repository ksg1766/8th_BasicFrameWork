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
		_int			iCurrentIndex = { -1 };
	}NAVIGATION_DESC;

private:
	CNavMeshAgent(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CNavMeshAgent(const CNavMeshAgent& rhs);
	virtual ~CNavMeshAgent() = default;

public:
	virtual HRESULT		Initialize_Prototype(const wstring& strNavigationData);
	virtual HRESULT		Initialize(void* pArg)				override;
	virtual void		Tick(const _float& fTimeDelta)		override;
	virtual void		DebugRender()						override;
	
public:
	_bool	Walkable(_fvector vPoint);
	void	ForceHeight() { m_pTransform->Translate(Vec3(0.f, GetHeightOffset(), 0.f)); }
	_float	GetHeightOffset();

private:
	HRESULT SetUp_Neighbors();

private:
	CTransform*				m_pTransform = nullptr;
	_int					m_iCurrentIndex = { -1 };
	static vector<class CCell*>	m_Cells;

#ifdef _DEBUG
	static _bool	m_IsRendered;

	PrimitiveBatch<VertexPositionColor>* m_pBatch = nullptr;
	BasicEffect* m_pEffect = nullptr;
	ID3D11InputLayout* m_pInputLayout = nullptr;
#endif

public:
	static CNavMeshAgent* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strNavigationData);
	virtual CComponent* Clone(CGameObject* pGameObject, void* pArg) override;
	virtual void Free() override;
};

END