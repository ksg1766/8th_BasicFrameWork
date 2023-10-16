#include "P_Strife_State_Idle.h"

CP_Strife_State_Idle::CP_Strife_State_Idle()
	:Super()
{

}

CP_Strife_State_Idle::CP_Strife_State_Idle(const CP_Strife_State_Idle& rhs)
	:Super(rhs)
{
}

HRESULT CP_Strife_State_Idle::Enter(_int i)
{
	return S_OK;
}

void CP_Strife_State_Idle::Tick(const _float& fTimeDelta)
{
}

void CP_Strife_State_Idle::LateTick(const _float& fTimeDelta)
{
}

void CP_Strife_State_Idle::Exit()
{
}

CP_Strife_State_Idle* CP_Strife_State_Idle::Create(const wstring& strStateName, CGameObject* pGameObject, const STATEANIMS& tStateAnim)
{
	CP_Strife_State_Idle* pInstance = new CP_Strife_State_Idle();

	if (FAILED(pInstance->Initialize(strStateName, pGameObject, tStateAnim)))
	{
		MSG_BOX("Failed to Created : CPlayerController");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CP_Strife_State_Idle::Free()
{
	Super::Free();
}