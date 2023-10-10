#pragma once

/* 모델의 특정 파츠를 의미하는 메시. */
/* 각 파츠(메시)마다 정점, 인덱스버퍼를 가진다. */
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

class CModel;
class CMesh final : public CVIBuffer
{
	using Super = CVIBuffer;
private:
	CMesh(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CMesh(const CMesh& rhs);
	virtual ~CMesh() = default;

public:
	HRESULT Initialize_Prototype(string& strName, vector<VTXMESH>& Vertices, vector<_int>& Indices, _uint iMatIndex, vector<_int>& Bones, Matrix& matPivot, CModel* pModel);
	HRESULT Initialize_Prototype(string& strName, vector<VTXANIMMESH>& Vertices, vector<_int>& Indices, _uint iMatIndex, vector<_int>& Bones, CModel* pModel);
	virtual HRESULT Initialize(void* pArg);
	virtual void	Tick(const _float& fTimeDelta) override;

public:
	_uint					Get_MaterialIndex() const	{ return m_iMaterialIndex; }
	KEYFRAMEDESC&			Get_KeyFrameDesc()			{ return m_KeyFrameDesc; }
	void					SetModel(CModel* pModel)	{ m_pModel = pModel; }

public:
	void					SetUp_BoneMatrices(_float4x4* pBoneMatrices, _fmatrix PivotMatrix); /* 메시의 정점을 그리기위해 셰이더에 넘기기위한 뼈행렬의 배열을 구성한다. */

private:
	char					m_szName[MAX_PATH] = "";
	_uint					m_iMaterialIndex = 0;	/* 메시를 그릴 때 사용하는 매태리얼 인덱스 */

private:
	vector<class CBone*>	m_Bones;			/* 이 메시에 영향을 주는 뼈 집합 (메시별로 렌더링시, 해당 메시에 영향을 주는 뼈 행렬을 모아서 토스 */
	vector<_int>			m_BoneIndices;		/* Cache (Initialize에서 뼈를 찾아오기 위한 인덱스들) */

private:
	CModel*					m_pModel = nullptr;
	KEYFRAMEDESC			m_KeyFrameDesc;

	ID3D11Texture2D*		m_pTexture = nullptr;
	ID3D11ShaderResourceView* m_pSRV = nullptr;

	vector<ID3D11Texture2D*> m_vecTextures;
	vector<ID3D11ShaderResourceView*> m_vecSRVs;

	vector<AnimTransform>	m_animTransforms;

private:
	HRESULT	CreateTexture2DArray();
	HRESULT	CreateTexture2D();
	void	CreateAnimationTransform(uint32 index, vector<AnimTransform>& animTransforms);

private:
	HRESULT Ready_StaticVertices(vector<VTXMESH>& Vertices, _fmatrix& PivotMatrix);
	HRESULT Ready_AnimVertices(vector<VTXANIMMESH>& Vertices);
	HRESULT Ready_Indices(vector<_int>& Indices);

public:
	static CMesh* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, string& strName, vector<VTXMESH>& Vertices, vector<_int>& Indices, _uint iMatIndex, vector<_int>& Bones, Matrix& matPivot, CModel* pModel);
	static CMesh* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, string& strName, vector<VTXANIMMESH>& Vertices, vector<_int>& Indices, _uint iMatIndex, vector<_int>& Bones, CModel* pModel);

	virtual CComponent* Clone(CGameObject* pGameObject, void* pArg = nullptr) override;
	virtual void Free() override;
};

END