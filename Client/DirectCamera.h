#pragma once
#include "Camera.h"
class CDirectCamera :
	public CCamera
{
protected:
	explicit CDirectCamera(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CDirectCamera();
public:
	
	virtual _int Update_GameObject(const _float& fTimeDelta) override;
protected:
	// x, y 포지션 변경  
	// angle 확대
	// 속도
	vector<_vec4> m_vecXYPosAngleSpeed;
	_int m_iIndex;
	_float m_fTimer;

protected:
	void Update_Direct(const _float& fTimeDelta);
	

protected:
	virtual void Free() override;
};

