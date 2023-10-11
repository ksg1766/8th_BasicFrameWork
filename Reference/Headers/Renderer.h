#pragma once

#include "Component.h"

/* 1. ȭ�鿡 �׷������ϴ� ��ü���� �׸��� ������� ��Ƽ� �����Ѵ�. */
/* 2. �����ϰ� �ִ� ��ü���� ������(��ο���)�� �����Ѵ�. */

BEGIN(Engine)

class ENGINE_DLL CRenderer final : public CComponent
{
public:
	/* RG_NONBLEND : ���� �׷����� Blend������Ʈ���� ���� ������ ���� �ݵ�� �������� �ֵ��� ���� �׷����Ѵ�. */
	/* RG_BLEND : �������ϰ� �׷����� ��ü�鵵 �ݵ�� �ָ��ִ� ����� �׸���. */
	enum RENDERGROUP { RG_PRIORITY, RG_NONLIGHT, RG_NONBLEND, RG_NONBLEND_INSTANCE, RG_BLEND, RG_BLEND_INSTANCE, RG_UI, RG_END };
private:
	CRenderer(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);	
	CRenderer(const CRenderer& rhs) = delete;
	virtual ~CRenderer() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;

public:
	HRESULT Add_RenderGroup(RENDERGROUP eRenderGroup, class CGameObject* pGameObject);
	HRESULT Draw_RenderObjects();

private:
	vector<class CGameObject*>			m_RenderObjects[RG_END];
	map<InstanceID, class CVIBuffer_Instance*> m_InstanceBuffers;

private:
	HRESULT Render_Priority();
	HRESULT Render_NonLight();
	HRESULT Render_NonBlend();
	HRESULT Render_NonBlend_Instance();
	HRESULT Render_Blend();
	HRESULT Render_Blend_Instance();
	HRESULT Render_UI();

private:
	void	AddInstanceData(InstanceID instanceId, InstancingData& data);
	void	ClearInstanceData();

public:
	static CRenderer* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CComponent* Clone(CGameObject* pGameObject, void* pArg) override;
	virtual void Free() override;
};

END