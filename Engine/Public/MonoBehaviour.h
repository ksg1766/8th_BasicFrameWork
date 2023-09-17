#pragma once
#include "Component.h"

BEGIN(Engine)

class ENGINE_DLL CMonoBehaviour final : public CComponent
{
	using Super = CComponent;

private:
	CMonoBehaviour(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CMonoBehaviour(const CMonoBehaviour& rhs);
	virtual ~CMonoBehaviour() = default;
	
public:
	virtual HRESULT Initialize_Prototype()				override;
	virtual HRESULT Initialize(void* pArg)				override;	// Start

	virtual void	Tick(const _float& fTimeDelta)		override;
	virtual void	LateTick(const _float& fTimeDelta)	override;
	//virtual HRESULT FixedUpdate(const _float& fTimeDelta)	override;

	virtual void	DebugRender()						override;

	static CMonoBehaviour* Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext);
	virtual CComponent* Clone(void* pArg) override;
	virtual void Free() override;
};

END