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
		//키가 눌렸고 체킹된적 없다.
		if (m_byKeyState[ubyKeyID]&0x80 && !m_bKeyCheck_main[ubyKeyID])
		{
			m_bKeyCheck_sub[ubyKeyID] = true;
			return KEYDOWN;
		}//키가 눌렸고 체킹된적 있다.
		else if(m_byKeyState[ubyKeyID] & 0x80 && m_bKeyCheck_main[ubyKeyID])
		{
			return KEYPRESS;
		}//키가 체킹된적 있지만 눌리진 않았다.
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