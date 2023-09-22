#pragma once
#include "Base.h"
#include "Client_Defines.h"

#include "../Public/ImGUI/imgui.h"
#include "../Public/ImGUI/imgui_impl_dx11.h"
#include "../Public/ImGUI/imgui_impl_win32.h"

BEGIN(Client)

class CImGUIManager final : public CBase
{
	DECLARE_SINGLETON(CImGUIManager)

private:
	CImGUIManager();
	virtual ~CImGUIManager() = default;

public:
	void Initialize(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	void Tick();
	void Render();

public:
	virtual void Free() override;
};

END