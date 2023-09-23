#include "QuadTree.h"
#include "QuadTreeNode.h"
#include "ObjectManager.h"
#include "GameObject.h"
#include "Transform.h"
#include "Layer.h"
#include "PipeLine.h"

IMPLEMENT_SINGLETON(CQuadTree)

CQuadTree::CQuadTree()
{
}

HRESULT CQuadTree::Build_QuadTree(_uint iNumLevels)
{
    m_pPipeLine = GET_INSTANCE(CPipeLine);
   
    CObjectManager* pObjectManager = GET_INSTANCE(CObjectManager);

    m_pQuadTreeRoot = BuildQuadTree(Vec3(0.f, m_vRootExtents.y, 0.f), 0.5f * m_vRootExtents, m_iDepthLimit);

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
    BoundingFrustum tFrustum;
    Update_Frustum(tFrustum);
    FrustumCull(tFrustum, m_pQuadTreeRoot);
}

void CQuadTree::Update_Frustum(BoundingFrustum& tFrustum)
{
    BoundingFrustum::CreateFromMatrix(tFrustum, m_pPipeLine->Get_Transform_Matrix(CPipeLine::D3DTS_PROJ));
    tFrustum.Transform(tFrustum, m_pPipeLine->Get_Transform_Matrix_Inverse(CPipeLine::D3DTS_VIEW));
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

    pBBox->Center = vCenter;
    pBBox->Extents = vHalfExtents;
    
    Vec3 vOffset;
    Vec3 vChildCenter;
    Vec3 vStep = Vec3(0.5f * vHalfExtents.x, vHalfExtents.y, 0.5f * vHalfExtents.z);

    for (_uint iTree = 0; iTree < CQuadTreeNode::m_iChild_Node_Count; ++iTree)
    {
        *((_float*)(&vOffset) + 0) = ((iTree & 1) ? vStep.x : -vStep.x);
        *((_float*)(&vOffset) + 2) = ((iTree & 2) ? vStep.z : -vStep.z);

        *((_float*)(&vChildCenter) + 0) = *((_float*)(&vOffset) + 0) + *((_float*)(&pBBox->Center) + 0);
        *((_float*)(&vChildCenter) + 1) = *((_float*)(&pBBox->Center) + 1);
        *((_float*)(&vChildCenter) + 2) = *((_float*)(&vOffset) + 2) + *((_float*)(&pBBox->Center) + 2);

        pNode->AddChildNode(BuildQuadTree(vChildCenter, vStep, iDepthLimit - 1));
    }

#ifdef _DEBUG
    pNode->InitDebugSquare();
#endif

    return pNode;
}

void CQuadTree::FrustumCull(BoundingFrustum& tFrustum, CQuadTreeNode* pNode)
{
    switch (tFrustum.Contains(*pNode->GetBoundingBox()))
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
            FrustumCull(tFrustum, vecChildren[i]);
}

void CQuadTree::AddObjectInNode(CTransform* pTransform, CQuadTreeNode* const pNode)
{
    Vec3 vTransformCenter = pTransform->GetPosition();
    Vec3 vNodeCenter = pNode->GetBoundingBox()->Center;

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
    m_pQuadTreeRoot->Free();
    Safe_Delete(m_pQuadTreeRoot);

    RELEASE_INSTANCE(CPipeLine);
}