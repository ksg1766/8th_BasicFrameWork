#include "BT_Abort.h"
#include "GameInstance.h"
#include "GameObject.h"
#include "MonoBehaviour.h"

CBT_Abort::CBT_Abort()
{
}

CBT_Abort::CBT_Abort(const CBT_Abort& rhs)
	:Super(rhs)
{
}

HRESULT CBT_Abort::Initialize(CGameObject* pGameObject, CMonoBehaviour* pController, AbortType eAbortType)
{
	Super::Initialize(pGameObject, pController);
	m_eAbortType = eAbortType;

	return S_OK;
}

void CBT_Abort::OnStart(const _float& fTimeDelta)
{
}

CBT_Node::BT_RETURN CBT_Abort::OnUpdate(const _float& fTimeDelta)
{
}

void CBT_Abort::OnEnd(const _float& fTimeDelta)
{
}

void CBT_Abort::Free()
{
	Super::Free();
}