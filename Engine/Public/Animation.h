#pragma once

#include "Base.h"

BEGIN(Engine)

class CAnimation final : public CBase
{
private:
	CAnimation();
	CAnimation(const CAnimation& rhs);
	virtual ~CAnimation() = default;

public:
	HRESULT Initialize_Prototype(const _float& fDuration, const _float& fTickPerSecond, vector<class CChannel*>& Channels);
	HRESULT Initialize(class CModel* pModel);

public:
	HRESULT Play_Animation(_float fTimeDelta);

private:
	vector<class CChannel*>		m_Channels;			/* ���Ǵ� ä�� ���� */

	_float						m_fDuration = 0.f;	/* ��ü ��� �ð� */
	_float						m_fPlayTime = 0.f;	/* ���� ��� �ð� */
	_float						m_fTickPerSecond = 0.f; /* �ʴ� ��� �ӵ� */

	vector<class CBone*>		m_Bones;			/* �� �ִϸ��̼��� ����ϴµ� �ʿ��� ���� */
	vector<_uint>				m_ChannelKeyFrames;

public:
	static CAnimation* Create(const _float& fDuration, const _float& fTickPerSecond, vector<class CChannel*>& Channels);
	CAnimation* Clone(class CModel* pModel);
	virtual void Free() override;
};

END