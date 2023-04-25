#pragma once
#include "DirectCamera.h"
class CBoss2Camera :
	public CDirectCamera
{
private:
	explicit CBoss2Camera(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CBoss2Camera();
	HRESULT Ready_Camera();
public:
	static CBoss2Camera*		Create(LPDIRECT3DDEVICE9 pGraphicDev);
private:
	virtual void Free() override;
};

