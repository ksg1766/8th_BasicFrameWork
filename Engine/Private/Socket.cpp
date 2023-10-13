#include "..\Public\Socket.h"

CSocket::CSocket()
{
}

HRESULT CSocket::Initialize(const _char* szName)
{
	strcpy_s(m_szName, strlen(szName), szName);

	return S_OK;
}

HRESULT CSocket::Render()
{
	return S_OK;
}

HRESULT CSocket::Load_VTF(ID3D11Texture2D* pSocketTexture, ID3D11ShaderResourceView* pSocketSRV)
{
	if (!pSocketTexture || !pSocketSRV)
		return E_FAIL;

	m_pSocketTexture = pSocketTexture; m_pSocketSRV = pSocketSRV;

	Safe_AddRef(m_pSocketTexture);
	Safe_AddRef(m_pSocketSRV);

	return S_OK;
}

CSocket * CSocket::Create(const _char* szName)
{
	CSocket*			pInstance = new CSocket();

	if (FAILED(pInstance->Initialize(szName)))
	{
		MSG_BOX("Failed To Created : CSocket");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CSocket::Free()
{
	Safe_Release(m_pSocketTexture);
	Safe_Release(m_pSocketSRV);
}
