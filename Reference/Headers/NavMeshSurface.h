#pragma once
#include "MonoBehaviour.h"
#include "NavMeshAgent.h"

BEGIN(Engine)

class ENGINE_DLL CNavMeshSurface final : public CMonoBehaviour
{
	using Super = CMonoBehaviour;
private:
	CNavMeshSurface(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CNavMeshSurface(const CNavMeshSurface& rhs);
	virtual ~CNavMeshSurface() = default;
	                                                                                                 
public:
	virtual HRESULT		Initialize_Prototype(const wstring& strNavigationData);
	virtual HRESULT		Initialize(void* pArg)				override;
	virtual void		DebugRender()						override;
	
public:
	void Update(_fmatrix WorldMatrix);

private:
	vector<class CCell*>	m_Cells;
	_float4x4				m_WorldMatrix;
	friend _bool CNavMeshAgent::IsMove(_fvector vPoint);

#ifdef _DEBUG
	class CShader*			m_pShader = { nullptr };
#endif
private:
	HRESULT SetUp_Neighbors();

public:
	static CNavMeshSurface* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strNavigationData);
	virtual CComponent* Clone(CGameObject* pGameObject, void* pArg) override;
	virtual void Free() override;
};

END