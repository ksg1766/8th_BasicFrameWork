#include "..\Public\Mesh.h"
#include "Model.h"
#include "Shader.h"
#include "Bone.h"
#include "Channel.h"
#include "Animation.h"
#include "GameObject.h"
#include "GraphicDevice.h"

CMesh::CMesh(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CVIBuffer(pDevice, pContext)
{
}

CMesh::CMesh(const CMesh& rhs)
	: CVIBuffer(rhs)
	, m_iMaterialIndex(rhs.m_iMaterialIndex)
	, m_BoneIndices(rhs.m_BoneIndices)
{
	if (!m_vecTextures.empty())
		m_vecTextures = rhs.m_vecTextures;
	if (!m_vecSRVs.empty())
		m_vecSRVs = rhs.m_vecSRVs;

	strcpy_s(m_szName, rhs.m_szName);
}

HRESULT CMesh::Initialize_Prototype(string& strName, vector<VTXMESH>& Vertices, vector<_int>& Indices,
	_uint iMatIndex, vector<_int>& Bones, Matrix& matPivot, CModel* pModel)
{
	/* 이 메시와 이름이 같은 뼈가 존재한다면. 이 뼈의 행렬을 메시를 구성하는 정점에 곱해질 수 있도록 유도한다. */
	strcpy_s(m_szName, strName.c_str());

	m_iMaterialIndex = iMatIndex;

	if (FAILED(Ready_StaticVertices(Vertices, matPivot)))
		return E_FAIL;

	if (FAILED(Ready_Indices(Indices)))
		return E_FAIL;

	/* 이 메시에서 사용하는 본의 인덱스를 저장한다. */
	m_BoneIndices.reserve((_uint)Bones.size());
	for (_int index : Bones)
		m_BoneIndices.push_back(index);

	if (m_vecTextures.empty())
	{
		CreateTexture2D();
	}

	return S_OK;
}

HRESULT CMesh::Initialize_Prototype(string& strName, vector<VTXANIMMESH>& Vertices, vector<_int>& Indices,
	_uint iMatIndex, vector<_int>& Bones, CModel* pModel)
{
	strcpy_s(m_szName, strName.c_str());

	m_iMaterialIndex = iMatIndex;

	if (FAILED(Ready_AnimVertices(Vertices)))
		return E_FAIL;

	if (FAILED(Ready_Indices(Indices)))
		return E_FAIL;

	/* 이 메시에서 사용하는 본의 인덱스를 저장한다. */
	m_BoneIndices.reserve((_uint)Bones.size());
	for (_int index : Bones)
		m_BoneIndices.push_back(index);

	return S_OK;
}

HRESULT CMesh::Initialize(void* pArg)
{
	/* 프로토타입에서 저장한 본 인덱스를 바탕으로 모델에서 본을 가져온다. */
	if (nullptr != pArg)
	{
		CModel* pModel = static_cast<CModel*>(pArg);

		for (size_t i = 0; i < m_BoneIndices.size(); i++)
		{
			CBone* pBone = pModel->GetBone(m_BoneIndices[i]);
			if (nullptr != pBone)
			{
				m_Bones.push_back(pBone);
				Safe_AddRef(pBone);
			}
		}

		m_BoneIndices.clear();
		m_BoneIndices.shrink_to_fit();
	}

	return S_OK;
}

void CMesh::Tick(const _float& fTimeDelta)
{
	if (m_vecTextures.empty())
		CreateTexture2D();

	_int iCurrentAnimIndex = m_pModel->GetCurAnimationIndex();

	if (iCurrentAnimIndex < 0)
		m_pModel->SetAnimation(m_pModel->GetAnimationCount());

	TWEENDESC& desc = m_TweenDesc;

	CAnimation* currentAnim = m_pModel->GetAnimations()[iCurrentAnimIndex];

	desc.curr.sumTime += fTimeDelta;

	{
		if (currentAnim)
		{
			_float timePerFrame = 1 / currentAnim->GetTickPerSecond();
			if (desc.curr.sumTime >= timePerFrame)
			{
				desc.curr.sumTime = 0.f;
				desc.curr.currFrame = (desc.curr.currFrame + 1) % currentAnim->GetMaxFrameCount();
				desc.curr.nextFrame = (desc.curr.currFrame + 1) % currentAnim->GetMaxFrameCount();
			}

			desc.curr.ratio = (desc.curr.sumTime / timePerFrame);
		}
	}

	// 다음 애니메이션이 예약 되어 있다면
	if (desc.next.animIndex >= 0)
	{
		desc.tweenSumTime += fTimeDelta;
		desc.tweenRatio = desc.tweenSumTime / desc.tweenDuration;

		if (desc.tweenRatio >= 1.f)
		{
			// 애니메이션 교체 성공
			desc.curr = desc.next;
			m_pModel->SetAnimation(desc.next.animIndex);
			desc.ClearNextAnim();
			m_pModel->SetNextAnimationIndex(-1);
		}
		else
		{
			// 교체중
			CAnimation* nextAnim = m_pModel->GetAnimations()[desc.next.animIndex];
			desc.next.sumTime += fTimeDelta;

			_float timePerFrame = 1.f / nextAnim->GetTickPerSecond();

			if (desc.next.ratio >= 1.f)
			{
				desc.next.sumTime = 0;

				desc.next.currFrame = (desc.next.currFrame + 1) % nextAnim->GetMaxFrameCount();
				desc.next.nextFrame = (desc.next.currFrame + 1) % nextAnim->GetMaxFrameCount();
			}

			desc.next.ratio = desc.next.sumTime / timePerFrame;
		}
	}

	if (m_IsNextAnimReserved)
	{
		desc.ClearNextAnim(); // 기존꺼 밀어주기
		desc.next.animIndex = m_pModel->GetNextAnimationIndex();

		m_IsNextAnimReserved = false;
	}

	if (FAILED(m_pGameObject->GetShader()->Bind_Texture("g_CurTransformMap", m_vecSRVs[iCurrentAnimIndex])))
		__debugbreak();
	if (desc.next.animIndex >= 0)
	{
		if (FAILED(m_pGameObject->GetShader()->Bind_Texture("g_NextTransformMap", m_vecSRVs[desc.next.animIndex])))
			__debugbreak();
	}
}

void CMesh::SetUp_BoneMatrices(_float4x4* pBoneMatrices, _fmatrix PivotMatrix)
{
	if (0 == m_Bones.size())
	{
		XMStoreFloat4x4(&pBoneMatrices[0], XMMatrixIdentity());
		return;
	}

	for (_uint i = 0; i < m_Bones.size(); ++i)
	{
		/* 셰이더에 행렬 던질 때는 전치 꼭 */
		/* 최종 트랜스폼 계산*/

		XMStoreFloat4x4(&pBoneMatrices[i], m_Bones[i]->Get_OffSetMatrix() * m_Bones[i]->Get_CombinedTransformation() * PivotMatrix);
	}
}

HRESULT CMesh::CreateTexture2D()
{
	_uint iAnimCount = m_pModel->GetAnimationCount();
	if (0 == iAnimCount)
		return S_OK;

	m_vecTextures.resize(iAnimCount);
	m_vecSRVs.resize(iAnimCount);

	vector<AnimTransform>	_animTransforms;
	_animTransforms.resize(iAnimCount);

	for (uint32 i = 0; i < iAnimCount; i++)
	{
		_uint iMaxFrameCount = m_pModel->GetAnimations()[i]->GetMaxFrameCount();
		CreateAnimationTransform(i, _animTransforms);

		// Creature Texture
		{
			D3D11_TEXTURE2D_DESC desc;
			ZeroMemory(&desc, sizeof(D3D11_TEXTURE2D_DESC));
			desc.Width = m_Bones.size() * 4;
			desc.Height = iMaxFrameCount;
			desc.ArraySize = 1;
			desc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT; // 16바이트
			desc.Usage = D3D11_USAGE_IMMUTABLE;
			desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
			desc.MipLevels = 1;
			desc.SampleDesc.Count = 1;

			const uint32 dataSize = m_Bones.size() * sizeof(Matrix);
			const uint32 pageSize = dataSize * iMaxFrameCount;
			void* mallocPtr = ::malloc(pageSize);

			// 파편화된 데이터를 조립한다.

			BYTE* pageStartPtr = reinterpret_cast<BYTE*>(mallocPtr);

			for (uint32 f = 0; f < iMaxFrameCount; f++)
			{
				void* ptr = pageStartPtr + f * dataSize;
				::memcpy(ptr, _animTransforms[i].transforms[f].data(), dataSize);
			}

			// 리소스 만들기
			D3D11_SUBRESOURCE_DATA subResource;

			void* ptr = (BYTE*)mallocPtr;
			subResource.pSysMem = ptr;
			subResource.SysMemPitch = dataSize;

			if (FAILED(CGraphicDevice::GetInstance()->GetDevice()->CreateTexture2D(&desc, &subResource, &m_vecTextures[i])))
				return E_FAIL;

			::free(mallocPtr);
		}

		// Create SRV
		{
			D3D11_SHADER_RESOURCE_VIEW_DESC desc;
			ZeroMemory(&desc, sizeof(desc));
			desc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
			desc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
			desc.Texture2DArray.MipLevels = 1;
			desc.Texture2DArray.ArraySize = iAnimCount;

			if (FAILED(CGraphicDevice::GetInstance()->GetDevice()->CreateShaderResourceView(m_vecTextures[i], &desc, &m_vecSRVs[i])))
				return E_FAIL;
		}
	}

	return S_OK;
}

void CMesh::CreateAnimationTransform(uint32 index, vector<AnimTransform>& animTransforms)
{
	CAnimation* pAnimation = m_pModel->GetAnimations()[index];

	for (uint32 f = 0; f < pAnimation->GetMaxFrameCount(); f++)
	{
		pAnimation->Calculate_Animation(f);

		for (uint32 b = 0; b < m_Bones.size(); b++)
		{
			m_Bones[b]->Set_CombinedTransformation();

			if (0 == m_Bones.size())
			{
				XMStoreFloat4x4(&animTransforms[index].transforms[f][b], XMMatrixIdentity());
				return;
			}
			
			XMStoreFloat4x4(&animTransforms[index].transforms[f][b], m_Bones[b]->Get_OffSetMatrix() * m_Bones[b]->Get_CombinedTransformation() * m_pModel->GetPivotMatrix());
		}
	}
}

HRESULT CMesh::Ready_StaticVertices(vector<VTXMESH>& Vertices, _fmatrix& PivotMatrix)
{
	m_iNumVBs = 1;
	m_iNumVertices = (_int)Vertices.size();
	m_iNumPrimitives = m_iNumVertices / 3;
	m_iNumIndicesofPrimitive = 3;
	m_iStride = sizeof(VTXMESH);

	ZeroMemory(&m_BufferDesc, sizeof(D3D11_BUFFER_DESC));
	m_BufferDesc.ByteWidth = m_iNumVertices * m_iStride;
	m_BufferDesc.Usage = D3D11_USAGE_DEFAULT;
	m_BufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	m_BufferDesc.CPUAccessFlags = 0;
	m_BufferDesc.MiscFlags = 0;
	m_BufferDesc.StructureByteStride = m_iStride;

	VTXMESH* pVertices = new VTXMESH[m_iNumVertices];
	ZeroMemory(pVertices, sizeof(VTXMESH) * m_iNumVertices);

	for (_uint i = 0; i < m_iNumVertices; ++i)
	{
		memcpy(&pVertices[i].vPosition, &Vertices[i].vPosition, sizeof(_float3));
		XMStoreFloat3(&pVertices[i].vPosition, XMVector3TransformCoord(XMLoadFloat3(&pVertices[i].vPosition), PivotMatrix));

		memcpy(&pVertices[i].vNormal, &Vertices[i].vNormal, sizeof(_float3));
		XMStoreFloat3(&pVertices[i].vNormal, XMVector3TransformNormal(XMLoadFloat3(&pVertices[i].vNormal), PivotMatrix));

		memcpy(&pVertices[i].vTexcoord, &Vertices[i].vTexcoord, sizeof(_float2));

		memcpy(&pVertices[i].vTangent, &Vertices[i].vTangent, sizeof(_float3));
		XMStoreFloat3(&pVertices[i].vTangent, XMVector3TransformNormal(XMLoadFloat3(&pVertices[i].vTangent), PivotMatrix));
	}

	ZeroMemory(&m_SubResourceData, sizeof(D3D11_SUBRESOURCE_DATA));
	m_SubResourceData.pSysMem = pVertices;

	if (FAILED(Super::Create_Buffer(&m_pVB)))
		return E_FAIL;

	Safe_Delete_Array(pVertices);

	return S_OK;
}

HRESULT CMesh::Ready_AnimVertices(vector<VTXANIMMESH>& Vertices)
{
	/* 기본 정점 버퍼 세팅 */
	m_iNumVBs = 1;
	m_iNumVertices = (_int)Vertices.size();
	m_iNumPrimitives = m_iNumVertices / 3;
	m_iStride = sizeof(VTXANIMMESH);

	ZeroMemory(&m_BufferDesc, sizeof(D3D11_BUFFER_DESC));
	m_BufferDesc.ByteWidth = m_iNumVertices * m_iStride;
	m_BufferDesc.Usage = D3D11_USAGE_DEFAULT;
	m_BufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	m_BufferDesc.CPUAccessFlags = 0;
	m_BufferDesc.MiscFlags = 0;
	m_BufferDesc.StructureByteStride = m_iStride;

	VTXANIMMESH* pVertices = new VTXANIMMESH[m_iNumVertices];
	ZeroMemory(pVertices, sizeof(VTXANIMMESH) * m_iNumVertices);

	for (_uint i = 0; i < m_iNumVertices; ++i)
	{
		/* NoneAnim 모델과 달리 사전변환을 진행하지 않는다. */
		memcpy(&pVertices[i].vPosition, &Vertices[i].vPosition, sizeof(_float3));
		memcpy(&pVertices[i].vNormal, &Vertices[i].vNormal, sizeof(_float3));
		memcpy(&pVertices[i].vTexture, &Vertices[i].vTexture, sizeof(_float2));
		memcpy(&pVertices[i].vTangent, &Vertices[i].vTangent, sizeof(_float3));
		memcpy(&pVertices[i].vBlendIndex, &Vertices[i].vBlendIndex, sizeof(XMUINT4));
		memcpy(&pVertices[i].vBlendWeight, &Vertices[i].vBlendWeight, sizeof(Vec4));
	}

	ZeroMemory(&m_SubResourceData, sizeof(D3D11_SUBRESOURCE_DATA));
	m_SubResourceData.pSysMem = pVertices;

	if (FAILED(Super::Create_Buffer(&m_pVB)))
		return E_FAIL;

	Safe_Delete_Array(pVertices);

	return S_OK;
}

HRESULT CMesh::Ready_Indices(vector<_int>& Indices)
{
	m_iNumPrimitives = ((_int)Indices.size()) / 3;
	m_iIndexSizeofPrimitive = sizeof(FACEINDICES32);
	m_iNumIndicesofPrimitive = 3;

	m_eIndexFormat = DXGI_FORMAT_R32_UINT;
	m_eTopology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;

	::ZeroMemory(&m_BufferDesc, sizeof(D3D11_BUFFER_DESC));
	m_BufferDesc.ByteWidth = m_iNumPrimitives * m_iIndexSizeofPrimitive;
	m_BufferDesc.Usage = D3D11_USAGE_DEFAULT;
	m_BufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	m_BufferDesc.CPUAccessFlags = 0;
	m_BufferDesc.MiscFlags = 0;
	m_BufferDesc.StructureByteStride = 0;

	FACEINDICES32* pIndices = new FACEINDICES32[m_iNumPrimitives];
	::ZeroMemory(pIndices, sizeof(FACEINDICES32) * m_iNumPrimitives);

	for (_uint i = 0, j = 0; i < m_iNumPrimitives; ++i, ++j)
	{
		pIndices[i]._0 = Indices[j];
		pIndices[i]._1 = Indices[++j];
		pIndices[i]._2 = Indices[++j];
	}

	ZeroMemory(&m_SubResourceData, sizeof(D3D11_SUBRESOURCE_DATA));
	m_SubResourceData.pSysMem = pIndices;

	if (FAILED(Super::Create_Buffer(&m_pIB)))
		return E_FAIL;

	Safe_Delete_Array(pIndices);

	return S_OK;
}

HRESULT CMesh::Initialize_VTF()
{
	if (m_vecTextures.empty())
		if(FAILED(CreateTexture2D()))
			return E_FAIL;

	return S_OK;
}

CMesh* CMesh::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, string& strName,
	vector<VTXMESH>& Vertices, vector<_int>& Indices, _uint iMatIndex, vector<_int>& Bones, Matrix& PivotMatrix, CModel* pModel)
{
	CMesh* pInstance = new CMesh(pDevice, pContext);
	pInstance->m_pModel = pModel;

	if (FAILED(pInstance->Initialize_Prototype(strName, Vertices, Indices, iMatIndex, Bones, PivotMatrix, pModel)))
	{
		MSG_BOX("Failed To Created : CMesh");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CMesh* CMesh::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, string& strName,
	vector<VTXANIMMESH>& Vertices, vector<_int>& Indices, _uint iMatIndex, vector<_int>& Bones, CModel* pModel)
{
	CMesh* pInstance = new CMesh(pDevice, pContext);
	pInstance->m_pModel = pModel;

	if (FAILED(pInstance->Initialize_Prototype(strName, Vertices, Indices, iMatIndex, Bones, pModel)))
	{
		MSG_BOX("Failed To Created : CMesh");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CComponent* CMesh::Clone(CGameObject* pGameObject, void* pArg)
{
	CMesh* pInstance = new CMesh(*this);
	pInstance->m_pGameObject = pGameObject;
	pInstance->m_pModel = static_cast<CModel*>(pArg);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed To Cloned : CMesh");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CMesh::Free()
{
	Super::Free();

	for (auto& pBone : m_Bones)
		Safe_Release(pBone);

	m_Bones.clear();
}
