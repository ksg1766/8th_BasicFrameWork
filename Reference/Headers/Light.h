#pragma once

#include "Base.h"

BEGIN(Engine)

class CShader;
class CVIBuffer_Rect;
class CLight final : public CBase
{
private:
	CLight();
	virtual ~CLight() = default;

public:
	const LIGHT_DESC* Get_LightDesc() const { return &m_LightDesc; } 

public:
	HRESULT Initialize(const LIGHT_DESC& LightDesc);
	HRESULT Render(CShader* pShader, CVIBuffer_Rect* pVIBuffer);

private:
	LIGHT_DESC			m_LightDesc;

public:
	static CLight* Create(const LIGHT_DESC& LightDesc);
	virtual void Free() override;
};

END