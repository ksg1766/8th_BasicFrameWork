#pragma once
#include "Base.h"

BEGIN(Engine)

class CGameObject;

class ENGINE_DLL CQuadTreeNode :
    public CBase
{
public:
    CQuadTreeNode();
   // CQuadTreeNode();
    CQuadTreeNode(const CGameObject& rhs) = delete;
    virtual ~CQuadTreeNode() = default;

public:
    //HRESULT               InitBoundingBoxVisible();

    void                    AddChildNode(CQuadTreeNode* pChild);
    void                    AddObject(CGameObject* pObject) { m_vecObjects.push_back(pObject); }

    Vec3                    GetPosition()                   { return m_vPosition; }
    CQuadTreeNode* const    GetParent()                     { return m_pParent; }
    vector<CQuadTreeNode*>& GetChildren()                   { return m_vecChildren; }
    CQuadTreeNode* const    GetChildNode(_uint _iChildNum)  { return m_vecChildren[_iChildNum]; }

    vector<CGameObject*>&   GetObjectList()                 { return m_vecObjects; }
    BoundingBox* const      GetBoundingBox()                { return &m_tBoundBox; }

    void                    SetPosition(Vec3 _vPos)         { m_vPosition = _vPos; }
    void                    SetParent(CQuadTreeNode* const _pParent) { m_pParent = _pParent; }

    void                    Render_QuadTreeNode();

    void                    CullNode(_int _iCulled)         { m_iCulled = _iCulled; }
    _int                    IsCulled()                      { return m_iCulled; }

public:
    const static _uint      m_iChild_Node_Count = 4;

private:
    BoundingBox             m_tBoundBox;

    vector<CQuadTreeNode*>  m_vecChildren;
    vector<CGameObject*>    m_vecObjects;

    CQuadTreeNode*          m_pParent;
    Vec3                    m_vPosition;

    _int                    m_iCulled;

    // Only Used in Debug mode
    // LPDIRECT3DVERTEXBUFFER9 m_pVB;
    // LPDIRECT3DINDEXBUFFER9	m_pIB;

public:
    virtual void Free() override;
};

END