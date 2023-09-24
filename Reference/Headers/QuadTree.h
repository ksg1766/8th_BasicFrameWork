#pragma once
#include "Base.h"

BEGIN(Engine)

//class CFrustum;
class CQuadTreeNode;
class CTransform;

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
    CQuadTreeNode*  GetCurrentNodeByPos(Vec3 vPos, CQuadTreeNode* const pNode);

private:
    CQuadTreeNode*  BuildQuadTree(Vec3 vCenter, Vec3 vHalfWidth, _int iDepthLimit = 5);
    void            AddObjectInNode(CTransform* pTransform, CQuadTreeNode* const pNode);

    void            Update_Frustum(BoundingFrustum& tFrustum);
    void	        FrustumCull(BoundingFrustum& tFrustum, CQuadTreeNode* pNode);

private:
    class CPipeLine* m_pPipeLine;

    const _int      m_iDepthLimit = 5;
    const _float    m_fLooseFactor = 2.f;

    Vec3            m_vRootExtents = Vec3(1024.f, 0.f, 1024.f);
    CQuadTreeNode*  m_pQuadTreeRoot = nullptr;

public:
    virtual void Free() override;
};

END