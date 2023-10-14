#include "..\Public\Socket.h"
#include "GameObject.h"
#include "Model.h"
#include "Shader.h"
#include "Transform.h"

CSocket::CSocket()
{
}

HRESULT CSocket::Initialize(const _int iBoneIndex)
{
	m_iBoneIndex = iBoneIndex;

	return S_OK;
}

HRESULT CSocket::Equip(CModel* pParts)
{
	if(m_pPartsModel)
		return E_FAIL;

	m_pPartsModel = const_cast<CModel*>(pParts);
	m_pPartsModel->SetSRV(m_pSocketSRV);
	
	return S_OK;
}

HRESULT CSocket::UnEquip()
{
	if (!m_pPartsModel)
		return E_FAIL;

	m_pPartsModel = nullptr;

	return S_OK;
}

HRESULT CSocket::LoadSRV(ID3D11ShaderResourceView*& pSocketSRV)
{
	if (m_pSocketSRV)
		return E_FAIL;

	m_pSocketSRV = pSocketSRV;

	return S_OK;
}

HRESULT CSocket::LoadTweenDescFromBone(TWEENDESC& tweenDec)
{
	m_pPartsModel->SetTweenDesc(tweenDec);

	return S_OK;
}

HRESULT CSocket::LoadTrasformFromBone(Matrix& matSocketWorld)
{
	const Matrix& matWorld = m_pPartsModel->GetTransform()->WorldMatrix();
	m_pPartsModel->GetTransform()->Set_WorldMatrix(matSocketWorld * matWorld);

	return S_OK;
}

HRESULT CSocket::BindBoneIndex()
{
	return m_pPartsModel->GetGameObject()->GetShader()->Bind_RawValue("g_iSocketBoneIndex", &m_iBoneIndex, sizeof(_int));
}

CSocket* CSocket::Create(const _int iBoneIndex)
{
	CSocket* pInstance = new CSocket();

	if (FAILED(pInstance->Initialize(iBoneIndex)))
	{
		MSG_BOX("Failed To Created : CSocket");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CSocket::Free()
{
	Safe_Release(m_pSocketSRV);
}
