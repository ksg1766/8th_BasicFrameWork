#pragma once

/* .FBX파일을 로드하여 데이터들을 내 구조에 맞게 정리한다. */
#include "VIBuffer.h"

BEGIN(Engine)

// Bone
#define MAX_BONES 200
#define MAX_KEYFRAMES 300

struct AnimTransform
{
	// [ ][ ][ ][ ][ ][ ][ ] ... 200개
	using TransformArrayType = array<Matrix, MAX_BONES>;
	// [ ][ ][ ][ ][ ][ ][ ] ... 300 개
	array<TransformArrayType, MAX_KEYFRAMES> transforms;
};

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
	virtual void	Tick(const _float& fTimeDelta)	override;
	void			DebugRender()					override;
	HRESULT			Render();
	HRESULT			RenderInstancing(class CVIBuffer_Instance*& buffer);

public:
	HRESULT			UpdateAnimation(const _float& fTimeDelta);
	HRESULT			UpdateTweenData(const _float& fTimeDelta);

	void			PushTweenData(const InstancedTweenDesc& desc);
	HRESULT			BindMaterialTexture(class CShader* pShader, const _char* pConstantName, _uint iMaterialIndex, aiTextureType eType);

private:
	TYPE						m_eModelType = TYPE_END;
	_uint						m_iNumMeshes = { 0 };

	_uint						m_iNumMaterials = { 0 };

	vector<CBone*>				m_Bones;
	vector<CMesh*>				m_Meshes;
	vector<CAnimation*>			m_Animations;
	vector<MESH_MATERIAL>		m_Materials;

	_float4x4					m_matPivot;

	_int						m_iCurrentAnimIndex = 0;
	_int						m_iNextAnimIndex = -1;

private:
	TWEENDESC					m_TweenDesc;

	vector<ID3D11Texture2D*>	m_vecTextures;
	vector<ID3D11ShaderResourceView*> m_vecSRVs;

	class CShader*				m_pShader;

public:
	_uint			GetNumMeshes() const					{ return (_uint)m_Meshes.size(); }
	_uint			GetMaterialIndex(_uint iMeshIndex);
	CBone*			GetBone(const _char* pNodeName);
	CBone*			GetBone(const _int& iIndex);
	const _int		GetCurAnimationIndex() const			{ return m_iCurrentAnimIndex; }
	const _int		GetNextAnimationIndex() const			{ return m_iNextAnimIndex; }
	
	InstanceID		GetInstanceID();
	TweenDesc&		GetTweenDesc() { return m_TweenDesc; }

public:
	void			SetAnimation(_int iAnimIndex)			{ m_iCurrentAnimIndex = iAnimIndex; }
	void			SetNextAnimationIndex(_int iAnimIndex);

private:
	HRESULT			Ready_Bones(const wstring& strModelFilePath);
	HRESULT			Ready_Meshes(const wstring& strModelFilePath, Matrix matPivot);
	HRESULT			Ready_Materials(const wstring& strModelFilePath);
	HRESULT			Ready_Animations(const wstring& strModelFilePath);

private:
	HRESULT			CreateVertexTexture2D();
	void			CreateAnimationTransform(_uint index, vector<AnimTransform>& animTransforms);

public:
	static	CModel* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strPath, _fmatrix matPivot = XMMatrixScaling(0.01f, 0.01f, 0.01f) * XMMatrixRotationY(XMConvertToRadians(90.0f)));
	virtual CComponent* Clone(CGameObject* pGameObject, void* pArg) override;
	virtual void Free() override;
};

END