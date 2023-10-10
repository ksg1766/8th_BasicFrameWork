#pragma once

/* .FBX������ �ε��Ͽ� �����͵��� �� ������ �°� �����Ѵ�. */
#include "VIBuffer.h"

BEGIN(Engine)

#define MAX_BONES 250 /* ���̴� ���Ͽ����� �Ȱ��� ���� ������Ѵ�. */

class CBone;
class CMesh;
class CAnimation;
class ENGINE_DLL CModel final : public CComponent
{
	using Super = CComponent;
public:
	enum TYPE { TYPE_NONANIM, TYPE_ANIM, TYPE_END };

private:
	CModel(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CModel(const CModel& rhs);
	virtual ~CModel() = default;

public:
	virtual HRESULT Initialize_Prototype(const wstring& strModelFilePath, _fmatrix& matPivot);
	virtual HRESULT Initialize(void* pArg)			override;
	HRESULT			InitializeWithFile(const wstring& strModelFilePath, _fmatrix& matPivot);
	virtual void	Tick(const _float& fTimeDelta)	override;
	void			DebugRender()					override;
	HRESULT			Render(_uint& iMeshIndex);

public:
	HRESULT			BindMaterialTexture(class CShader* pShader, const _char* pConstantName, _uint iMaterialIndex, aiTextureType eType);
	HRESULT			UpdateAnimation(const _float& fTimeDelta);

private:
	TYPE						m_eModelType = TYPE_END;
	_uint						m_iNumMeshes = { 0 };

	_uint						m_iNumMaterials = { 0 };

	vector<CBone*>				m_Bones;
	vector<CMesh*>				m_Meshes;
	vector<CAnimation*>			m_Animations;
	vector<MESH_MATERIAL>		m_Materials;

	_float4x4					m_matPivot;
	/* Cache */
	_int						m_iCurrentAnimIndex = 0;
	_int						m_iNextAnimIndex = -1;
	_float4x4					m_BoneMatrices[MAX_BONES] = {};

public:
	_uint		GetNumMeshes() const					{ return (_uint)m_Meshes.size(); }
	_uint		GetMaterialIndex(_uint iMeshIndex);
	_matrix		GetPivotMatrix()						{ return XMLoadFloat4x4(&m_matPivot); }
	CBone*		GetBone(const _char* pNodeName);
	CBone*		GetBone(const _int& iIndex);
	vector<CAnimation*>& GetAnimations()				{ return m_Animations; }
	const _int	GetAnimationCount() const				{ return (_int)m_Animations.size(); }
	const _int	GetCurAnimationIndex() const			{ return m_iCurrentAnimIndex; }
	const _int	GetNextAnimationIndex() const			{ return m_iNextAnimIndex; }

public:
	void		SetAnimation(_int iAnimIndex)			{ m_iCurrentAnimIndex = iAnimIndex; }
	void		SetNextAnimationIndex(_int iAnimIndex);

private:
	HRESULT Ready_Bones(const wstring& strModelFilePath);
	HRESULT Ready_Meshes(const wstring& strModelFilePath, Matrix matPivot);
	//HRESULT Ready_Skeletal_Meshes(const wstring& strModelFilePath);
	//HRESULT Ready_Static_Meshes(const wstring& strModelFilePath, Matrix matPivot);
	HRESULT Ready_Materials(const wstring& strModelFilePath);
	HRESULT Ready_Animations(const wstring& strModelFilePath);

public:
	//static	CModel* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	static	CModel* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strPath, _fmatrix matPivot = XMMatrixScaling(0.01f, 0.01f, 0.01f) * XMMatrixRotationY(XMConvertToRadians(180.0f)));
	virtual CComponent* Clone(CGameObject* pGameObject, void* pArg) override;
	virtual void Free() override;
};

END