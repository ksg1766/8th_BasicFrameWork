#include "BT_Action.h"
#include "GameInstance.h"
#include "GameObject.h"
#include "MonoBehaviour.h"

CBT_Action::CBT_Action()
{
}

CBT_Action::CBT_Action(const CBT_Action& rhs)
	:Super(rhs)
{
}

HRESULT CBT_Action::Initialize(CGameObject* pGameObject, CBehaviorTree* pBehaviorTree, const BEHAVEANIMS& tStateAnim, CMonoBehaviour* pController)
{
	if(FAILED(Super::Initialize(pGameObject, pBehaviorTree, pController)))
		return E_FAIL;

	for (const wstring& strAnim : tStateAnim.vecAnimations)
	{
		_int iAnimIndex = m_pModel->GetAnimationIndexByName(strAnim);
		_float fAnimTimes = m_pModel->GetAnimationTimeByIndex(iAnimIndex);
		if (iAnimIndex < 0)
			return E_FAIL;

		m_vecAnimIndexTime.push_back(make_pair(iAnimIndex, fAnimTimes));
	}

	return S_OK;
}

void CBT_Action::Free()
{
	Super::Free();

}