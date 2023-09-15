#include "..\Public\TransformEx.h"

CTransformEx::CTransformEx(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CComponent(pDevice, pContext, ComponentType::Transform)
{
}

CTransformEx::CTransformEx(const CTransformEx& rhs)
	: CComponent(rhs)
{
}

void CTransformEx::Set_State(STATE eState, _fvector vState)
{
	_matrix		StateMatrix;

	StateMatrix = XMLoadFloat4x4(&m_WorldMatrix);

	StateMatrix.r[eState] = vState;

	XMStoreFloat4x4(&m_WorldMatrix, StateMatrix);
}

HRESULT CTransformEx::Initialize_Prototype()
{
	/* XMFloat4x4 -> XMMatrix*/
	/*XMLoadFloat4x4(&m_WorldMatrix);*/

	/* XMMatrix -> XMFloat4x4*/
	XMStoreFloat4x4(&m_WorldMatrix, XMMatrixIdentity());

	/*_float3	vTmp;
	_vector		vTmp1;

	vTmp1 = XMLoadFloat3(&vTmp);

	XMStoreFloat3(&vTmp, vTmp1);*/

	return S_OK;
}

HRESULT CTransformEx::Initialize(void* pArg)
{
	if (nullptr != pArg)
	{
		TRANSFORM_DESC	TransformDesc;

		memmove(&TransformDesc, pArg, sizeof TransformDesc);

		m_fSpeedPerSec = TransformDesc.fSpeedPerSec;
		m_fRotationRadianPerSec = TransformDesc.fRotationRadianPerSec;
	}

	return S_OK;
}

void CTransformEx::Go_Straight(_float fTimeDelta)
{
	/* vLook : 현재 내가 바라보는 방향 + z스케일정보를 가지낟. */
	/* vLook의 길이(크기)를 내가 원하는 초당 움직여하는 속도로 변경하자. */
	_vector		vLook = Get_State(STATE_LOOK);

	_vector		vPosition = Get_State(STATE_POSITION);

	/* vLook을 정규화(길이1)하고 원하는 길이(초당속도)를 곱한다. */
	vPosition += XMVector3Normalize(vLook) * m_fSpeedPerSec * fTimeDelta;

	Set_State(STATE_POSITION, vPosition);
}

void CTransformEx::Go_Backward(_float fTimeDelta)
{
	_vector		vLook = Get_State(STATE_LOOK);

	_vector		vPosition = Get_State(STATE_POSITION);

	vPosition -= XMVector3Normalize(vLook) * m_fSpeedPerSec * fTimeDelta;

	Set_State(STATE_POSITION, vPosition);
}

void CTransformEx::Go_Left(_float fTimeDelta)
{
	_vector		vRight = Get_State(STATE_RIGHT);

	_vector		vPosition = Get_State(STATE_POSITION);

	vPosition -= XMVector3Normalize(vRight) * m_fSpeedPerSec * fTimeDelta;

	Set_State(STATE_POSITION, vPosition);
}

void CTransformEx::Go_Right(_float fTimeDelta)
{
	_vector		vRight = Get_State(STATE_RIGHT);

	_vector		vPosition = Get_State(STATE_POSITION);

	vPosition += XMVector3Normalize(vRight) * m_fSpeedPerSec * fTimeDelta;

	Set_State(STATE_POSITION, vPosition);
}

void CTransformEx::Fix_Rotation(_fvector vAxis, _float fRadian)
{
}

void CTransformEx::Turn(_fvector vAxis, _float fTimeDelta)
{
}

CTransformEx* CTransformEx::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CTransformEx* pInstance = new CTransformEx(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CTransform");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CComponent* CTransformEx::Clone(void* pArg)
{
	CTransformEx* pInstance = new CTransformEx(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CTransformEx");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CTransformEx::Free()
{
	__super::Free();
}
