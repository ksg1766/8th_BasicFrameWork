#include "MapTool.h"
#include "GameInstance.h"
#include "GameObject.h"
#include "Terrain.h"
#include "Utils.h"

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
	m_vPickedPosition = Vec3::Zero;

	return S_OK;
}

HRESULT CMapTool::Tick()
{
	Input();

	InfoView();
	TapGroups();

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
	if (ImGui::GetIO().WantCaptureMouse)
		return;

	_float fPickDistance;

	if (m_pGameInstance->Mouse_Down(DIM_LB))
	{
		if (m_IsPickingActivated)
		{
			const POINT& p = m_pGameInstance->GetMousePos();
			if (m_pTerrainBuffer->Pick(p.x, p.y, m_vPickedPosition, fPickDistance))
			{
				PlaceObject(m_ePickedLayerTag, m_strPickedObject, m_vPickedPosition);
			}
		}
	}
}

void CMapTool::InfoView()
{
	ImGui::Begin("Map Tool");

	ImGui::Text("This window has some useful function for Map Tool.");
	ImGui::NewLine();

	const POINT& p = m_pGameInstance->GetMousePos();
	ImGui::Text("Mouse Viewport Position : %d, %d", p.x, p.y);
	ImGui::NewLine();

	ImGui::Text("Picked Position : %.3f, %.3f, %.3f", m_vPickedPosition.x, m_vPickedPosition.y, m_vPickedPosition.z);
	ImGui::NewLine();

	ImGui::Text("Picking ");
	if (m_IsPickingActivated)
	{
		ImGui::SameLine();
		if (ImGui::Button("Deactivate"))
			m_IsPickingActivated = false;
	}
	else
	{
		ImGui::SameLine();
		if (ImGui::Button("Activate"))
			m_IsPickingActivated = true;
	}
	ImGui::NewLine();
}

void CMapTool::TapGroups()
{
	ImGuiTabBarFlags tab_bar_flags = ImGuiTabBarFlags_None;
	if (ImGui::BeginTabBar("Tool", tab_bar_flags))
	{
		if (ImGui::BeginTabItem("Ground"))
		{
			/*const _char* items[] = { "" };
			_int item_current = 0;
			ImGui::ListBox("Ground Objects", &item_current, items, IM_ARRAYSIZE(items), 10);
			m_ePickedLayerTag = LAYERTAG::GROUND;
			m_strPickedObject = Utils::ToWString(items[item_current]);*/
			ImGui::EndTabItem();
		}
		if (ImGui::BeginTabItem("Wall"))
		{
			const _char* items[] = { "StaticTest" };
			_int item_current = 0;
			ImGui::ListBox("Wall Objects", &item_current, items, IM_ARRAYSIZE(items), 10);
			m_ePickedLayerTag = LAYERTAG::WALL;
			m_strPickedObject = Utils::ToWString(items[item_current]);
			ImGui::EndTabItem();
		}
		ImGui::EndTabBar();
	}
	ImGui::End();
}

void CMapTool::PlaceObject(const LAYERTAG& eLayerTag, const wstring& strObjectTag, const Vec3& vPickPosition)
{
	if (eLayerTag == LAYERTAG::LAYER_END) return;

	const wstring strPrototypeTag = TEXT("Prototype_GameObject_") + strObjectTag;

	CGameObject* pGameObject = m_pGameInstance->Add_GameObject(LEVEL_GAMETOOL, eLayerTag, strPrototypeTag);
	pGameObject->GetTransform()->Translate(vPickPosition);
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
