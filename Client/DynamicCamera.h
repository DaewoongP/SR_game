#pragma once

#include "GameObject.h"

class CDynamicCamera : public CGameObject
{
private:
	explicit CDynamicCamera(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CDynamicCamera();

public:
	HRESULT			Ready_GameObject(void);

	virtual			_int Update_GameObject(const _float& fTimeDelta);
	virtual			void Render_GameObject() override;

private:
	void		Key_Input(const _float& fTimeDelta);
	void		Mouse_Move(void);
	void		Fix_Mouse(void);

private:
	_vec3		m_vEye;
	_vec3		m_vAt;
	_vec3		m_vUp;
	_float		m_fFov;
	_float		m_fAspect;
	_float		m_fNear;
	_float		m_fFar;
	_matrix		m_matView;
	_matrix		m_matProj;
	_bool		m_bFix;
	_bool		m_bClick;
	_float		m_fSpeed;

public:
	static CDynamicCamera*		Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	virtual void	Free(void);
};

