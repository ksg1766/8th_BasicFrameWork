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
	HRESULT Initialize(const _char* szName);	// TODO: 내일 정신 맑을 때 확인 해 볼 것들 :
	HRESULT Tick(const _char* szName);	// 트위닝은 모델의 트위닝 정보를 가져오면 굳이 따로 계싼하지 않아도 된다.
										// 아예 소켓의 텍스쳐와 srv를 빼고, 모델의 텍스쳐와 srv를 바인딩한 후 뼈인덱스로 접근하면 텍스쳐를 안들고 있어도 될까?
	HRESULT	Render();					// 무기의 렌더는 srv를 가진 소켓에서 srv 바인딩 후 호출하도록 위임하면 어떨까

public:									// 전부 private으로 해놓고 모델의 특정 함수에만 friend 로 열어 보는건 어떨까?
	HRESULT			Load_VTF(ID3D11Texture2D* pSocketTexture, ID3D11ShaderResourceView* pSocketSRV);

public:
	const _char*	Get_Name() const { return m_szName; }

private:
	_char			m_szName[MAX_PATH] = "";

	class CGameObject*			m_pPartsObject = nullptr;

	ID3D11Texture2D*			m_pSocketTexture = nullptr;
	ID3D11ShaderResourceView*	m_pSocketSRV = nullptr;

public:
	static CSocket* Create(const _char* strName);	// TODO: Model 클래스의 Create에서 Socket 사이즈 및 위치 전달하도록.
	virtual void Free();
};

END