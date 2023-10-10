#pragma once

/* ���� Ư�� ������ �ǹ��ϴ� �޽�. */
/* �� ����(�޽�)���� ����, �ε������۸� ������. */
#include "VIBuffer.h"

BEGIN(Engine)

// Bone
#define MAX_BONES 200
#define MAX_KEYFRAMES 300

struct AnimTransform
{
	// [ ][ ][ ][ ][ ][ ][ ] ... 200��
	using TransformArrayType = array<Matrix, MAX_BONES>;
	// [ ][ ][ ][ ][ ][ ][ ] ... 300 ��
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
	void					SetUp_BoneMatrices(_float4x4* pBoneMatrices, _fmatrix PivotMatrix); /* �޽��� ������ �׸������� ���̴��� �ѱ������ ������� �迭�� �����Ѵ�. */

private:
	char					m_szName[MAX_PATH] = "";
	_uint					m_iMaterialIndex = 0;	/* �޽ø� �׸� �� ����ϴ� ���¸��� �ε��� */

private:
	vector<class CBone*>	m_Bones;			/* �� �޽ÿ� ������ �ִ� �� ���� (�޽ú��� ��������, �ش� �޽ÿ� ������ �ִ� �� ����� ��Ƽ� �佺 */
	vector<_int>			m_BoneIndices;		/* Cache (Initialize���� ���� ã�ƿ��� ���� �ε�����) */

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