#include "..\Public\Model.h"
#include "Mesh.h"'
#include "Texture.h"
#include "Utils.h"

CModel::CModel(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: Super(pDevice, pContext, ComponentType::Model)
{
}

CModel::CModel(const CModel& rhs)
	: Super(rhs)
	, m_iNumMeshes(rhs.m_iNumMeshes)
	, m_Meshes(rhs.m_Meshes)
	, m_iNumMaterials(rhs.m_iNumMaterials)
	, m_Materials(rhs.m_Materials)
{
	for (auto& Material : m_Materials)
	{
		for (size_t i = 0; i < AI_TEXTURE_TYPE_MAX; i++)
		{
			Safe_AddRef(Material.pTextures[i]);
		}
	}

	for (auto& pMesh : m_Meshes)
	{
		Safe_AddRef(pMesh);
	}
}

HRESULT CModel::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CModel::Initialize(void * pArg)
{
	return S_OK;
}

HRESULT CModel::InitializeWithFile(const wstring& strModelFilePath, _fmatrix& matPivot)
{
	/* .fbx 파일을 읽어서 aiScene을 리턶나다. */
	m_pAIScene = m_Importer.ReadFile(Utils::ToString(strModelFilePath), aiProcess_PreTransformVertices | aiProcess_ConvertToLeftHanded | aiProcessPreset_TargetRealtime_Fast/*aiProcessPreset_TargetRealtime_MaxQuality*/);
	if (nullptr == m_pAIScene)
		return E_FAIL;

	XMStoreFloat4x4(&m_matPivot, matPivot);

	/* DirectX로 그려낼 수 있도록 데이터들을 정리한다. */

	if (FAILED(Ready_Meshes()))
		return E_FAIL;

	/* 모델의 픽셀별로 정의된 재질 정보를 저장해준 텍스쳐를 읽어오자. */
	if (FAILED(Ready_Materials(strModelFilePath)))
		return E_FAIL;

	return S_OK;
}

void CModel::DebugRender()
{
}

HRESULT CModel::Render(_uint& iMeshIndex)
{
	m_Meshes[iMeshIndex]->Render();

	return S_OK;
}

HRESULT CModel::Bind_MaterialTexture(CShader* pShader, const _char* pConstantName, _uint iMeshIndex, aiTextureType eType)
{
	if (iMeshIndex >= m_iNumMeshes)
		return E_FAIL;

	_uint iMaterialIndex = m_Meshes[iMeshIndex]->Get_MaterialIndex();
	if (iMaterialIndex >= m_iNumMaterials)
		return E_FAIL;

	return m_Materials[iMaterialIndex].pTextures[eType]->Bind_ShaderResource(pShader, pConstantName, 0);
}

HRESULT CModel::Ready_Meshes()
{
	m_iNumMeshes = m_pAIScene->mNumMeshes;

	m_Meshes.reserve(m_iNumMeshes);

	for (_uint i = 0; i < m_iNumMeshes; i++)
	{
		CMesh*	pMesh = CMesh::Create(m_pDevice, m_pContext, m_pAIScene->mMeshes[i], XMLoadFloat4x4(&m_matPivot));
		if (nullptr == pMesh)
			return E_FAIL;

		m_Meshes.push_back(pMesh);
	}

	return S_OK;
}

HRESULT CModel::Ready_Materials(const wstring& strModelFilePath)
{
	m_iNumMaterials = m_pAIScene->mNumMaterials;
 	for (size_t i = 0; i < m_iNumMaterials; ++i)
	{
		aiMaterial*	pMaterials =  m_pAIScene->mMaterials[i];

		MESH_MATERIAL MeshMaterial;
		ZeroMemory(&MeshMaterial, sizeof MeshMaterial);

		for (size_t j = 0; j < AI_TEXTURE_TYPE_MAX; ++j)
		{
			//_uint iNumTexture = pMaterials->GetTextureCount((aiTextureType)j);
			aiString	strTexturePath;

			if (FAILED(pMaterials->GetTexture((aiTextureType)j, /*iNumTexture*/0, &strTexturePath)))
				continue;

			_char	szDrive[MAX_PATH] = "";
			_char	szDirectory[MAX_PATH] = "";
			_splitpath_s(Utils::ToString(strModelFilePath).c_str(), szDrive, MAX_PATH, szDirectory, MAX_PATH, nullptr, 0, nullptr, 0);

			_char	szFileName[MAX_PATH] = "";
			_char	szExt[MAX_PATH] = "";
			_splitpath_s(strTexturePath.data, nullptr, 0, nullptr, 0, szFileName, MAX_PATH, szExt, MAX_PATH);

			_char			szTmp[MAX_PATH] = "";
			strcpy_s(szTmp, szDrive);
			strcat_s(szTmp, szDirectory);
			strcat_s(szTmp, szFileName);
			strcat_s(szTmp, szExt);

			_tchar			szFullPath[MAX_PATH] = TEXT("");
			MultiByteToWideChar(CP_ACP, 0, szTmp, strlen(szTmp), szFullPath, MAX_PATH);

			MeshMaterial.pTextures[j] = CTexture::Create(m_pDevice, m_pContext, szFullPath);
			if (nullptr == MeshMaterial.pTextures[j])
				return E_FAIL;
		}

		m_Materials.push_back(MeshMaterial);
	}

	return S_OK;
}

CModel * CModel::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
{
	CModel*	pInstance = new CModel(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CModel");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CComponent * CModel::Clone(CGameObject* pGameObject, void * pArg)
{
	CModel*	pInstance = new CModel(*this);
	pInstance->m_pGameObject = pGameObject;

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CModel");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CModel::Free()
{
	Super::Free();

	for (auto& Material : m_Materials)
	{
		for (size_t i = 0; i < AI_TEXTURE_TYPE_MAX; i++)
		{
			Safe_Release(Material.pTextures[i]);
		}
	}
	m_Materials.clear();

	for (auto& pMesh : m_Meshes)
	{
		Safe_Release(pMesh);
	}
	m_Importer.FreeScene();
}
