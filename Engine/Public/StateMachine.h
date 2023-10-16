#pragma once
#include "Component.h"
#include "Hasher.h"

BEGIN(Engine)

class CState;
class ENGINE_DLL CStateMachine final : public CComponent
{
	using Super = CComponent;

private:
	CStateMachine(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CStateMachine(const CStateMachine& rhs);
	virtual ~CStateMachine() = default;
	
public:
	virtual HRESULT Initialize_Prototype()				override;
	virtual HRESULT Initialize(void* pArg)				override;
	virtual void	Tick(const _float& fTimeDelta)		override;
	virtual void	LateTick(const _float& fTimeDelta)	override;

	virtual void	DebugRender()						override;

public:
	CState* GetCurState() const { return m_pCurrentState; }
	HRESULT ChangeState(const wstring& strStateTag);
	HRESULT AddState(CState* pState);

private:
	using STATES = unordered_map<wstring, CState*, djb2Hasher>;
	STATES	m_hashStates;

	CState* m_pCurrentState = nullptr;

public:
	static CStateMachine* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CComponent* Clone(CGameObject* pGameObject, void* pArg)	override;
	virtual void Free() override;
};

END