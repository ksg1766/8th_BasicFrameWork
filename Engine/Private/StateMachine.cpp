#include "StateMachine.h"
#include "GameObject.h"
#include "State.h"

CStateMachine::CStateMachine(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: Super(pDevice, pContext, ComponentType::StateMachine)
{

}

CStateMachine::CStateMachine(const CStateMachine& rhs)
	:Super(rhs)
{
}

HRESULT CStateMachine::Initialize_Prototype()
{


	return S_OK;
}

HRESULT CStateMachine::Initialize(void* pArg)
{

	return S_OK;
}

void CStateMachine::Tick(const _float& fTimeDelta)
{
	if(m_pCurrentState)
		m_pCurrentState->Tick(fTimeDelta);
}

void CStateMachine::LateTick(const _float& fTimeDelta)
{
	if (m_pCurrentState)
		m_pCurrentState->Tick(fTimeDelta);
}

void CStateMachine::DebugRender()
{
}

HRESULT CStateMachine::AddState(CState* pState)
{
	STATES::iterator iter = m_hashStates.find(pState->GetName());

	if (iter != m_hashStates.end())
		return E_FAIL;

	m_hashStates.emplace(pState->GetName(), pState);

	return S_OK;
}

HRESULT CStateMachine::ChangeState(const wstring& strStateTag)
{
	STATES::iterator iter = m_hashStates.find(strStateTag);

	if (iter == m_hashStates.end())
		return E_FAIL;

	if (m_pCurrentState)
	{
		iter->second->Exit();
		Safe_Release(m_pCurrentState);
	}

	m_pCurrentState = iter->second;
	Safe_AddRef(m_pCurrentState);

	m_pCurrentState->Enter();

	return S_OK;
}

CStateMachine* CStateMachine::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CStateMachine* pInstance = new CStateMachine(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CStateMachine");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CComponent* CStateMachine::Clone(CGameObject* pGameObject, void* pArg)
{
	CStateMachine* pInstance = new CStateMachine(*this);
	pInstance->m_pGameObject = pGameObject;

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CStateMachine");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CStateMachine::Free()
{
	Super::Free();
}
