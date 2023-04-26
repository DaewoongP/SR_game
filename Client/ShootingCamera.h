#pragma once
#include "Camera.h"
class CShootingCamera : public CCamera
{
private:
	explicit CShootingCamera(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CShootingCamera();

public:
	HRESULT		 Ready_Camera();
	virtual _int Update_GameObject(const _float& fTimeDelta) override;
	virtual void Render_GameObject() override;

public:
	static CShootingCamera*	Create(LPDIRECT3DDEVICE9 pGraphicDev);
private:
	virtual void Free();
};

