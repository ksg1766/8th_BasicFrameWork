#pragma once
#include "Tool.h"
#include "Client_Defines.h"

BEGIN(Engine)

class CTerrain;

END

BEGIN(Client)

class CMapTool : public CTool
{
    using Super = CTool;
private:
	CMapTool(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual ~CMapTool() = default;

public:
	virtual HRESULT Initialize(void* pArg)	override;
	virtual HRESULT Tick()					override;
	virtual HRESULT LateTick()				override;
	virtual HRESULT	DebugRender()			override;

private:
	void	Input();
	void	PlaceObject(const LAYERTAG& eLayerTag, const wstring& strPrototypeTag, const Vec3& vPickPosition);

private:
	void	InfoView();
	void	TapGroups();

private:
	CTerrain*			m_pTerrainBuffer = nullptr;

	_bool				m_IsPickingActivated = false;

	Vec3				m_vPickedPosition;
	LAYERTAG			m_ePickedLayerTag = LAYERTAG::LAYER_END;
	wstring				m_strPickedObject;

public:
	static class CMapTool* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, CTerrain* m_pTerrainGrid);
	virtual void Free() override;
};

END