#pragma once
#include "View.h"
#include "Client_Defines.h"

BEGIN(Engine)

class CGameObject;
class CCell;

END

BEGIN(Client)

class CNavMeshView : public CView
{
    using Super = CView;
private:
	CNavMeshView(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual ~CNavMeshView() = default;

public:
	virtual HRESULT Initialize(void* pArg)	override;
	virtual HRESULT Tick()					override;
	virtual HRESULT LateTick()				override;
	virtual HRESULT	DebugRender()			override;

private:
	void	Input();
	_bool	Pick(_uint screenX, _uint screenY, Vec3& pickPos, _float& distance);

	void	AddCell(CCell* pCell) { m_vecCells.push_back(pCell); }
	//void	AddPoint(Vec3 vPoint) { m_arrPoints}

private:
	void	InfoView();
	void	PointGroup();
	void	CellGroup();


private:
	wstring				m_strPickedObject;
	CGameObject*		m_pPickedObject = nullptr;

	_int				m_Item_Current = 0;

	vector<CCell*>		m_vecCells;
	vector<_char*>		m_strCells;
	// cache
	vector<Vec3>		m_vecPoints;
	vector<const _char*>m_strPoints;

public:
	static class CNavMeshView* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual void Free() override;
};

END