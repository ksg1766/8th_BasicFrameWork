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
	using Super = CBase;
protected:
	/* ������ ������ �� */
	CGameObject(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	/* �纻�� ������ �� */
	CGameObject(const CGameObject& rhs); /* ���� ������. */
	virtual ~CGameObject() = default;

public:
	virtual HRESULT			Initialize_Prototype();
	virtual HRESULT			Initialize(void* pArg);
	virtual void			Tick(const _float& fTimeDelta);
	virtual void			LateTick(const _float& fTimeDelta);
	virtual void			DebugRender();
	virtual HRESULT			Render();

	CComponent*				GetFixedComponent(const ComponentType& type) const;
	CTransform*				GetTransform()	const;
	CVIBuffer* const		GetBuffer()		const;
	CRigidBody*				GetRigidBody()	const;
	//CCamera*				GetCamera();
	//CMeshRenderer*		GetMeshRenderer();
	//CModelRenderer*		GetModelRenderer();
	//CModelAnimator*		GetModelAnimator();
	CRenderer*				GetRenderer()	const;
	CShader*				GetShader()		const;
	CTexture*				GetTexture()	const;
	//CLight*				GetLight();
	//CTerrain*				GetTerrain();

	CTransform*				GetOrAddTransform(_uint iLevelIndex);
	_bool					IsDead()		const			{ return m_IsDead; }
	LAYERTAG				GetLayerTag()	const			{ return m_eLayerTag; }
	const wstring&			GetObjectTag()	const			{ return m_strObjectTag; }

	void					SetLayerTag(LAYERTAG eLayerTag) { m_eLayerTag = eLayerTag; }
	void					SetObjectTag(const wstring& strObjectTag) { m_strObjectTag = strObjectTag; }
	void					SetDeadState(_bool bDead)		{ m_IsDead = bDead; }

	HRESULT					AddComponent(_uint iLevelIndex, ComponentType type, const wstring& strPrototypeTag, void* pArg = nullptr);

	/*void					SetLayerIndex(uint8 layer) { m_i8LayerIndex = layer; }
	uint8					GetLayerIndex() { return m_i8LayerIndex; }*/

	virtual	void			OnCollisionEnter(const CGameObject* pOther) {};
	virtual	void			OnCollisionStay(const CGameObject* pOther) {};
	virtual	void			OnCollisionExit(const CGameObject* pOther) {};

	virtual	void			OnTriggerEnter(const CGameObject* pOther) {};
	virtual	void			OnTriggerStay(const CGameObject* pOther) {};
	virtual	void			OnTriggerExit(const CGameObject* pOther) {};

protected:
	ID3D11Device*			m_pDevice = { nullptr };
	ID3D11DeviceContext*	m_pContext = { nullptr };

protected:
	array<CComponent*, FIXED_COMPONENT_COUNT> m_arrComponents = { nullptr };
	vector<CMonoBehaviour*> m_vecScripts;

private:
	_bool					m_IsDead;
	LAYERTAG				m_eLayerTag;	// �ʿ������ �����غ���.
	wstring					m_strObjectTag;

public:
	virtual CGameObject* Clone(void* pArg) = 0;
	virtual void Free() override;
};

END