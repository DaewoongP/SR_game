#pragma once

#include "Include.h"
#include "Base.h"
#include "Export_Function.h"

class CImguiMgr;

BEGIN(Engine)

class CGraphicDev;
class CManagement;

END

class CMainApp : public CBase
{
private:	// 持失切, 社瑚切
	explicit CMainApp();
	~CMainApp();

public:
	HRESULT		Ready_MainApp(void);
	_int		Update_MainApp(const _float& fTimeDelta);
	void		LateUpdate_MainApp(void);
	void		Render_MainApp(void);

private:
	HRESULT			Ready_Proto();
	HRESULT			Ready_DefaultSetting(LPDIRECT3DDEVICE9* ppGraphicDev);

private:
	Engine::CGraphicDev*		m_pDeviceClass;
	Engine::CManagement*		m_pManagementClass;

	LPDIRECT3DDEVICE9			m_pGraphicDev;
	CImguiMgr* m_pCImguiMgr;

public:
	static CMainApp*		Create(void);

private:
	virtual void		Free(void);
};

