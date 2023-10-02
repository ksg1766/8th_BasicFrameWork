#pragma once

/* ���� Ư�� ������ �ǹ��ϴ� �޽�. */
/* �� ����(�޽�)���� ����, �ε������۸� ������. */
#include "VIBuffer.h"

BEGIN(Engine)

class CMesh final : public CVIBuffer
{
	using Super = CVIBuffer;
private:
	CMesh(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CMesh(const CMesh& rhs);
	virtual ~CMesh() = default;

public:
	virtual HRESULT Initialize_Prototype(const aiMesh* pAIMesh, _fmatrix& matPivot);
	virtual HRESULT Initialize(void* pArg) override;

public:
	_uint	Get_MaterialIndex() const { return m_iMaterialIndex; }

private:
	_uint	m_iMaterialIndex = { 0 };

public:
	static CMesh* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const aiMesh* pAIMesh, _fmatrix& matPivot);
	virtual CComponent* Clone(CGameObject* pGameObject, void* pArg) override;
	virtual void Free() override;
};

END