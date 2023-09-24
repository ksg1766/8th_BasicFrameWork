#include "MapTool.h"
#include "GameInstance.h"
#include "Terrain.h"

#include "../Public/ImGUI/imgui.h"
#include "../Public/ImGUI/imgui_impl_dx11.h"
#include "../Public/ImGUI/imgui_impl_win32.h"

CMapTool::CMapTool(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:Super(pDevice, pContext)
{
}

HRESULT CMapTool::Initialize(void* pArg)
{
	Super::Initialize(pArg);

	m_pTerrainBuffer = reinterpret_cast<CTerrain*>(pArg);
	m_vPickPosition = Vec3::Zero;

	return S_OK;
}

HRESULT CMapTool::Tick()
{
	Input();

	ImGui::Begin("Map Tool");

	ImGui::Text("This window has some useful function for Map Tool.");
	ImGui::NewLine();
	ImGui::Text("%.3f", m_vPickPosition.x);
	ImGui::Text("%.3f", m_vPickPosition.y);
	ImGui::Text("%.3f", m_vPickPosition.z);

	ImGui::End();

	return S_OK;
}

HRESULT CMapTool::LateTick()
{


	return S_OK;
}

HRESULT CMapTool::DebugRender()
{
	return S_OK;
}

void CMapTool::Input()
{
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	_float fPickDistance;

	if (pGameInstance->Mouse_Down(DIM_LB))
	{
		m_pTerrainBuffer->Pick(g_iWinSizeX, g_iWinSizeY, m_vPickPosition, fPickDistance);
	}

	RELEASE_INSTANCE(CGameInstance);
}

CMapTool* CMapTool::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, CTerrain* m_pTerrainBf)
{
	CMapTool* pInstance = new CMapTool(pDevice, pContext);

	if (FAILED(pInstance->Initialize(m_pTerrainBf)))
	{
		MSG_BOX("Failed to Created : CMapTool");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CMapTool::Free()
{
	Super::Free();
}
