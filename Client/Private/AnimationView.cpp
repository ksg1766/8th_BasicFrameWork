#include "stdafx.h"
#include "ViewMediator.h"
#include "AnimationView.h"
#include "GameInstance.h"
#include "GameObject.h"
#include <filesystem>
#include "Utils.h"
#include "FileUtils.h"

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
	AnimationGroup();
	DeleteReservedGroup();

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
	ImGui::Text("This window has some useful function for Animations.");
	ImGui::NewLine();

	ImGui::Text("Current Object :"); ImGui::SameLine(); ImGui::Text(m_strSelectedObject.c_str());
	
	ImGui::NewLine();

	if (ImGui::Button("Load Model"))
	{
		m_vecAnimationNames.clear();
		m_vecDeleteReserved.clear();

		wstring strDirectory = TEXT("../Bin/Resources/Models/Skeletal/") + Utils::ToWString(m_strSelectedObject);
		
		wstring strPrototype = TEXT("Prototype_GameObject_") + Utils::ToWString(m_strSelectedObject);
		
		m_pAnimModelObject = m_pGameInstance->CreateObject(strPrototype, LAYERTAG::DEFAULT);
		
		LoadAnimations(strDirectory);

		//

		
	}
	ImGui::SameLine();
	if (ImGui::Button("Reload VTF"))
	{
		/*if (m_pAnimModelObject)
			m_pGameInstance->DeleteObject(m_pAnimModelObject);



		wstring strPrototype = TEXT("Prototype_GameObject_") + Utils::ToWString(m_strSelectedObject);
		m_pAnimModelObject = m_pGameInstance->CreateObject(strPrototype, LAYERTAG::DEFAULT);
		
		wstring strDirectory = TEXT("../Bin/Resources/Models/Skeletal/") + Utils::ToWString(m_strSelectedObject);
		LoadAnimations(strDirectory);*/
	}
	ImGui::SameLine();
	if (ImGui::Button("Save Anim"))
	{

	}

	if (m_pAnimModelObject)
	{
		Matrix matTransformCam = m_pGameInstance->Get_Transform_float4x4_Inverse(CPipeLine::D3DTS_VIEW);
		m_pAnimModelObject->GetTransform()->SetPosition(matTransformCam.Translation() + 400.f * matTransformCam.Backward());
	}

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
			if (entry.is_directory())
			{
				string fileName = Utils::ToString(entry.path().stem());
				const _char* szSrc = fileName.c_str();
				size_t len = strlen(szSrc) + 1; // NULL 문자 포함
				_char* szCopy = new _char[len];
				strcpy_s(szCopy, len, szSrc);

				vecItems.push_back(szCopy);
			}
		}
	}

	ImGui::ListBox("Skeletal Files", &m_Item_Current, vecItems.data(), vecItems.size(), 5);
	m_strSelectedObject = string(vecItems[m_Item_Current]);
}

void CAnimationView::AnimationGroup()
{
	if (m_vecAnimationNames.empty())
		return;

	_int iCurSelected = m_Animation_Current;
	ImGui::ListBox("Animation List", &m_Animation_Current, m_vecAnimationNames.data(), m_vecAnimationNames.size(), 5);

	if (iCurSelected != m_Animation_Current)
		m_pAnimModelObject->GetModel()->SetNextAnimationIndex(m_Animation_Current);

	m_strSelectedAnimation = string(m_vecAnimationNames[m_Animation_Current]);

	if (ImGui::Button("Delete"))
	{
		m_vecDeleteReserved.push_back(m_vecAnimationNames[m_Animation_Current]);
		m_vecAnimationNames[m_Animation_Current] = "---Deleted---";
	}
}

void CAnimationView::DeleteReservedGroup()
{
	if (m_vecDeleteReserved.empty())
		return;

	ImGui::ListBox("Delete List", &m_Delete_Current, m_vecDeleteReserved.data(), m_vecDeleteReserved.size(), 5);

	
	/*if (ImGui::Button("Reset"))
	{
		for (auto& iter : m_vecAnimationNames)
		{
			for (auto& _iter : m_vecDeleteReserved)
			{
				if (S_OK == strcmp(iter, _iter))
					iter = _iter;
			}
		}
		m_vecAnimationNames.push_back(m_vecDeleteReserved[m_Delete_Current]);
		m_vecDeleteReserved.erase(m_vecDeleteReserved.begin() + m_Delete_Current);
	}*/
}

HRESULT CAnimationView::LoadAnimations(const wstring& strModelFilePath)
{
	/* 파일 셋업 */
	wstring folderName, filePath;
	shared_ptr<FileUtils> file = make_shared<FileUtils>();
	{
		folderName = filesystem::path(strModelFilePath).stem();
		filePath = (filesystem::path(strModelFilePath) / wstring(folderName + TEXT(".anim")));
		Utils::Replace(filePath, TEXT("\\"), TEXT("/"));

		if (!filesystem::exists(filePath))
			return E_FAIL;

		file->Open(filePath, FileMode::Read);
	}

	/* 모든 애니메이션 순회 */
	size_t iNumAnims = file->Read<size_t>();
	for (size_t i = 0; i < iNumAnims; i++)
	{
		file->Read<_float>(); // Duration
		file->Read<_float>(); // TickPerSecond

		/* 모든 채널 순회 */
		size_t iNumChannels = file->Read<size_t>();

		for (size_t j = 0; j < iNumChannels; j++)
		{
			string strName = file->Read<string>();

			/* 모든 키프레임 순회 */
			size_t iNumKeyframes = file->Read<size_t>();
			
			for (size_t k = 0; k < iNumKeyframes; k++)
			{
				file->Read<_float>();	// Time
				file->Read<Vec3>();		// Scale
				file->Read<Vec4>();		// Rotation
				file->Read<Vec3>();		// Position
			}

			const _char* szSrc = strName.c_str();
			size_t len = strlen(szSrc) + 1;
			_char* szCopy = new _char[len];
			strcpy_s(szCopy, len, szSrc);

			m_vecAnimationNames.push_back(szCopy);
		}
	}

	return S_OK;
}

HRESULT CAnimationView::ExportAnimations(const wstring& strModelFilePath)
{
	//shared_ptr<FileUtils> file = make_shared<FileUtils>();
	//file->Open(strModelFilePath, FileMode::Write);

	//file->Write<size_t>(_animations.size());
	//for (shared_ptr<asAnimation>& animation : _animations)
	//{
	//	//file->Write<string>(animation->name);
	//	file->Write<float>(animation->fDuration);
	//	file->Write<float>(animation->fTickPerSecond);

	//	file->Write<size_t>(animation->channels.size());
	//	for (shared_ptr<asChannel>& channel : animation->channels)
	//	{
	//		file->Write<string>(channel->name);

	//		file->Write<size_t>(channel->keyframes.size());
	//		for (shared_ptr<asKeyFrame>& keyframe : channel->keyframes)
	//		{
	//			file->Write<float>(keyframe->fTime);
	//			file->Write<Vec3>(keyframe->vScale);
	//			file->Write<Quaternion>(keyframe->quatRotation);
	//			file->Write<Vec3>(keyframe->vPosition);
	//		}
	//	}
	//}
	return S_OK;
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
