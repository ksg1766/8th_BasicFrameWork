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

void CBT_Action::OnStart(const _float& fTimeDelta)
{
}

CBT_Node::BT_RETURN CBT_Action::OnUpdate(const _float& fTimeDelta)
{
}

void CBT_Action::OnEnd(const _float& fTimeDelta)
{
}

void CBT_Action::Free()
{
	Super::Free();
}