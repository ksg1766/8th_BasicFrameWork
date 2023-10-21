#include "NavMeshAgent.h"
#include "ComponentManager.h"
#include "GameInstance.h"
#include "LevelManager.h"
#include "PipeLine.h"
#include "Shader.h"
#include "FileUtils.h"
#include "Cell.h"
#include "DebugDraw.h"

vector<CCell*> CNavMeshAgent::m_Cells;

CNavMeshAgent::CNavMeshAgent(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: Super(pDevice, pContext, ComponentType::NavMeshAgent)
{
}

CNavMeshAgent::CNavMeshAgent(const CNavMeshAgent& rhs)
	: Super(rhs)
	, m_pBatch(rhs.m_pBatch)
	, m_pEffect(rhs.m_pEffect)
	, m_pInputLayout(rhs.m_pInputLayout)
{
}

HRESULT CNavMeshAgent::Initialize_Prototype(const wstring& strNavigationData)
{
	/*shared_ptr<FileUtils> file = make_shared<FileUtils>();

	file->Open(strNavigationData, Read);

	while (true)
	{
		_float3		vPoints[CCell::POINT_END] = {};

		if (false == file->Read(vPoints))
			break;

		CCell* pCell = CCell::Create(m_pDevice, m_pContext, vPoints, m_Cells.size());
		if (nullptr == pCell)
			return E_FAIL;

		m_Cells.push_back(pCell);
	}*/
	_ulong		dwByte = 0;
	HANDLE		hFile = CreateFile(strNavigationData.c_str(), GENERIC_READ, 0, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
	if (0 == hFile)
		return E_FAIL;

	while (true)
	{
		_float3		vPoints[CCell::POINT_END] = {};

		ReadFile(hFile, vPoints, sizeof(_float3) * CCell::POINT_END, &dwByte, nullptr);

		if (0 == dwByte)
			break;

		CCell* pCell = CCell::Create(m_pDevice, m_pContext, vPoints, m_Cells.size());
		if (nullptr == pCell)
			return E_FAIL;

		m_Cells.push_back(pCell);
	}

	CloseHandle(hFile);

	if (FAILED(SetUp_Neighbors()))
		return E_FAIL;

#ifdef _DEBUG
	/*m_pShader = CShader::Create(m_pDevice, m_pContext, TEXT("../Bin/ShaderFiles/Shader_Cell.hlsl"), VTXPOS::Elements, VTXPOS::iNumElements);
	CComponentManager* pComponentManager = GET_INSTANCE(CComponentManager);
	CLevelManager* pLevelManager = GET_INSTANCE(CLevelManager);
	m_pShader = static_cast<CShader*>(pComponentManager->Clone_Component(m_pGameObject, pLevelManager->GetCurrentLevelIndex(), TEXT("Prototype_Component_Shader_VtxDebug")));
	if (nullptr == m_pShader)
		return E_FAIL;*/

	m_pBatch = new PrimitiveBatch<VertexPositionColor>(m_pContext);

	m_pEffect = new BasicEffect(m_pDevice);
	m_pEffect->SetVertexColorEnabled(true);

	const void* pShaderByteCodes = nullptr;
	size_t		iLength = 0;
	m_pEffect->GetVertexShaderBytecode(&pShaderByteCodes, &iLength);

	if (FAILED(m_pDevice->CreateInputLayout(VertexPositionColor::InputElements, VertexPositionColor::InputElementCount, pShaderByteCodes, iLength, &m_pInputLayout)))
	{
		Safe_Delete(m_pBatch);
		Safe_Delete(m_pEffect);
		Safe_Release(m_pInputLayout);
		return E_FAIL;
	}
#endif

	return S_OK;
}

HRESULT CNavMeshAgent::Initialize(void* pArg)
{
	if (nullptr == pArg)
		return S_OK;

	NAVIGATION_DESC* pNaviDesc = (NAVIGATION_DESC*)pArg;

	/*  이 네비게이션을 이용하고자하는 객체가 어떤 셀에 있는지 저장한다. */
	m_iCurrentIndex = pNaviDesc->iCurrentIndex;

	return S_OK;
}

void CNavMeshAgent::DebugRender()
{
#ifdef _DEBUG

	m_pEffect->SetWorld(XMMatrixIdentity());

	CPipeLine* pPipeLine = GET_INSTANCE(CPipeLine);

	m_pEffect->SetView(pPipeLine->Get_Transform_Matrix(CPipeLine::D3DTS_VIEW));
	m_pEffect->SetProjection(pPipeLine->Get_Transform_Matrix(CPipeLine::D3DTS_PROJ));

	RELEASE_INSTANCE(CPipeLine);

	m_pEffect->Apply(m_pContext);
	m_pContext->IASetInputLayout(m_pInputLayout);

	m_pBatch->Begin();

	if (-1 == m_iCurrentIndex)
	{
		for (auto& pCell : m_Cells)
		{
			if (nullptr != pCell)
				pCell->DebugRender(m_pBatch, Colors::Cyan);
		}
	}
	else
	{
		m_Cells[m_iCurrentIndex]->DebugRender(m_pBatch, Colors::YellowGreen);
	}

	m_pBatch->End();
	////////////////////////////////////////////////////////

	//if (FAILED(m_pShader->Bind_Matrix("g_WorldMatrix", &Matrix::Identity)))
	//	__debugbreak();

	//CPipeLine* pPipeLine = GET_INSTANCE(CPipeLine);

	////if (FAILED(m_pShader->Bind_Matrix("g_ViewMatrix", &pPipeLine->Get_Transform_float4x4(CPipeLine::D3DTS_VIEW))))
	//if (FAILED(m_pGameInstance->Bind_TransformToShader(m_pShader, "g_ViewMatrix", CPipeLine::D3DTS_VIEW)))
	//	__debugbreak();

	//if (FAILED(m_pGameInstance->Bind_TransformToShader(m_pShader, "g_ProjMatrix", CPipeLine::D3DTS_PROJ)))
	//	__debugbreak();

	//RELEASE_INSTANCE(CPipeLine);

	//if (-1 == m_iCurrentIndex)
	//{
	//	/*if (FAILED(m_pShader->Bind_RawValue("g_vLineColor", &_float4(0.f, 1.f, 0.f, 1.f), sizeof(_float4))))
	//		__debugbreak();*/
	//	/*m_pShader->SetPassIndex(1);
	//	_float		fHeight = 0.f;
	//	if (FAILED(m_pShader->Bind_RawValue("g_fHeight", &fHeight, sizeof(_float))))
	//		__debugbreak();

	//	if (FAILED(m_pShader->Begin()))
	//		__debugbreak();*/

	//	for (auto& pCell : m_Cells)
	//	{
	//		if (nullptr != pCell)
	//			pCell->DebugRender();
	//	}
	//}
	//else
	//{
	//	/*if (FAILED(m_pShader->Bind_RawValue("g_vLineColor", &_float4(1.f, 0.f, 0.f, 1.f), sizeof(_float4))))
	//		__debugbreak();*/
	//	/*m_pShader->SetPassIndex(0);
	//	_float		fHeight = 0.1f;
	//	if (FAILED(m_pShader->Bind_RawValue("g_fHeight", &fHeight, sizeof(_float))))
	//		__debugbreak();

	//	if (FAILED(m_pShader->Begin()))
	//		__debugbreak();*/

	//	m_Cells[m_iCurrentIndex]->DebugRender();
	//}
#endif
}

_bool CNavMeshAgent::IsMove(_fvector vPoint)
{
	_int		iNeighborIndex = 0;

	if (true == m_Cells[m_iCurrentIndex]->isOut(vPoint, &iNeighborIndex))
	{
		/* 나간 방향에 이웃셀이 있으면 움직여야해! */
		if (-1 != iNeighborIndex)
		{
			while (true)
			{
				if (-1 == iNeighborIndex)
					return false;

				if (false == m_Cells[iNeighborIndex]->isOut(vPoint, &iNeighborIndex))
				{
					m_iCurrentIndex = iNeighborIndex;
					break;
				}
			}
			return true;
		}
		else
			return false;
	}
	else
		return true;
}

HRESULT CNavMeshAgent::SetUp_Neighbors()
{
	for (auto& pSourCell : m_Cells)
	{
		for (auto& pDestCell : m_Cells)
		{
			if (pSourCell == pDestCell)
				continue;

			if (true == pDestCell->Compare_Points(pSourCell->Get_Point(CCell::POINT_A), pSourCell->Get_Point(CCell::POINT_B)))
			{
				pSourCell->SetUp_Neighbor(CCell::LINE_AB, pDestCell);
			}

			else if (true == pDestCell->Compare_Points(pSourCell->Get_Point(CCell::POINT_B), pSourCell->Get_Point(CCell::POINT_C)))
			{
				pSourCell->SetUp_Neighbor(CCell::LINE_BC, pDestCell);
			}

			else if (true == pDestCell->Compare_Points(pSourCell->Get_Point(CCell::POINT_C), pSourCell->Get_Point(CCell::POINT_A)))
			{
				pSourCell->SetUp_Neighbor(CCell::LINE_CA, pDestCell);
			}
		}
	}

	return S_OK;
}

CNavMeshAgent* CNavMeshAgent::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strNavigationData)
{
	CNavMeshAgent* pInstance = new CNavMeshAgent(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype(strNavigationData)))
	{
		MSG_BOX("Failed to Created : CNavMeshAgent");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CComponent* CNavMeshAgent::Clone(CGameObject* pGameObject, void* pArg)
{
	CNavMeshAgent* pInstance = new CNavMeshAgent(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CNavMeshAgent");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CNavMeshAgent::Free()
{
	Super::Free();
}