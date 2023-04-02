#pragma once

#include "Base.h"
#include "Include.h"

BEGIN(Engine)

class  CDInputMgr : public CBase
{
	DECLARE_SINGLETON(CDInputMgr)

private:
	explicit CDInputMgr();
	virtual ~CDInputMgr();

public:
	Engine::KEYSTATE	Get_DIKeyState(_ubyte ubyKeyID) 
	{
		//Ű�� ���Ȱ� üŷ���� ����.
		if (m_byKeyState[ubyKeyID]&0x80 && !m_bKeyCheck_main[ubyKeyID])
		{
			m_bKeyCheck_sub[ubyKeyID] = true;
			return KEYDOWN;
		}//Ű�� ���Ȱ� üŷ���� �ִ�.
		else if(m_byKeyState[ubyKeyID] & 0x80 && m_bKeyCheck_main[ubyKeyID])
		{
			return KEYPRESS;
		}//Ű�� üŷ���� ������ ������ �ʾҴ�.
		else if(!(m_byKeyState[ubyKeyID] & 0x80) && m_bKeyCheck_main[ubyKeyID])
		{
			m_bKeyCheck_sub[ubyKeyID] = false;
			return KEYUP;
		}
		return KEYNONE;
	}
	_byte		Get_DIMouseState(MOUSEKEYSTATE eMouseID)
	{
		return m_MouseState.rgbButtons[eMouseID];
	}

	_long		Get_DIMouseMove(MOUSEMOVESTATE eMouseMoveID)
	{
		return *(((_long*)&m_MouseState) + eMouseMoveID);
	}


public:
	HRESULT			Ready_DInput(HINSTANCE hInst, HWND hWnd);
	void			Update_DInput(void);

private:
	LPDIRECTINPUT8				m_pInputSDK;

	LPDIRECTINPUTDEVICE8		m_pKeyBoard;
	LPDIRECTINPUTDEVICE8		m_pMouse;

private:
	_byte					m_byKeyState[256];
	_bool					m_bKeyCheck_sub[256];
	_bool					m_bKeyCheck_main[256];
	 DIMOUSESTATE			m_MouseState;

public:
	virtual void		Free(void);

};

END