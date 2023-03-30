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

private:
	VIEWPARAMS	m_tViewParams;
	PROJPARAMS	m_tProjParams;

	_matrix		m_matView;
	_matrix		m_matProj;

	_bool		m_bFix;
	_bool		m_bClick;

	HWND		m_hWnd;

public:
	static CCamera* Create(LPDIRECT3DDEVICE9 pGraphicDev, 
		VIEWPARAMS& tViewParam = VIEWPARAMS(
	{100.f, 50.f, -15.f},
	{0.f, 0.f, 0.f},
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