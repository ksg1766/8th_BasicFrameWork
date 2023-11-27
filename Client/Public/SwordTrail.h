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
		//TWEENDESC	pPrePreTweenDesc;
		//TWEENDESC	pPrePrePreTweenDesc;
		TWEENDESC	pCurTweenDesc;

		Matrix*		pMatOffsetTop = nullptr;
		Matrix*		pMatOffsetBottom = nullptr;

		_int		iBoneIndex = 0;

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
	Matrix			m_matPreWorld = Matrix::Identity;
	TWEENDESC		m_tPrePrePreTweenDesc;
	TWEENDESC		m_tPrePreTweenDesc;
	TWEENDESC		m_tPreTweenDesc;
	TWEENDESC		m_tCurTweenDesc;
	_int			m_iBoneIndex = 0;

	Matrix*			m_pOffsetTop = nullptr;
	Matrix*			m_pOffsetBottom = nullptr;

private:
	HRESULT			Bind_ShaderResources(); /* ���̴� ���������� �� ������. */
	HRESULT			Ready_FixedComponents();
	HRESULT			Ready_Scripts();

public:
	static	CSwordTrail* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override; /* �纻��ü�� �����Ҷ� ���������ͷκ��� �����ؿ� �����Ϳܿ� �� �߰��� ����� �����Ͱ� �ִٶ�� �޾ƿ��ڴ�. */
	virtual void Free() override;
};

END