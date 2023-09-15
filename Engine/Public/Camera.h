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
    HRESULT Initialize(void* pArg) override;
    static CCamera* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
    virtual CComponent* Clone(void* pArg) override;
    virtual void Free() override;
};

END