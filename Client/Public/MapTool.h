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

private:
	CTerrain*			m_pTerrainBuffer = nullptr;

	Vec3				m_vPickPosition;

public:
	static class CMapTool* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, CTerrain* m_pTerrainGrid);
	virtual void Free() override;
};

END