#pragma once

#include "Client_Defines.h"
#include "Level.h"

BEGIN(Client)

class CLevel_GameTool final : public CLevel
{
	using Super = CLevel;
private:
	CLevel_GameTool(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual ~CLevel_GameTool() = default;

public:
	virtual HRESULT Initialize()						override;
	virtual HRESULT Tick(const _float& fTimeDelta)		override;
	virtual HRESULT LateTick(const _float& fTimeDelta)	override;
	virtual HRESULT	DebugRender()						override;

	void			ImGUIDemo();

private:
	HRESULT			Ready_Layer_Terrain();
	HRESULT			Ready_Layer_Camera();
	HRESULT			Ready_Layer_Wall();

	HRESULT			Ready_Tools();

	class CMapTool*			m_pMapTool = nullptr;

	class CBasicTerrain*	m_pBasicTerrain = nullptr;

private:
	class CImGUIManager* m_pImGUIInstance;

	_bool	m_IsImGUIReady = false;
	_bool	show_demo_window = true;
	_bool	show_another_window = false;
	Vec4	clear_color = Vec4(0.f);

public:
	static class CLevel_GameTool* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual void Free() override;
};

END