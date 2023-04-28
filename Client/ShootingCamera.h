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
	void Start_Camera_Shake(_float fDuration, _int iPower, SHAKE_AXIS eShake_Axis = Engine::SHAKE_ALL, _bool bUseWeak = true)
	{
		m_pTransform->m_bUseWeak = bUseWeak;
		m_pTransform->Set_Shake(fDuration, iPower, eShake_Axis);
	}
private:
	virtual void Free();
};

