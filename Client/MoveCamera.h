#pragma once
#include "Include.h"
#include "Camera.h"

class CMoveCamera : public CCamera
{
private:
	explicit CMoveCamera(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CMoveCamera();
public:
	virtual HRESULT		Ready_GameObject(void)override;
	virtual _int		Update_GameObject(const _float& fTimeDelta) override;
	virtual void		Render_GameObject() override;
private:
	void	Move_Cam(const _float& fTimeDelta);
private:
	_matrix		m_matRot;

public:
	static CMoveCamera*	Create(LPDIRECT3DDEVICE9 pGraphicDev);
private:
	virtual void Free(void) override;
};

