#pragma once

/* .FBX������ �ε��Ͽ� �����͵��� �� ������ �°� �����Ѵ�. */
#include "VIBuffer.h"

BEGIN(Engine)

class ENGINE_DLL CModel final : public CComponent
{
	using Super = CComponent;
private:
	CModel(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CModel(const CModel& rhs);
	virtual ~CModel() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	HRESULT			InitializeWithFile(const wstring& pModelFilePath);
	void			DebugRender()	override;
	HRESULT			Render();

private: /* .fbx������ ��� �о��ִ� ��Ȱ */
	Assimp::Importer			m_Importer; 

	/* m_Importer�� �о��� �����͵��� �����Ѵ�. */
	const aiScene*				m_pAIScene = { nullptr };

private:
	_uint						m_iNumMeshes = { 0 };
	vector<class CMesh*>		m_Meshes;	

private:
	HRESULT Ready_Meshes();

public:
	static	CModel* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CComponent* Clone(CGameObject* pGameObject, void* pArg) override;
	virtual void Free() override;
};

END