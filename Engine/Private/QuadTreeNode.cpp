#include "QuadTreeNode.h"
#include "GameObject.h"

CQuadTreeNode::CQuadTreeNode()
{
}

void CQuadTreeNode::AddChildNode(CQuadTreeNode* pChild)
{
    if (pChild)
    {
        m_vecChildren.push_back(pChild);
        pChild->SetParent(this);
    }
}

void CQuadTreeNode::Render_QuadTreeNode()
{
	if (!m_vecChildren.empty())
	{
		for (auto& iter : m_vecChildren)
		{
			if (0 == iter->IsCulled())
				continue;
			else if (1 == iter->IsCulled())
				iter->Render_QuadTreeNode();
			else
			{
				for (auto& _iter : iter->GetObjectList())
					_iter->Render();
				continue;
			}
		}
	}
}

void CQuadTreeNode::Free()
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