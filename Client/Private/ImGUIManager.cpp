#include "stdafx.h"
#include "ImGUIManager.h"

#include "../Public/ImGUI/imgui.h"
#include "../Public/ImGUI/imgui_impl_dx11.h"
#include "../Public/ImGUI/imgui_impl_win32.h"

IMPLEMENT_SINGLETON(CImGUIManager)

CImGUIManager::CImGUIManager()
{

}

void CImGUIManager::Initialize(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

	// Setup Dear ImGui style
	ImGui::StyleColorsDark();
	//ImGui::StyleColorsLight();

	// Setup Platform/Renderer backends
	ImGui_ImplWin32_Init(g_hWnd);
	ImGui_ImplDX11_Init(pDevice, pContext);
}

void CImGUIManager::Tick()
{
	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();
}

void CImGUIManager::Render()
{// Rendering
	ImGui::Render();
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
}

void CImGUIManager::Free()
{

}
