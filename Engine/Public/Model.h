#pragma once

/* .FBX파일을 로드하여 데이터들을 내 구조에 맞게 정리한다. */
#include "VIBuffer.h"

BEGIN(Engine)

#define MAX_BONES 250 /* 셰이더 파일에서도 똑같이 정의 해줘야한다. */

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
	_uint						m_iCurrentAnimIndex = 37;
	_float4x4					m_BoneMatrices[MAX_BONES] = {};

public:
	_uint		GetNumMeshes() const			{ return (_uint)m_Meshes.size(); }
	_uint		GetMaterialIndex(_uint iMeshIndex);
	_matrix		GetPivotMatrix()				{ return XMLoadFloat4x4(&m_matPivot); }
	CBone*		GetBone(const _char* pNodeName);
	CBone*		GetBone(const _int& iIndex);
	vector<CAnimation*>& GetAnimations()		{ return m_Animations; }
	const _uint GetAnimationCount() const		{ return (_uint)m_Animations.size(); }
	const _uint GetCurAnimationIndex() const	{ return m_iCurrentAnimIndex; }

public:
	void		SetAnimation(_uint iAnimIndex)	{ m_iCurrentAnimIndex = iAnimIndex; }

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