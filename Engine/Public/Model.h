#pragma once

/* .FBX파일을 로드하여 데이터들을 내 구조에 맞게 정리한다. */
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

private: /* .fbx파일을 열어서 읽어주는 역활 */
	Assimp::Importer			m_Importer; 

	/* m_Importer가 읽어준 데이터들을 보관한다. */
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