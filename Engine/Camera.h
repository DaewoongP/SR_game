#pragma once

#include "Component.h"

BEGIN(Engine)

class CCameraMgr;
class CCamera : public CComponent
{
	friend CCameraMgr;

private:
	explicit CCamera(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CCamera(const CCamera& rhs);
	virtual ~CCamera();

public:
	HRESULT			Ready_Camera(VIEWPARAMS& tViewParam, PROJPARAMS& tProjParam);
	virtual	_int	Update_Component(const _float& fTimeDelta);
	virtual	void	LateUpdate_Component() {}

	void			Set_CameraName(const _tchar* pName) { m_pName = pName; }

private:
	void			On_Camera() { m_bSwitch = true; }
	void			Off_Camera() { m_bSwitch = false; }

private:
	

	_matrix			m_matView;
	_matrix			m_matProj;

	const _tchar*	m_pName;
	_bool			m_bSwitch;

public:
	static CCamera* Create(LPDIRECT3DDEVICE9 pGraphicDev,
		VIEWPARAMS& tViewParam = VIEWPARAMS(
	{ 0.f, 0.f, 0.f },
	{ 0.f, 0.f, 0.f },
	{ 0.f, 1.f, 0.f }),

		PROJPARAMS& tProjParam = PROJPARAMS(
			D3DXToRadian(60.f),
			(_float)WINCX / WINCY,
			1.f, 1000.f));
public:
	VIEWPARAMS		m_tViewParams;
	PROJPARAMS		m_tProjParams;

	virtual CComponent * Clone(void) override;

private:
	virtual void Free() override;
};

END

#if 0
#pragma once

#include "Component.h"

BEGIN(Engine)

class CCamera : public CComponent
{
private:
	explicit CCamera(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CCamera(const CCamera& rhs);
	virtual ~CCamera();

public:
	HRESULT			Ready_Camera(VIEWPARAMS& tViewParam, PROJPARAMS& tProjParam, HWND& hWnd);
	virtual	_int	Update_Component(const _float& fTimeDelta);
	virtual	void	LateUpdate_Component() {}

private:
	void		Key_Input(const _float & fTimeDelta);
	void		Mouse_Move();
	void		Fix_Mouse();

	//1ÀÎÄª
	void		First_Person_View();

	//3ÀÎÄª
	void		Third_Person_View();

	//Å¸°Ù ºä
	void		Ready_Target(_vec3* Target);
	void		Target_View(_vec3* Target);

	//Å×½ºÆ® ¿ë ÇÔ¼ö
	void		Test_Change_View();
private:
	VIEWPARAMS	m_tViewParams;
	PROJPARAMS	m_tProjParams;

	_matrix		m_matView;
	_matrix		m_matProj;

	_bool		m_bFix;
	_bool		m_bClick;

	HWND		m_hWnd;

	_vec3*		m_pvTarget;

	//Å×½ºÆ® ¿ë º¯¼ö
	_int		m_iTest;

public:
	static CCamera* Create(LPDIRECT3DDEVICE9 pGraphicDev,
		VIEWPARAMS& tViewParam = VIEWPARAMS(
	{ 100.f, 50.f, -15.f },
	{ 0.f, 0.f, 0.f },
	{ 0.f, 1.f, 0.f }),

		PROJPARAMS& tProjParam = PROJPARAMS(
			D3DXToRadian(60.f),
			(_float)WINCX / WINCY,
			1.f, 1000.f),

		HWND hWnd = g_hWnd);

	virtual CComponent * Clone(void) override;

private:
	virtual void Free() override;
};

END
#endif // 0
