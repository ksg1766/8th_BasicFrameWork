#pragma once

#include "Base.h"
#include "Component.h"
/* Ŭ���̾���Ʈ���� ������ �پ��� ���ӿ�����Ʈ���� �θ𰡵ȴ�. */

BEGIN(Engine)

class CMonoBehaviour;
class CRenderer;
class CShader;
class CTexture;
class CVIBuffer;
class CRigidBody;

class ENGINE_DLL CGameObject abstract : public CBase
{
protected:
	/* ������ ������ �� */
	CGameObject(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	/* �纻�� ������ �� */
	CGameObject(const CGameObject& rhs); /* ���� ������. */
	virtual ~CGameObject() = default;

public:
	virtual HRESULT			Initialize_Prototype();
	virtual HRESULT			Initialize(void* pArg);
	virtual void			Tick(_float fTimeDelta);
	virtual void			LateTick(_float fTimeDelta);
	virtual void			DebugRender();
	virtual HRESULT			Render();

	CComponent*				GetFixedComponent(ComponentType type) const;
	CTransformEx*			GetTransform() const;
	CVIBuffer*				GetBuffer() const;
	CRigidBody*				GetRigidBody() const;
	//CCamera*				GetCamera();
	//CMeshRenderer*		GetMeshRenderer();
	//CModelRenderer*		GetModelRenderer();
	//CModelAnimator*		GetModelAnimator();
	CRenderer*				GetRenderer() const;
	CShader*				GetShader() const;
	CTexture*				GetTexture() const;
	//CLight*				GetLight();
	//CTerrain*				GetTerrain();

	CTransformEx*			GetOrAddTransform(_uint iLevelIndex);
	_bool					IsDead() const					{ return m_IsDead; }
	void					SetDeadState(_bool bDead)	{ m_IsDead = bDead; }

	HRESULT					AddComponent(_uint iLevelIndex, ComponentType type, const wstring& strPrototypeTag, void* pArg = nullptr);

	/*void					SetLayerIndex(uint8 layer) { m_i8LayerIndex = layer; }
	uint8					GetLayerIndex() { return m_i8LayerIndex; }*/

	virtual	void			OnCollisionEnter(CGameObject* pOther) {};
	virtual	void			OnCollisionStay(CGameObject* pOther) {};
	virtual	void			OnCollisionExit(CGameObject* pOther) {};

	virtual	void			OnTriggerEnter(CGameObject* pOther) {};
	virtual	void			OnTriggerStay(CGameObject* pOther) {};
	virtual	void			OnTriggerExit(CGameObject* pOther) {};
protected:
	ID3D11Device*			m_pDevice = { nullptr };
	ID3D11DeviceContext*	m_pContext = { nullptr };

protected:
	array<CComponent*, FIXED_COMPONENT_COUNT> m_arrComponents = { nullptr };
	vector<CMonoBehaviour*> m_vecScripts;

	//uint8					m_i8LayerIndex = 0;

protected:
	/* Ư�� ������Ʈ�� �˻��� �����ϰ� �ϱ� ���ؼ�. */
	/* �˻��� ����ϴ� ����?! : Ÿ ��ü�� ���� ��ü�� ����(������Ʈ)�� �����ϰ��� �ϴ� ��찡 ����ϴ�. */
	//map<const wstring, class CComponent*>		m_Components;

protected:
	//HRESULT Add_Component(_uint iLevelIndex, const wstring& strPrototypeTag, const wstring& strComponentTag, _Inout_ CComponent** ppOut, void* pArg = nullptr);
	//class CComponent* Find_Component(const wstring& strComponentTag);

private:
	_bool					m_IsDead;

public:
	virtual CGameObject* Clone(void* pArg) = 0;
	virtual void Free() override;
};

END