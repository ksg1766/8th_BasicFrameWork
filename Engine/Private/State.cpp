#include "State.h"
#include "GameObject.h"
#include "Model.h"

CState::CState()
{

}

CState::CState(const CState& rhs)
	:Super(rhs)
{
}

HRESULT CState::Initialize(const wstring& strStateName, CGameObject* pGameObject, const STATEANIMS& tStateAnim)
{
	m_strStateName = strStateName;
	m_pGameObject = pGameObject;

	for (const wstring& strAnim : tStateAnim.vecAnimations)
	{
		_int iAnimIndex = pGameObject->GetModel()->GetAnimationIndexByName(strAnim);
		if (iAnimIndex < 0)
			return E_FAIL;

		m_vecAnimaIndices.push_back(iAnimIndex);
	}

	return S_OK;
}

HRESULT CState::Enter(_int i)
{
	m_pGameObject->GetModel()->SetNextAnimationIndex(i);	// TODO: 루프 설정 아직 안함

	return S_OK;
}

void CState::Free()
{
	Super::Free();
}