#pragma once

#include "VIBuffer.h"

BEGIN(Engine)

class ENGINE_DLL CTerrain final : public CVIBuffer
{
	using Super = CVIBuffer;
private:
	CTerrain(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CTerrain(const CTerrain& rhs);
	virtual ~CTerrain() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg) override;
	HRESULT			InitializeWithHeightMap(const wstring& strHeightMapPath);

public:
	
private:
	_ulong			m_iNumVerticesX = { 0 };
	_ulong			m_iNumVerticesZ = { 0 };
	FACEINDICES32*	m_pFaceIndices = nullptr;

public:
	static CTerrain* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CComponent* Clone(CGameObject* pGameObject, void* pArg) override;
	virtual void Free() override;
};

END