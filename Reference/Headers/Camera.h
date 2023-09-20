#pragma once
#include "Component.h"

BEGIN(Engine)

class ENGINE_DLL CCamera :
    public CComponent
{
    using Super = CComponent;
private:
    CCamera(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
    CCamera(const CCamera& rhs);
    virtual ~CCamera() = default;

public:
    virtual HRESULT Initialize_Prototype()          override;
    virtual HRESULT Initialize(void* pArg)          override;
    virtual void Tick(const _float& fTimeDelta)     override;
    virtual void LateTick(const _float& fTimeDelta) override;

private:
    class CPipeLine* m_pPipeLine = { nullptr };

    Vec4			m_vEye, m_vAt;
    _float			m_fFovy, m_fAspect, m_fNear, m_fFar;

public:
    static CCamera* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
    virtual CComponent* Clone(CGameObject* pGameObject, void* pArg) override;
    virtual void Free() override;
};

END