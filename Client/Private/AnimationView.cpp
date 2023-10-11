#include "stdafx.h"
#include "ViewMediator.h"
#include "AnimationView.h"
#include "GameInstance.h"
#include "GameObject.h"
#include <filesystem>
#include "Utils.h"

CAnimationView::CAnimationView(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:Super(pDevice, pContext)
{
}

HRESULT CAnimationView::Initialize(void* pArg)
{
	Super::Initialize(pArg);

	return S_OK;
}

HRESULT CAnimationView::Tick()
{
	Input();

	ImGui::Begin("Animations");

	InfoView();
	ItemGroup();

	ImGui::End();

	return S_OK;
}

HRESULT CAnimationView::LateTick()
{

	return S_OK;
}

HRESULT CAnimationView::DebugRender()
{

	return S_OK;
}

void CAnimationView::Input()
{
	if (ImGui::GetIO().WantCaptureMouse)
		return;

}

void CAnimationView::InfoView()
{
	ImGui::Text("This window has some useful function for Objects in Level.");
	ImGui::NewLine();

	ImGui::Text("Mouse Picking ");
	
	ImGui::NewLine();

	

	ImGui::NewLine();

	

	ImGui::NewLine();
}

void CAnimationView::ItemGroup()
{
	vector<const _char*> vecItems;
	wstring strDirectory = TEXT("../Bin/Resources/Models/Skeletal/");

	if (filesystem::exists(strDirectory) && filesystem::is_directory(strDirectory))
	{
		for (const auto& entry : filesystem::directory_iterator(strDirectory))
		{
			if (entry.is_regular_file())
			{
				string fileName = Utils::ToString(entry.path().filename());
				const _char* szSrc = fileName.c_str();
				size_t len = strlen(szSrc) + 1; // NULL 문자 포함
				_char* szCopy = new _char[len];
				strcpy_s(szCopy, len, szSrc);

				vecItems.push_back(szCopy);
			}
		}
	}

	ImGui::ListBox("Skeletal Files", &m_Item_Current, vecItems.data(), vecItems.size(), 5);
}

CAnimationView* CAnimationView::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CAnimationView* pInstance = new CAnimationView(pDevice, pContext);

	if (FAILED(pInstance->Initialize(nullptr)))
	{
		MSG_BOX("Failed to Created : CAnimationView");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CAnimationView::Free()
{
	Super::Free();
}
