#pragma once

#include "Include.h"
#include "Base.h"
#include "Export_Function.h"

BEGIN(Engine)

class CGraphicDev;
class CManagement;


END

class CMainApp : public CBase
{
private:	// 생성자, 소멸자
	explicit CMainApp();
	~CMainApp();

public:
	HRESULT		Ready_MainApp(void);
	_int		Update_MainApp(const _float& fTimeDelta);
	void		LateUpdate_MainApp(void);
	void		Render_MainApp(void);

private:
	HRESULT			Ready_DefaultSetting(LPDIRECT3DDEVICE9* ppGraphicDev);
	HRESULT			Set_Scene(LPDIRECT3DDEVICE9 pGraphicDev, CManagement** ppManagement);

// 플레이어의 값 변경 함수
private:
	void			Change_Player_Scale(float fX, float fY, float fZ);
	void			Change_Player_Pos(float fX, float fY, float fZ);
	void			Change_Player_Rotation(float fX, float fY, float fZ);

// 플레이어의 값 변경 변수


private:
	Engine::CGraphicDev*		m_pDeviceClass;
	Engine::CManagement*		m_pManagementClass;

	LPDIRECT3DDEVICE9			m_pGraphicDev;

public:
	static CMainApp*		Create(void);

private:
	virtual void		Free(void);
};

