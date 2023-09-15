#pragma once

#include "Component.h"

/* 클라이언트에서 제작할 다양한 게임오브젝트들의 부모가된다. */

BEGIN(Engine)

class CMonoBehaviour;
class ENGINE_DLL CGameObject abstract : public CBase
{
protected:
	/* 원형을 생성할 때 */
	CGameObject(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	/* 사본을 생성할 때 */
	CGameObject(const CGameObject& rhs); /* 복사 생성자. */
	virtual ~CGameObject() = default;

public:
	virtual HRESULT			Initialize_Prototype();
	virtual HRESULT			Initialize(void* pArg);
	virtual void			Tick(_float fTimeDelta);
	virtual void			LateTick(_float fTimeDelta);
	virtual HRESULT			Render();

	CComponent* GetFixedComponent(ComponentType type);
	CTransform* GetTransform();
	//CCamera* GetCamera();
	//CMeshRenderer* GetMeshRenderer();
	//CModelRenderer* GetModelRenderer();
	//CModelAnimator* GetModelAnimator();
	//CLight* GetLight();
	//CBaseCollider* GetCollider();
	//CTerrain* GetTerrain();
	//CButton* GetButton();

	CTransform*				GetOrAddTransform();
	void					AddComponent(CComponent* component);

	void					SetLayerIndex(uint8 layer) { m_i8LayerIndex = layer; }
	uint8					GetLayerIndex() { return m_i8LayerIndex; }

protected:
	ID3D11Device*			m_pDevice = { nullptr };
	ID3D11DeviceContext*	m_pContext = { nullptr };

protected:
	array<CComponent*, FIXED_COMPONENT_COUNT> m_arrComponents;
	vector<CMonoBehaviour*> m_vecScripts;

	uint8 m_i8LayerIndex = 0;

public:
	virtual CGameObject* Clone(void* pArg) PURE;
	virtual void Free() override;
};

END