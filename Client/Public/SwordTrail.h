#pragma once

#include "Client_Defines.h"
#include "GameObject.h"

BEGIN(Engine)

class CTexture;
class CVIBuffer_Point;

END

BEGIN(Client)

class CSwordTrail final : public CGameObject
{
public:
	typedef struct tagCSwordTrailDesc
	{
		CModel*		pModel = nullptr;
		_float		fLifeTime = 0.3f;

		Matrix		matPreWorld = Matrix::Identity;
		Matrix		matCurWorld = Matrix::Identity;
		TWEENDESC	pPreTweenDesc;
		TWEENDESC	pCurTweenDesc;

		Matrix*		pMatOffsetTop = nullptr;
		Matrix*		pMatOffsetBottom = nullptr;

		_int		iBoneIndex = 0;

		/*Matrix		matPrePrePreWorld = Matrix::Identity;
		Matrix		matPrePreWorld = Matrix::Identity;
		TWEENDESC	tPrePreTweenDesc;
		TWEENDESC	tPrePrePreTweenDesc;*/

	}SWORDTRAIL_DESC;

	using Super = CGameObject;
private:
	CSwordTrail(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CSwordTrail(const CSwordTrail& rhs);
	virtual ~CSwordTrail() = default;

public:
	virtual HRESULT Initialize_Prototype()				override;
	virtual HRESULT Initialize(void* pArg)				override;
	virtual void	Tick(const _float& fTimeDelta)		override;
	virtual void	LateTick(const _float& fTimeDelta)	override;
	virtual void	DebugRender()						override;
	virtual HRESULT Render()							override;

private:
	CModel*			m_pModel = nullptr;

	CTexture*		m_pAlphaTexture = nullptr;

	_float			m_fLifeTime = 0.3f;
	Matrix			m_matPrePrePreWorld = Matrix::Identity;
	Matrix			m_matPrePreWorld = Matrix::Identity;
	Matrix			m_matPreWorld = Matrix::Identity;
	TWEENDESC		m_tPrePrePreTweenDesc;
	TWEENDESC		m_tPrePreTweenDesc;
	TWEENDESC		m_tPreTweenDesc;
	TWEENDESC		m_tCurTweenDesc;
	_int			m_iBoneIndex = 0;

	Matrix*			m_pOffsetTop = nullptr;
	Matrix*			m_pOffsetBottom = nullptr;

	Vec2			m_vDistortionOffset = Vec2(0.f, 0.f);

private:
	HRESULT			Bind_ShaderResources(); /* 셰이더 전역변수에 값 던진다. */
	HRESULT			Ready_FixedComponents();
	HRESULT			Ready_Scripts();

public:
	static	CSwordTrail* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override; /* 사본객체를 생성할때 원본데이터로부터 복제해올 데이터외에 더 추가해 줘야할 데이터가 있다라면 받아오겠다. */
	virtual void Free() override;
};

END