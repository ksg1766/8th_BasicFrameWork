#include "QuadTree.h"
#include "QuadTreeNode.h"
#include "ObjectManager.h"
#include "GameObject.h"
#include "Transform.h"
#include "Layer.h"

IMPLEMENT_SINGLETON(CQuadTree)

CQuadTree::CQuadTree()
{
}

HRESULT CQuadTree::Build_QuadTree(_uint iNumLevels)
{
    CObjectManager* pObjectManager = GET_INSTANCE(CObjectManager);

    m_pQuadTreeRoot = BuildQuadTree(Vec3(0.f, m_vRootExtents.y * 0.5f, 0.f), Vec3(m_vRootExtents), m_iDepthLimit);

    map<LAYERTAG, CLayer*>& mapLayers = *pObjectManager->GetCurrentLevelLayers();

    vector<CGameObject*>& vecObjects = mapLayers[LAYERTAG::WALL]->GetGameObjects();
    /*vector<CGameObject*>& vecObjects = mapLayers[LAYERTAG::WALL]->GetGameObjects();
    vector<CGameObject*>& vecObjects = mapLayers[LAYERTAG::WALL]->GetGameObjects();*/
    
    for (auto& iter : vecObjects)
        AddObjectInNode(iter->GetTransform(), m_pQuadTreeRoot);

    if (!m_pQuadTreeRoot)
    {
        RELEASE_INSTANCE(CObjectManager)
        return E_FAIL;
    }

    RELEASE_INSTANCE(CObjectManager)

    return S_OK;
}

void CQuadTree::Update_QuadTree()
{
    Update_Frustum();
    FrustumCull(m_pQuadTreeRoot);
}

void CQuadTree::Update_Frustum()
{
    // TODO: 이 함수가 필요해서 놔둔건지 기억이 안남... 프러스텀이 자동으로 갱신되지 않는다면 여기서 갱신해줄 것.
    //BoundingFrustum::CreateFromMatrix(m_tFrustum, );
}

void CQuadTree::Render_QuadTree()
{
    m_pQuadTreeRoot->Render_QuadTreeNode();
}

CQuadTreeNode* CQuadTree::GetCurrentNodeByPos(Vec3 vPos, CQuadTreeNode* const pNode)
{
    // Recursive
    if (pNode->GetBoundingBox()->Contains(vPos))
    {
        if (pNode->GetChildren().empty())
            return pNode;
        else
        {
            for (_uint iIndex = 0; iIndex < CQuadTreeNode::m_iChild_Node_Count; ++iIndex)
            {
                CQuadTreeNode* pChild = GetCurrentNodeByPos(vPos, pNode->GetChildNode(iIndex));
                if (pChild)
                    return pChild;
            }

            return pNode;
        }
    }
    else
        return nullptr;
}

CQuadTreeNode* CQuadTree::BuildQuadTree(Vec3 vCenter, Vec3 vHalfExtents, _int iDepthLimit)
{
    if (iDepthLimit < 0)
        return nullptr;

    CQuadTreeNode*  pNode = new CQuadTreeNode;
    BoundingBox*    pBBox = pNode->GetBoundingBox();

    //Vec3           vCorner[4];

    //for (_uint i = 0; i < 4; ++i)
    //{
    //    *((_float*)(&vCorner[i]) + 0) = ((i & 1) ? vHalfExtents.x : -vHalfExtents.x) + *((_float*)(&vCenter) + 0);
    //    //*((_float*)(&vCorner[i]) + 1) = ((i & 4) ? vHalfExtents.y : -vHalfExtents.y) + *((_float*)(&vCenter) + 1);
    //    *((_float*)(&vCorner[i]) + 2) = ((i & 2) ? vHalfExtents.z : -vHalfExtents.z) + *((_float*)(&vCenter) + 2);
    //}

    //pNode->SetCorners(vCorner);
    pNode->SetPosition(vCenter);
    pBBox->Extents = vHalfExtents;
    
    Vec3 vOffset;
    Vec3 vChildCenter;
    Vec3 vStep = vHalfExtents * 0.5f;

    for (_uint iTree = 0; iTree < CQuadTreeNode::m_iChild_Node_Count; ++iTree)
    {
        *((_float*)(&vOffset) + 0) = ((iTree & 1) ? vStep.x : -vStep.x);
        *((_float*)(&vOffset) + 2) = ((iTree & 2) ? vStep.z : -vStep.z);

        *((_float*)(&vChildCenter) + 0) = *((_float*)(&vOffset) + 0) + *((_float*)(&vCenter) + 0);
        *((_float*)(&vChildCenter) + 2) = *((_float*)(&vOffset) + 2) + *((_float*)(&vCenter) + 2);

        pNode->AddChildNode(BuildQuadTree(vChildCenter, vStep, iDepthLimit - 1));
    }

#ifdef _DEBUG
    //pNode->InitBoundingBoxVisible();
#endif

    return pNode;
}

void CQuadTree::FrustumCull(CQuadTreeNode* pNode)
{
    switch (m_tFrustum.Contains(*pNode->GetBoundingBox()))
    {
    case CONTAINS:
        pNode->CullNode(CONTAINS);
        return;
    case INTERSECTS:
        pNode->CullNode(INTERSECTS);
        break;
    case DISJOINT:
        pNode->CullNode(DISJOINT);
        return;
    }

    vector<CQuadTreeNode*>& vecChildren = pNode->GetChildren();

    if (!vecChildren.empty())
        for (_int i = 0; i < 4; ++i)
            FrustumCull(vecChildren[i]);
}

void CQuadTree::AddObjectInNode(CTransform* pTransform, CQuadTreeNode* const pNode)
{
    Vec3 vTransformCenter = pTransform->GetPosition();
    Vec3 vNodeCenter = pNode->GetPosition();

    Vec3 vExtents = pNode->GetBoundingBox()->Extents;
    vExtents *= m_fLooseFactor;
    
    if (vTransformCenter.x < vNodeCenter.x - vExtents.x || vTransformCenter.x > vNodeCenter.x + vExtents.x
        || vTransformCenter.z < vNodeCenter.z - vExtents.z || vTransformCenter.z > vNodeCenter.z + vExtents.z)
        return;

    pNode->AddObject(pTransform->GetGameObject());

    if (pNode->GetChildren().empty())
        return;

    for (_uint index = 0; index < CQuadTreeNode::m_iChild_Node_Count; ++index)
        AddObjectInNode(pTransform, pNode->GetChildNode(index));
}

void CQuadTree::Free()
{
    //Safe_Delete(m_tBoundBox);

    //if (!m_vecChildren.empty())
    //{
    //	/*for (auto& iter : m_vecChildren)
    //	{
    //		iter->Free();
    //		Safe_Delete(iter);
    //	}*/
    //	m_vecObjects.clear();
    //	m_vecChildren.clear();
    //}
}