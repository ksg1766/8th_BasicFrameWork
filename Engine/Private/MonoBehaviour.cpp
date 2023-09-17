#include "MonoBehaviour.h"

CMonoBehaviour::CMonoBehaviour(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: Super(pDevice, pContext, ComponentType::Script)
{

}

CMonoBehaviour::CMonoBehaviour(const CMonoBehaviour& rhs)
	:Super(rhs)
{
}

HRESULT CMonoBehaviour::Initialize_Prototype()
{
	return E_NOTIMPL;
}

HRESULT CMonoBehaviour::Initialize(void* pArg)
{
	return E_NOTIMPL;
}

void CMonoBehaviour::Tick(const _float& fTimeDelta)
{
}

void CMonoBehaviour::LateTick(const _float& fTimeDelta)
{
}

void CMonoBehaviour::DebugRender()
{
}

CMonoBehaviour* CMonoBehaviour::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CMonoBehaviour* pInstance = new CMonoBehaviour(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CMonoBehaviour");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CComponent* CMonoBehaviour::Clone(void* pArg)
{
	return nullptr;
}

void CMonoBehaviour::Free()
{
}
