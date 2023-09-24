#include "Tool.h"
#include "ImGUIManager.h"

CTool::CTool(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: m_pDevice(pDevice)
	, m_pContext(pContext)
{
}

HRESULT CTool::Initialize(void* pArg)
{
	m_pImGUIInstance = GET_INSTANCE(CImGUIManager);

	return S_OK;
}

void CTool::Free()
{
	RELEASE_INSTANCE(CImGUIManager);
}
