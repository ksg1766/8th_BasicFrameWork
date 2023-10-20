#include "stdafx.h"
#include "ViewMediator.h"
#include "NavMeshView.h"
#include "GameInstance.h"
#include "GameObject.h"
#include "Utils.h"
#include "Layer.h"
#include "DissolveManager.h"

CNavMeshView::CNavMeshView(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:Super(pDevice, pContext)
{
}

HRESULT CNavMeshView::Initialize(void* pArg)
{
	Super::Initialize(pArg);

	return S_OK;
}

HRESULT CNavMeshView::Tick()
{
	Input();

	ImGui::Begin("Layers");

	InfoView();

	ImGui::End();

	return S_OK;
}

HRESULT CNavMeshView::LateTick()
{

	return S_OK;
}

HRESULT CNavMeshView::DebugRender()
{

	return S_OK;
}

void CNavMeshView::Input()
{
	if (ImGui::GetIO().WantCaptureMouse)
		return;

	if (m_pGameInstance->Mouse_Down(DIM_LB))
	{
		if (m_IsPickingActivated)
		{
			const POINT& p = m_pGameInstance->GetMousePos();
			Pick(p.x, p.y, , );
		}
	}
}

_bool CNavMeshView::Pick(_uint screenX, _uint screenY, Vec3& pickPos, _float& distance)
{
	vector<CGameObject*>& vecGoundObjects = m_pGameInstance->GetCurrentLevelLayers()[LAYERTAG::GROUND]->GetGameObjects();
	if (vecGoundObjects.empty()) return false;

	Viewport& vp = m_pGameInstance->GetViewPort();

	_float fWidth = vp.width;
	_float fHeight = vp.height;

	Matrix projectionMatrix = m_pGameInstance->Get_Transform_float4x4(CPipeLine::D3DTS_PROJ);

	_float viewX = (+2.0f * screenX / fWidth - 1.0f) / projectionMatrix(0, 0);
	_float viewY = (-2.0f * screenY / fHeight + 1.0f) / projectionMatrix(1, 1);

	Matrix matViewMatrixInv = m_pGameInstance->Get_Transform_float4x4_Inverse(CPipeLine::D3DTS_VIEW);

	Vec3 vPickedPos = Vec3::Zero;
	_float fMinDistance = FLT_MAX;
	
	for (auto& iter : vecGoundObjects)
	{
		Vec4 vRayOrigin = Vec4(0.0f, 0.0f, 0.0f, 1.0f);
		Vec4 vRayDir = Vec4(viewX, viewY, 1.0f, 0.0f);

		Vec3 vWorldRayOrigin = XMVector3TransformCoord(vRayOrigin, matViewMatrixInv);
		Vec3 vWorldRayDir = XMVector3TransformNormal(vRayDir, matViewMatrixInv);
		vWorldRayDir.Normalize();

		Ray cRay = Ray(vWorldRayOrigin, vWorldRayDir);

		_float fDistance = 0.f;

		vector<Vec3>& vecSurfaceVtx = iter->GetModel()->GetSurfaceVtx();
		vector<FACEINDICES32>& vecSurfaceIdx = iter->GetModel()->GetSurfaceIdx();
		for (_int i = 0; i < vecSurfaceIdx.size(); i++)
		{
			if (cRay.Intersects(vecSurfaceVtx[vecSurfaceIdx[i]._0], vecSurfaceVtx[vecSurfaceIdx[i]._1], vecSurfaceVtx[vecSurfaceIdx[i]._2], OUT distance))
			{
				vPickedPos = cRay.position + cRay.direction * distance;
				if (isnan(vPickedPos.x) || isnan(vPickedPos.y) || isnan(vPickedPos.z) || isnan(distance))
					continue;

				if (distance < fMinDistance)
				{
					fMinDistance = distance;
					pickPos = vPickedPos;
				}
			}
		}
	}

	m_vecPoints.push_back(pickPos);

	string x = to_string(pickPos.x);
	string y = to_string(pickPos.y);
	string z = to_string(pickPos.z);
	m_strPoints.push_back(x.c_str());
	m_strPoints.push_back(y.c_str());
	m_strPoints.push_back(z.c_str());

	if (3 == m_vecPoints.size())
	{
		//CCell::Create();
		//m_vecCells.push_back();
		//m_vecPoints.clear();
		//m_strPoints.clear();
	}

	return true;
}

void CNavMeshView::InfoView()
{
	ImGui::Text("This window has some useful function for Objects in Level.");
	ImGui::NewLine();

	ImGui::Text("Mouse Picking ");
	if (m_IsPickingActivated)
	{
		ImGui::SameLine();
		if (ImGui::Button("Deactivate"))
			m_IsPickingActivated = false;
	}
	else
	{
		ImGui::SameLine();
		if (ImGui::Button("Activate"))
		{
			m_IsPickingActivated = true;
			m_pMediator->OnNotifiedPickingOn(this);
		}
	}
	ImGui::NewLine();
}

void CNavMeshView::PointGroup()
{
	ImGui::ListBox("Points", &m_Item_Current, m_strPoints.data(), m_strPoints.size(), 3);
}

void CNavMeshView::CellGroup()
{
	ImGui::ListBox("Cells", &m_Item_Current, m_strCells.data(), m_strCells.size(), 7);
}

CNavMeshView* CNavMeshView::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CNavMeshView* pInstance = new CNavMeshView(pDevice, pContext);

	if (FAILED(pInstance->Initialize(nullptr)))
	{
		MSG_BOX("Failed to Created : CNavMeshView");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CNavMeshView::Free()
{
	Super::Free();
}
