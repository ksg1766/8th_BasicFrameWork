#pragma once

#include "Base.h"

BEGIN(Engine)

class CLight;
class CShader;
class CVIBuffer_Rect;
class CLightManager final : public CBase
{
	DECLARE_SINGLETON(CLightManager)
private:
	CLightManager();
	virtual ~CLightManager() = default;

public:
	const LIGHT_DESC* Get_LightDesc(_uint iLightIndex);

public:
	HRESULT Add_Light(const LIGHT_DESC& LightDesc);
	HRESULT Render(CShader* pShader, CVIBuffer_Rect* pVIBuffer);

	HRESULT GetLightViewMatrix(_uint iLightIndex = 0);
	HRESULT GetLightProjMatrix(_uint iLightIndex = 0);

private:
	vector<CLight*>				m_vecLights;

public:
	virtual void Free() override;
};

END