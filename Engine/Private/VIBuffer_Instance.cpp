#include "..\Public\VIBuffer_Instance.h"

CVIBuffer_Instance::CVIBuffer_Instance(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: Super(pDevice, pContext)
{

}

CVIBuffer_Instance::CVIBuffer_Instance(const CVIBuffer_Instance & rhs)
	: Super(rhs)
{
	Safe_AddRef(m_pVB);
	Safe_AddRef(m_pIB);	
}

HRESULT CVIBuffer_Instance::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CVIBuffer_Instance::Initialize(void * pArg)
{
	return S_OK;
}

HRESULT CVIBuffer_Instance::Render()
{
	ID3D11Buffer*	pVertexBuffers[] = {
		m_pVB, 		
	};

	_uint			iStrides[] = {
		m_iStride,
	};

	_uint			iOffsets[] = {
		0,
	};

	/* ���ؽ� ���۵��� �Ҵ��Ѵ�. */
	m_pContext->IASetVertexBuffers(0, m_iNumVBs, pVertexBuffers, iStrides, iOffsets);

	/* �ε��� ���۸� �Ҵ��Ѵ�. */
	m_pContext->IASetIndexBuffer(m_pIB, m_eIndexFormat, 0);

	/* �ش� �������� � ������� �׸�����. */
	m_pContext->IASetPrimitiveTopology(m_eTopology);

	/* �ε����� ����Ű�� ������ Ȱ���Ͽ� �׸���. */
	//m_pContext->DrawIndexed(m_iNumIndices, 0, 0);
	m_pContext->DrawIndexed(m_iNumPrimitives * m_iNumIndicesofPrimitive, 0, 0);

	return S_OK;
}


HRESULT CVIBuffer_Instance::Create_Buffer(_uint maxCount)
{
	//TODO:�� ��ġ��
	// 
	//if (nullptr == m_pDevice)
	//	return E_FAIL;

	//return m_pDevice->CreateBuffer(&m_BufferDesc, &m_SubResourceData, ppOut);	
	return S_OK;
}

void CVIBuffer_Instance::ClearData()
{
	m_vecData.clear();
}

void CVIBuffer_Instance::AddData(InstancingData& data)
{
	m_vecData.push_back(data);
}

void CVIBuffer_Instance::PushData()
{
	const _uint dataCount = GetCount();

	D3D11_MAPPED_SUBRESOURCE subResource;

	m_pContext->Map(m_pVB, 0, D3D11_MAP_WRITE_DISCARD, 0, &subResource);
	{
		::memcpy(subResource.pData, m_vecData.data(), sizeof(InstancingData) * dataCount);
	}
	m_pContext->Unmap(m_pVB, 0);
	//
	/*m_pContext->IASetVertexBuffers();
	m_pVB->PushData();*/
}

CVIBuffer_Instance* CVIBuffer_Instance::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CVIBuffer_Instance* pInstance = new CVIBuffer_Instance(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CVIBuffer_Instance");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CVIBuffer_Instance::Free()
{
	Super::Free();
}
