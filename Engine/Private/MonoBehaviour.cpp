#include "MonoBehaviour.h"

CMonoBehaviour::CMonoBehaviour(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: Super(pDevice, pContext, ComponentType::Script)
{

}

CMonoBehaviour::CMonoBehaviour(const CMonoBehaviour& rhs)
	:Super(rhs)
{
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

void CMonoBehaviour::Free()
{
}
