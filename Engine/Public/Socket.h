#pragma once

#include "Base.h"

BEGIN(Engine)

class ENGINE_DLL CSocket final : public CBase
{
private:
	CSocket();
	CSocket(const CSocket& rhs) = delete;
	virtual ~CSocket() = default;

public:
	HRESULT Initialize(const _char* szName);	// TODO: ���� ���� ���� �� Ȯ�� �� �� �͵� :
	HRESULT Tick(const _char* szName);	// Ʈ������ ���� Ʈ���� ������ �������� ���� ���� ������� �ʾƵ� �ȴ�.
										// �ƿ� ������ �ؽ��Ŀ� srv�� ����, ���� �ؽ��Ŀ� srv�� ���ε��� �� ���ε����� �����ϸ� �ؽ��ĸ� �ȵ�� �־ �ɱ�?
	HRESULT	Render();					// ������ ������ srv�� ���� ���Ͽ��� srv ���ε� �� ȣ���ϵ��� �����ϸ� ���

public:									// ���� private���� �س��� ���� Ư�� �Լ����� friend �� ���� ���°� ���?
	HRESULT			Load_VTF(ID3D11Texture2D* pSocketTexture, ID3D11ShaderResourceView* pSocketSRV);

public:
	const _char*	Get_Name() const { return m_szName; }

private:
	_char			m_szName[MAX_PATH] = "";

	class CGameObject*			m_pPartsObject = nullptr;

	ID3D11Texture2D*			m_pSocketTexture = nullptr;
	ID3D11ShaderResourceView*	m_pSocketSRV = nullptr;

public:
	static CSocket* Create(const _char* strName);	// TODO: Model Ŭ������ Create���� Socket ������ �� ��ġ �����ϵ���.
	virtual void Free();
};

END