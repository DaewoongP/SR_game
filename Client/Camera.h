#pragma once
#include "GameObject.h"
class CCamera : public CGameObject
{
protected:
	enum CamView { EYE, AT, UP, CAM_END };
	enum Proj { FOV, ASPECT, NEAR_, FAR_, PROJ_END };
protected:
	explicit CCamera(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CCamera();
public:
	virtual _int Update_GameObject(const _float& fTimeDelta) override;
	virtual void Render_GameObject() override;
protected:
	void	Update_Matrix();
protected:
	_vec3		m_vCam[CAM_END];
	_float		m_fProj[PROJ_END];
	_matrix		m_matView;
	_matrix		m_matProj;
	_float		m_fSpeed;
protected:
	virtual void Free() override;
};

