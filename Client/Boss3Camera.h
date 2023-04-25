#pragma once
#include "DirectCamera.h"
class CBoss3Camera :
	public CDirectCamera
{
private:
	explicit CBoss3Camera(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CBoss3Camera();
	HRESULT Ready_Camera();
public:
	static CBoss3Camera*		Create(LPDIRECT3DDEVICE9 pGraphicDev);
private:
	virtual void Free() override;
};

