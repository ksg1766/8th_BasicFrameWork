#pragma once

#include "Base.h"
#include "AsTypes.h"
#include "FileUtils.h"

BEGIN(Engine)

class CAnimation final : public CBase
{
private:
	CAnimation();
	CAnimation(const CAnimation& rhs);
	virtual ~CAnimation() = default;

public:
	HRESULT Initialize_Prototype();
	HRESULT Initialize(class CModel* pModel);
	HRESULT	LoadData_FromAnimationFile(FileUtils* pFileUtils, Matrix PivotMatrix);
	HRESULT	LoadData_FromConverter(shared_ptr<asAnimation> pAnimation, Matrix PivotMatrix);

	HRESULT Play_Animation(_float fTimeDelta);

	KEY_DESC Get_KeyDesc()				{ return m_tKeyDesc; }
	wstring	 Get_Name()					{ return m_szName; }
	_float	 Get_MaxFrameRatio();
	void	 Set_Speed(_float fSpeed)	{ m_fSpeed = fSpeed; }
	_uint	 Get_Frame()				{ return m_tKeyDesc.iCurrFrame; }
	void	 Set_Frame(_uint iFrame);
	
	vector<ModelKeyframeData>& Get_KeyDatas(_uint iFrame) { return m_KeyFrameBones[iFrame]; }
	HRESULT Make_KeyframeData(vector<shared_ptr<ModelKeyframe>>& KeyFrames);

	HRESULT SetUpAnimation_OnShader(class CShader* pShader, const char* strMapname);

	_bool	Is_End()					{ return m_bEnd; }
	void	Reset_End()					{ m_bEnd = false; }
	void	Reset_Animation();
private:

private:
	wstring						m_szName;

	/* �� �ִϸ��̼��� �����ϱ����� ���Ǵ� ���� ����. */

	/* �ִϸ��̼� ����ϴµ� �ɸ��� ��ü�ð�. */
	_float						m_fDuration = 0.f;

	/* �ִϸ��̼��� �ʴ� ��� �ӵ�. */
	_float						m_fTickPerSecond = 0.f;
	_float						m_fPlayTime = 0.f;
	_uint						m_iFrameCount;
	_float						m_fSpeed = 1.f;

	KEY_DESC					m_tKeyDesc;

private: /* ������ �ִϸ��̼� ���� ���� ������. */
	_uint						m_iBoneSize;

	_bool						m_bEnd = false;

	vector<vector<ModelKeyframeData>> m_KeyFrameBones;

public:
	static CAnimation* Create();
	CAnimation* Clone(class CModel* pModel);
	virtual void Free() override;
};

END