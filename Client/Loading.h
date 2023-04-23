#pragma once

#include "Base.h"
#include "Include.h"

class CLoading : public CBase
{
public:
	explicit CLoading(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CLoading();

public:
	_bool				Get_Finish(void)	{ return m_bFinish; }
	CRITICAL_SECTION	Get_Crt(void)		{ return m_Crt; }
	LOADINGID			Get_LoadingID(void) { return m_eID; }
	const _tchar*		Get_String(void)	{ return m_szString; }
	void				Set_String(_tchar* pString) { lstrcpy(m_szString, pString); }

	CScene*	Get_Scene() { return m_pScene; }
public:
	static unsigned int CALLBACK Thread_Main(void* pArg);

public:
	HRESULT				Ready_Loading(LOADINGID eID);
	_uint				Loading_ForLogo(void);
	_uint				Loading_ForStage1(void);
	_uint				Loading_ForStage2(void);
	_uint				Loading_ForStage3(void);
	_uint				Loading_ForStage4(void);
	_uint				Loading_ForMini1(void);
	_uint				Loading_ForStage8(void);

	_uint				Loading_ForFinal1(void);
	_uint				Loading_ForBGTool(void);
	_int				Get_LoadingPercent() { return m_iLoadingTexImgNum; }
private:
	LPDIRECT3DDEVICE9			m_pGraphicDev;
	HANDLE						m_hThread;
	LOADINGID					m_eID;
	_bool						m_bFinish;
	_tchar						m_szString[128];

	CRITICAL_SECTION			m_Crt;

	CScene*						m_pScene = nullptr;

	_int						m_iLoadingTexImgNum = 0;
public:
	static CLoading*		Create(LPDIRECT3DDEVICE9 pGraphicDev, LOADINGID eID);

private:
	virtual void		Free(void);
};

