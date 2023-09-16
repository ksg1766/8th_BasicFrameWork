#include "stdafx.h"
#include "..\Public\Level_GameTool.h"
#include "ImGUIManager.h"

#include "GameInstance.h"

CLevel_GameTool::CLevel_GameTool(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: CLevel(pDevice, pContext)
{
}

HRESULT CLevel_GameTool::Initialize()
{ 
	CImGUIManager::GetInstance()->Initialize(m_pDevice, m_pContext);
	
 	if (FAILED(Ready_Layer_Terrain(LAYERTAG::TERRAIN)))
		return E_FAIL;

	return S_OK;
}

HRESULT CLevel_GameTool::Tick(_float fTimeDelta)
{
	// UI
	if (!m_IsImGUIReady)
		m_IsImGUIReady = true;
	
	CImGUIManager::GetInstance()->Tick();
	Demo();

	return S_OK;
}

HRESULT CLevel_GameTool::LateTick(_float fTimeDelta)
{
	SetWindowText(g_hWnd, TEXT("게임 툴 레벨입니다."));

	return S_OK;
}

HRESULT CLevel_GameTool::DebugRender()
{
	if(m_IsImGUIReady)
		CImGUIManager::GetInstance()->Render();

	return S_OK;
}

HRESULT CLevel_GameTool::Ready_Layer_Terrain(const LAYERTAG& eLayerTag)
{
	/* 원형객체를 복제하여 사본객체를 생성하고 레이어에 추가한다. */
	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);

	if (FAILED(pGameInstance->Add_GameObject(LEVEL_GAMETOOL, eLayerTag, TEXT("Prototype_GameObject_BasicTerrain"))))
		return E_FAIL;

	Safe_Release(pGameInstance);

	return S_OK;
}

void CLevel_GameTool::Demo()
{
	// 1. Show the big demo window (Most of the sample code is in ImGui::ShowDemoWindow()! You can browse its code to learn more about Dear ImGui!).
	if (show_demo_window)
		ImGui::ShowDemoWindow(&show_demo_window);

	// 2. Show a simple window that we create ourselves. We use a Begin/End pair to create a named window.
	{
		static float f = 0.0f;
		static int counter = 0;

		ImGui::Begin("Hello, world!");                          // Create a window called "Hello, world!" and append into it.

		ImGui::Text("This is some useful text.");               // Display some text (you can use a format strings too)
		ImGui::Checkbox("Demo Window", &show_demo_window);      // Edit bools storing our window open/close state
		ImGui::Checkbox("Another Window", &show_another_window);

		ImGui::SliderFloat("float", &f, 0.0f, 1.0f);            // Edit 1 float using a slider from 0.0f to 1.0f
		ImGui::ColorEdit3("clear color", (float*)&clear_color); // Edit 3 floats representing a color

		if (ImGui::Button("Button"))                            // Buttons return true when clicked (most widgets return true when edited/activated)
			counter++;
		ImGui::SameLine();
		ImGui::Text("counter = %d", counter);

		//ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);
		ImGui::End();
	}

	// 3. Show another simple window.
	if (show_another_window)
	{
		ImGui::Begin("Another Window", &show_another_window);   // Pass a pointer to our bool variable (the window will have a closing button that will clear the bool when clicked)
		ImGui::Text("Hello from another window!");
		if (ImGui::Button("Close Me"))
			show_another_window = false;
		ImGui::End();
	}
}

CLevel_GameTool* CLevel_GameTool::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
{
	CLevel_GameTool*	pInstance = new CLevel_GameTool(pDevice, pContext);

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Failed to Created : CLevel_GameTool");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CLevel_GameTool::Free()
{
	Super::Free();


}
