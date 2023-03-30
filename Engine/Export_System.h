#pragma once

#include "GraphicDev.h"
#include "TimerMgr.h"
#include "FrameMgr.h"
#include "FontMgr.h"
#include "DInputMgr.h"

BEGIN(Engine)

// GraphicDev
inline  LPDIRECT3DDEVICE9		Get_GraphicDev(void);
inline 	HRESULT					Ready_GraphicDev(HWND hWnd, WINMODE eMode, const _uint& iSizeX, const _uint& iSizeY, CGraphicDev** ppGraphicDev);

inline 	void					Render_Begin(D3DXCOLOR Color);
inline 	void					Render_End(void);

// TimerMgr
inline _float		Get_Timer(const _tchar* pTimerTag);
inline void		Set_Timer(const _tchar* pTimerTag);
inline  HRESULT		Ready_Timer(const _tchar* pTimerTag);

// FrameMgr

inline HRESULT		Ready_Frame(const _tchar* pFrameTag, const _float& fTimeDelta);
inline _bool		IsPermit_Call(const _tchar * pFrameTag, const _float& fTimeDelta);

// FontMgr
inline HRESULT			Ready_Font(LPDIRECT3DDEVICE9 pGraphicDev,
	const _tchar* pFontTag,	 // 키값
	const _tchar* pFontType, // 서체
	const _uint& iWidth,
	const _uint& iHeight,
	const _uint& iWeight);

inline void	Render_Font(const _tchar* pFontTag,
	const _tchar* pString,
	const _vec2* pPos,
	D3DXCOLOR Color);

// Dinput
inline _byte		Get_DIKeyState(_ubyte ubyKeyID);
inline _byte		Get_DIMouseState(MOUSEKEYSTATE eMouseID);
inline _long		Get_DIMouseMove(MOUSEMOVESTATE eMouseMoveID);
inline HRESULT		Ready_DInput(HINSTANCE hInst, HWND hWnd);
inline void			Update_DInput(void);

inline void						Release_System(void);

#include "Export_System.inl"

END