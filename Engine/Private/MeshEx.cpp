#include "..\Public\MeshEx.h"
#include "FileUtils.h"
#include "Utils.h"

CMeshEx::CMeshEx(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: Super(pDevice, pContext)
{
}

CMeshEx::CMeshEx(const CMeshEx& rhs)
	: Super(rhs)
	, m_iMaterialIndex(rhs.m_iMaterialIndex)
	, m_szName(rhs.m_szName)
	, m_szMaterialName(rhs.m_szMaterialName)
	, m_iBoneIndex(rhs.m_iBoneIndex)
{
}

HRESULT CMeshEx::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CMeshEx::Initialize(void * pArg)
{
	return S_OK;
}

HRESULT CMeshEx::LoadData_FromMeshFile(CModelEx::TYPE eModelType, FileUtils* pFileUtils, Matrix PivotMatrix)
{
	m_szName = Utils::ToWString(pFileUtils->Read<string>());
	m_iBoneIndex = pFileUtils->Read<int32>();

	// Material
	m_szMaterialName = Utils::ToWString(pFileUtils->Read<string>());
	m_iMaterialIndex = pFileUtils->Read<int32>();

	//VertexData
	{
		const uint32 iVTXCount = pFileUtils->Read<uint32>();

		m_iNumVBs = 1;
		m_iNumVertices = iVTXCount;
		m_iStride = sizeof(VTXANIMMODEL);

		ZeroMemory(&m_BufferDesc, sizeof(D3D11_BUFFER_DESC));
		m_BufferDesc.ByteWidth = m_iNumVertices * m_iStride;
		m_BufferDesc.Usage = D3D11_USAGE_DEFAULT;
		m_BufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		m_BufferDesc.CPUAccessFlags = 0;
		m_BufferDesc.MiscFlags = 0;
		m_BufferDesc.StructureByteStride = m_iStride;

		VTXANIMMODEL* pVertices = new VTXANIMMODEL[iVTXCount];
		ZeroMemory(pVertices, sizeof(VTXANIMMODEL) * iVTXCount);

		void* pData = pVertices;
		pFileUtils->Read(&pData, sizeof(VTXANIMMODEL) * iVTXCount);

		if (eModelType == CModelEx::TYPE::TYPE_NONANIM)
			Ready_Vertices(pVertices, PivotMatrix);

		ZeroMemory(&m_SubResourceData, sizeof(D3D11_SUBRESOURCE_DATA));
		m_SubResourceData.pSysMem = pVertices;

		if (FAILED(Super::Create_Buffer(&m_pVB)))
			return E_FAIL;

		Safe_Delete_Array(pVertices);
	}

	//IndexData
	{
		const uint32 iIDXCount = pFileUtils->Read<uint32>();

		m_iNumPrimitives = iIDXCount / 3;
		m_iIndexSizeofPrimitive = sizeof(FACEINDICES32);
		m_iNumIndicesofPrimitive = 3;
		m_eIndexFormat = DXGI_FORMAT_R32_UINT;
		m_eTopology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;

		ZeroMemory(&m_BufferDesc, sizeof(D3D11_BUFFER_DESC));
		m_BufferDesc.ByteWidth = m_iNumPrimitives * m_iIndexSizeofPrimitive;
		m_BufferDesc.Usage = D3D11_USAGE_DEFAULT;
		m_BufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
		m_BufferDesc.CPUAccessFlags = 0;
		m_BufferDesc.MiscFlags = 0;
		m_BufferDesc.StructureByteStride = 0;

		FACEINDICES32* pIndices = new FACEINDICES32[m_iNumPrimitives];
		ZeroMemory(pIndices, sizeof(FACEINDICES32) * m_iNumPrimitives);

		void* pData = pIndices;
		pFileUtils->Read(&pData, sizeof(FACEINDICES32) * m_iNumPrimitives);

		ZeroMemory(&m_SubResourceData, sizeof(D3D11_SUBRESOURCE_DATA));
		m_SubResourceData.pSysMem = pIndices;

		if (FAILED(Super::Create_Buffer(&m_pIB)))
			return E_FAIL;

		Safe_Delete_Array(pIndices);
	}

	return S_OK;
}

HRESULT CMeshEx::LoadData_FromConverter(CModelEx::TYPE eModelType, shared_ptr<asMesh> pMesh, Matrix matPivot)
{
	m_szName = Utils::ToWString(pMesh->name);
	m_iBoneIndex = pMesh->boneIndex;

	// Material
	m_szMaterialName = Utils::ToWString(pMesh->materialName);
	m_iMaterialIndex = pMesh->materialIndex;

	//VertexData
	{
		const uint32 iVTXCount = pMesh->vertices.size();

		m_iNumVBs = 1;
		m_iNumVertices = iVTXCount;
		m_iStride = sizeof(VTXANIMMODEL);

		::ZeroMemory(&m_BufferDesc, sizeof(D3D11_BUFFER_DESC));
		m_BufferDesc.ByteWidth = m_iNumVertices * m_iStride;
		m_BufferDesc.Usage = D3D11_USAGE_DEFAULT;
		m_BufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		m_BufferDesc.CPUAccessFlags = 0;
		m_BufferDesc.MiscFlags = 0;
		m_BufferDesc.StructureByteStride = m_iStride;

		VTXANIMMODEL* pVertices = new VTXANIMMODEL[iVTXCount];
		::ZeroMemory(pVertices, sizeof(VTXANIMMODEL) * iVTXCount);

		::CopyMemory(pVertices, pMesh->vertices.data(), sizeof(VTXANIMMODEL) * iVTXCount);

		if (eModelType == CModelEx::TYPE::TYPE_NONANIM)
			Ready_Vertices(pVertices, matPivot);

		::ZeroMemory(&m_SubResourceData, sizeof(D3D11_SUBRESOURCE_DATA));
		m_SubResourceData.pSysMem = pVertices;

		if (FAILED(Super::Create_Buffer(&m_pVB)))
			return E_FAIL;

		Safe_Delete_Array(pVertices);
	}

	//IndexData
	{
		const uint32 iIDXCount = pMesh->indices.size();

		m_iNumPrimitives = iIDXCount / 3;
		m_iIndexSizeofPrimitive = sizeof(FACEINDICES32);
		m_iNumIndicesofPrimitive = 3;
		m_eIndexFormat = DXGI_FORMAT_R32_UINT;
		m_eTopology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;

		::ZeroMemory(&m_BufferDesc, sizeof(D3D11_BUFFER_DESC));
		m_BufferDesc.ByteWidth = m_iNumPrimitives * m_iIndexSizeofPrimitive;
		m_BufferDesc.Usage = D3D11_USAGE_DEFAULT;
		m_BufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
		m_BufferDesc.CPUAccessFlags = 0;
		m_BufferDesc.MiscFlags = 0;
		m_BufferDesc.StructureByteStride = 0;

		FACEINDICES32* pIndices = new FACEINDICES32[m_iNumPrimitives];
		::ZeroMemory(pIndices, sizeof(FACEINDICES32) * m_iNumPrimitives);

		::CopyMemory(pIndices, pMesh->indices.data(), sizeof(FACEINDICES32) * m_iNumPrimitives);

		::ZeroMemory(&m_SubResourceData, sizeof(D3D11_SUBRESOURCE_DATA));
		m_SubResourceData.pSysMem = pIndices;

		if (FAILED(Super::Create_Buffer(&m_pIB)))
			return E_FAIL;

		Safe_Delete_Array(pIndices);
	}

	return S_OK;
}

HRESULT CMeshEx::Ready_Vertices(VTXANIMMODEL* pVertices, Matrix PivotMatrix)
{
	for (_uint i = 0; i < m_iNumVertices; ++i)
	{
		_float fY = *&pVertices[i].vPosition.y;
		*&pVertices[i].vPosition.y = -*&pVertices[i].vPosition.z;
		*&pVertices[i].vPosition.z = fY;

		XMStoreFloat3(&pVertices[i].vPosition, XMVector3TransformCoord(XMLoadFloat3(&pVertices[i].vPosition), PivotMatrix));
		XMStoreFloat3(&pVertices[i].vNormal, XMVector3TransformNormal(XMLoadFloat3(&pVertices[i].vNormal), PivotMatrix));
		XMStoreFloat3(&pVertices[i].vTangent, XMVector3TransformNormal(XMLoadFloat3(&pVertices[i].vTangent), PivotMatrix));
	}

	return S_OK;
}

CMeshEx* CMeshEx::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CMeshEx* pInstance = new CMeshEx(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed To Created : CMeshEx");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CComponent * CMeshEx::Clone(CGameObject* pGameObject, void* pArg)
{
	CMeshEx*	pInstance = new CMeshEx(*this);
	pInstance->m_pGameObject = pGameObject;

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CMeshEx");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CMeshEx::Free()
{
	Super::Free();


}
