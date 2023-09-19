#pragma once
#include "Base.h"
#include "TransformEx.h"

BEGIN(Engine)

//class CFrustum;
class CQuadTreeNode;

class CQuadTree final : public CBase
{
    DECLARE_SINGLETON(CQuadTree)

private:
    CQuadTree();
    virtual ~CQuadTree() = default;

public:
    HRESULT			Build_QuadTree(_uint iNumLevels);
    void            Update_QuadTree();
    void            Render_QuadTree();

    CQuadTreeNode*  GetCQuadTreeRoot() { return m_pQuadTreeRoot; }
    //CQuadTreeNode*  GetParentNodeByPos(Vec3 vPos, CQuadTreeNode* const pNode);
    CQuadTreeNode*  GetCurrentNodeByPos(Vec3 vPos, CQuadTreeNode* const pNode);

private:
    CQuadTreeNode*  BuildQuadTree(Vec3 vCenter, Vec3 vHalfWidth, _int iDepthLimit = 5);
    void            AddObjectInNode(CTransformEx* pTransform, CQuadTreeNode* const pNode);

    void            Update_Frustum();
    void	        FrustumCull(CQuadTreeNode* pNode);

private:
    const _int      m_iDepthLimit = 5;
    const _float    m_fLooseFactor = 2.f;

    Vec3            m_vRootExtents = Vec3(256.f, 128.f, 256.f);
    CQuadTreeNode*  m_pQuadTreeRoot = nullptr;

    BoundingFrustum m_tFrustum;

public:
    virtual void Free() override;
};

END