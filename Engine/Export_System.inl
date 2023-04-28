LPDIRECT3DDEVICE9		Get_GraphicDev(void)
{
	return CGraphicDev::GetInstance()->Get_GraphicDev();
}
HRESULT					Ready_GraphicDev(HWND hWnd, WINMODE eMode, const _uint& iSizeX, const _uint& iSizeY, CGraphicDev** ppGraphicDev)
{
	return CGraphicDev::GetInstance()->Ready_GraphicDev(hWnd, eMode, iSizeX, iSizeY, ppGraphicDev);
}

void					Render_Begin(D3DXCOLOR Color)
{
	CGraphicDev::GetInstance()->Render_Begin(Color);
}
void					Render_End(void)
{
	CGraphicDev::GetInstance()->Render_End();
}


_float		Get_Timer(const _tchar* pTimerTag)
{
	return CTimerMgr::GetInstance()->Get_Timer(pTimerTag);
}
void		Set_Timer(const _tchar* pTimerTag)
{
	CTimerMgr::GetInstance()->Set_Timer(pTimerTag);
}
HRESULT		Ready_Timer(const _tchar* pTimerTag)
{
	return CTimerMgr::GetInstance()->Ready_Timer(pTimerTag);
}

// FrameMgr

HRESULT		Ready_Frame(const _tchar* pFrameTag, const _float& fTimeDelta)
{
	return CFrameMgr::GetInstance()->Ready_Frame(pFrameTag, fTimeDelta);
}
_bool		IsPermit_Call(const _tchar * pFrameTag, const _float& fTimeDelta)
{
	return CFrameMgr::GetInstance()->IsPermit_Call(pFrameTag, fTimeDelta);
}
HRESULT		FrameResetToZero(const _tchar * pFrameTag)
{
	return CFrameMgr::GetInstance()->FrameResetToZero(pFrameTag);
}

//FOntMgr

HRESULT			Ready_Font(LPDIRECT3DDEVICE9 pGraphicDev,
	const _tchar* pFontTag,	 // 키값
	const _tchar* pFontType, // 서체
	const _uint& iWidth,
	const _uint& iHeight,
	const _uint& iWeight)
{
	return CFontMgr::GetInstance()->Ready_Font(pGraphicDev, pFontTag, pFontType, iWidth, iHeight, iWeight);
}

void	Render_Font(const _tchar* pFontTag,
	const _tchar* pString,
	const _vec2* pPos,
	D3DXCOLOR Color)
{
	CFontMgr::GetInstance()->Render_Font(pFontTag, pString, pPos, Color);
 }

// Dinput
_byte		Get_DIKeyState(_ubyte ubyKeyID)
{
	return CDInputMgr::GetInstance()->Get_DIKeyState(ubyKeyID);
}
_byte		Get_DIMouseState(MOUSEKEYSTATE eMouseID)
{
	return CDInputMgr::GetInstance()->Get_DIMouseState(eMouseID);
}
_long		Get_DIMouseMove(MOUSEMOVESTATE eMouseMoveID)
{
	return CDInputMgr::GetInstance()->Get_DIMouseMove(eMouseMoveID);
}
HRESULT		Ready_DInput(HINSTANCE hInst, HWND hWnd)
{
	return CDInputMgr::GetInstance()->Ready_DInput(hInst, hWnd);
}
void			Update_DInput(void)
{
	CDInputMgr::GetInstance()->Update_DInput();
}

//Sound
HRESULT Ready_Sound()
{
	return CSoundMgr::GetInstance()->Ready_Sound();
}

HRESULT Ready_Sound_Effect()
{
	return CSoundMgr::GetInstance()->Ready_Sound_Effect();
}
void PlaySound(TCHAR* pSoundKey, CHANNELID eID, float fVolume)
{
	return CSoundMgr::GetInstance()->PlaySound(pSoundKey, eID, fVolume);
}
void PlaySound_Effect(TCHAR* pSoundKey, CHANNELID eID, float fVolume)
{
	return CSoundMgr::GetInstance()->PlaySound_Effect(pSoundKey, eID, fVolume);
}
void PlayBGM(TCHAR* pSoundKey, float fVolume)
{
	return CSoundMgr::GetInstance()->PlayBGM(pSoundKey, fVolume);
}

void StopSound(CHANNELID eID)
{
	return CSoundMgr::GetInstance()->StopSound(eID);
}

void StopAll()
{
	return CSoundMgr::GetInstance()->StopAll();
}

void SetChannelVolume(CHANNELID eID, float fVolume)
{
	return CSoundMgr::GetInstance()->SetChannelVolume(eID, fVolume);
}



void	Release_System(void)
{
	CDInputMgr::GetInstance()->DestroyInstance();
	CFontMgr::GetInstance()->DestroyInstance();
	CFrameMgr::GetInstance()->DestroyInstance();
	CTimerMgr::GetInstance()->DestroyInstance();
	CSoundMgr::GetInstance()->DestroyInstance();
	CGraphicDev::GetInstance()->DestroyInstance();
}