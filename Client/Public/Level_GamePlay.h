#pragma once

#include "Client_Defines.h"
#include "Level.h"

BEGIN(Client)

class CLevel_GamePlay final : public CLevel
{
	using Super = CLevel;
private:
	CLevel_GamePlay(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual ~CLevel_GamePlay() = default;

public:
	virtual HRESULT Initialize() override;
	virtual HRESULT Tick(const _float& fTimeDelta) override;
	virtual HRESULT LateTick(const _float& fTimeDelta) override;

private:
	HRESULT			Ready_Layer_Terrain();
	HRESULT			Ready_Layer_Player();
	HRESULT			Ready_Layer_Default();
	HRESULT			Ready_Layer_Camera();

public:
	static class CLevel_GamePlay* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual void Free() override;
};

END