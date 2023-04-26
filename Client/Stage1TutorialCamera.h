#pragma once
#include "DirectCamera.h"
class CStage1TutorialCamera :
	public CDirectCamera
{
private:
	explicit CStage1TutorialCamera(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CStage1TutorialCamera();
	HRESULT Ready_Camera();
public:
	static CStage1TutorialCamera*		Create(LPDIRECT3DDEVICE9 pGraphicDev);
private:
	virtual void Free() override;
};
