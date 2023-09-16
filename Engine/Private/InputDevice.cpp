#include "..\Public\InputDevice.h"

IMPLEMENT_SINGLETON(CInputDevice)

CInputDevice::CInputDevice()
{
}

_bool CInputDevice::Key_Down(_ubyte eKeyID)
{
	if (m_PreKeyState[eKeyID] != m_CurKeyState[eKeyID])
	{
		if (m_CurKeyState[eKeyID] & 0x80)
			return true;
	}
	return false;
}

_bool CInputDevice::Key_Up(_ubyte eKeyID)
{
	if (m_PreKeyState[eKeyID] != m_CurKeyState[eKeyID])
	{
		if (m_PreKeyState[eKeyID] & 0x80)
			return true;
	}
	return false;
}

_bool CInputDevice::Key_Pressing(_ubyte eKeyID)
{
	if (m_PreKeyState[eKeyID] == m_CurKeyState[eKeyID])
	{
		if (m_PreKeyState[eKeyID] & 0x80)
			return true;
	}
	return false;
}

_bool CInputDevice::Mouse_Down(MOUSEKEYSTATE eMouseKeyID)
{
	if (m_PreMouseState.rgbButtons[eMouseKeyID] != m_CurMouseState.rgbButtons[eMouseKeyID])
	{
		if (m_CurMouseState.rgbButtons[eMouseKeyID] & 0x80)
			return true;
	}
	return false;
}

_bool CInputDevice::Mouse_Up(MOUSEKEYSTATE eMouseKeyID)
{
	if (m_PreMouseState.rgbButtons[eMouseKeyID] != m_CurMouseState.rgbButtons[eMouseKeyID])
	{
		if (m_PreMouseState.rgbButtons[eMouseKeyID] & 0x80)
			return true;
	}
	return false;
}

_bool CInputDevice::Mouse_Pressing(MOUSEKEYSTATE eMouseKeyID)
{
	if (m_PreMouseState.rgbButtons[eMouseKeyID] == m_CurMouseState.rgbButtons[eMouseKeyID])
	{
		if (m_CurMouseState.rgbButtons[eMouseKeyID] & 0x80)
			return true;
	}
	return false;
}

HRESULT CInputDevice::Initialize(HINSTANCE hInst, HWND hWnd)
{
	/* m_pInputSDK */
	if (FAILED(DirectInput8Create(hInst, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&m_pInputSDK, NULL)))
		return E_FAIL;

	if (FAILED(m_pInputSDK->CreateDevice(GUID_SysKeyboard, &m_pKeyBoard, nullptr)))
		return E_FAIL;

	if (FAILED(m_pKeyBoard->SetDataFormat(&c_dfDIKeyboard)))
		return E_FAIL;
	if (FAILED(m_pKeyBoard->SetCooperativeLevel(hWnd, DISCL_BACKGROUND | DISCL_NONEXCLUSIVE)))
		return E_FAIL;
	if (FAILED(m_pKeyBoard->Acquire()))
		return E_FAIL;

	::ZeroMemory(m_PreKeyState, sizeof(m_PreKeyState));

	if (FAILED(m_pInputSDK->CreateDevice(GUID_SysMouse, &m_pMouse, nullptr)))
		return E_FAIL;

	if (FAILED(m_pMouse->SetDataFormat(&c_dfDIMouse)))
		return E_FAIL;
	if (FAILED(m_pMouse->SetCooperativeLevel(hWnd, DISCL_BACKGROUND | DISCL_NONEXCLUSIVE /*| DISCL_NOWINKEY*/)))
		return E_FAIL;
	if (FAILED(m_pMouse->Acquire()))
		return E_FAIL;

	::ZeroMemory(&m_PreMouseState, sizeof(m_PreMouseState));

	return S_OK;
}

void CInputDevice::Update()
{
	::CopyMemory(&m_PreKeyState, &m_CurKeyState, sizeof(_char) * 256);
	m_pKeyBoard->GetDeviceState(256, m_CurKeyState);
	::CopyMemory(&m_PreMouseState, &m_CurMouseState, sizeof(DIMOUSESTATE));
	m_pMouse->GetDeviceState(sizeof(DIMOUSESTATE), &m_CurMouseState);
}

void CInputDevice::Free()
{
	Safe_Release(m_pKeyBoard);
	Safe_Release(m_pMouse);

	Safe_Release(m_pInputSDK);
}
