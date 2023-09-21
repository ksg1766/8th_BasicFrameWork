#include "..\Public\VIBuffer_Grid.h"

CVIBuffer_Grid::CVIBuffer_Grid(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: CVIBuffer(pDevice, pContext)
{
}

CVIBuffer_Grid::CVIBuffer_Grid(const CVIBuffer_Grid& rhs)
	: CVIBuffer(rhs)
{

}

HRESULT CVIBuffer_Grid::Initialize_Prototype()
{
	m_iStride = sizeof(VTXPOS); /* 정점하나의 크기 .*/
	m_iNumVertices = 100 * 100 * 8;
	m_iNumVBs = 1;

	VTXPOS*		pVertices = new VTXPOS[m_iNumVertices];
	::ZeroMemory(pVertices, sizeof(VTXPOS) * m_iNumVertices);

	_ushort* pIndices = new _ushort[m_iNumVertices];
	::ZeroMemory(pIndices, sizeof(_ushort) * m_iNumVertices);

	_uint index = 0;

	for (_uint i = 0; i < 100; ++i)
	{
		for (_uint j = 0; j < 100; ++j)
		{
			_float	fPositionX = (_float)i;
			_float	fPositionZ = (_float)(j+1);

			pVertices[index].vPosition = XMFLOAT3(fPositionX, 0.f, fPositionZ);
			pIndices[index] = index++;

			fPositionX = (_float)(i + 1);
			fPositionZ = (_float)(j + 1);

			pVertices[index].vPosition = XMFLOAT3(fPositionX, 0.f, fPositionZ);
			pIndices[index] = index++;

			fPositionX = (_float)(i + 1);
			fPositionZ = (_float)(j + 1);

			pVertices[index].vPosition = XMFLOAT3(fPositionX, 0.f, fPositionZ);
			pIndices[index] = index++;

			fPositionX = (_float)(i + 1);
			fPositionZ = (_float)j;

			pVertices[index].vPosition = XMFLOAT3(fPositionX, 0.f, fPositionZ);
			pIndices[index] = index++;

			fPositionX = (_float)(i + 1);
			fPositionZ = (_float)j;

			pVertices[index].vPosition = XMFLOAT3(fPositionX, 0.f, fPositionZ);
			pIndices[index] = index++;

			fPositionX = (_float)i;
			fPositionZ = (_float)j;

			pVertices[index].vPosition = XMFLOAT3(fPositionX, 0.f, fPositionZ);
			pIndices[index] = index++;

			fPositionX = (_float)i;
			fPositionZ = (_float)(j + 1);

			pVertices[index].vPosition = XMFLOAT3(fPositionX, 0.f, fPositionZ);
			pIndices[index] = index++;
		}
	}

	/* 정점버퍼와 인덱스 버퍼를 만드낟. */
	::ZeroMemory(&m_BufferDesc, sizeof m_BufferDesc);

	// m_BufferDesc.ByteWidth = 정점하나의 크기(Byte) * 정점의 갯수;
	m_BufferDesc.ByteWidth = m_iStride * m_iNumVertices;
	m_BufferDesc.Usage = D3D11_USAGE_DEFAULT; /* 정적버퍼로 할당한다. (Lock, unLock 호출 불가)*/
	m_BufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	m_BufferDesc.CPUAccessFlags = 0;
	m_BufferDesc.MiscFlags = 0;
	m_BufferDesc.StructureByteStride = m_iStride;

	::ZeroMemory(&m_SubResourceData, sizeof m_SubResourceData);
	m_SubResourceData.pSysMem = pVertices;

	if (FAILED(__super::Create_Buffer(&m_pVB)))
		return E_FAIL;
	
	Safe_Delete_Array(pVertices);

	// Index Buffer
	m_eIndexFormat = DXGI_FORMAT_R16_UINT;
	m_eTopology = D3D_PRIMITIVE_TOPOLOGY_LINELIST;

	/* 인덱스 버퍼를 만드낟. */
	::ZeroMemory(&m_BufferDesc, sizeof m_BufferDesc);
	m_BufferDesc.ByteWidth = sizeof(_ushort) * m_iNumVertices;
	m_BufferDesc.Usage = D3D11_USAGE_DEFAULT; /* 정적버퍼로 할당한다. (Lock, unLock 호출 불가)*/
	m_BufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	m_BufferDesc.CPUAccessFlags = 0;
	m_BufferDesc.MiscFlags = 0;
	m_BufferDesc.StructureByteStride = 0;

	::ZeroMemory(&m_SubResourceData, sizeof m_SubResourceData);
	m_SubResourceData.pSysMem = pIndices;

	if (FAILED(__super::Create_Buffer(&m_pIB)))
		return E_FAIL;

	Safe_Delete_Array(pIndices);

	return S_OK;
}

HRESULT CVIBuffer_Grid::Initialize(void * pArg)
{
	return S_OK;
}

CVIBuffer_Grid* CVIBuffer_Grid::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
{
	CVIBuffer_Grid*	pInstance = new CVIBuffer_Grid(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CVIBuffer_Grid");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CComponent * CVIBuffer_Grid::Clone(CGameObject* pGameObject, void * pArg)
{
	CVIBuffer_Grid*	pInstance = new CVIBuffer_Grid(*this);
	pInstance->m_pGameObject = pGameObject;

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CVIBuffer_Grid");
		Safe_Release(pInstance);
	}

	return pInstance;
}


void CVIBuffer_Grid::Free()
{
	__super::Free();


}