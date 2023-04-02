#pragma once
#include "Include.h"
#include "GameObject.h"

class CDynamiCamera : public Engine::CGameObject
{
public:
	explicit CDynamiCamera(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CDynamiCamera();

public:
	virtual HRESULT Ready_GameObject(void) override;
	virtual _int Update_GameObject(const _float& fTimeDelta) override;
	virtual void LateUpdate_GameObject(void) override;
	virtual void Render_GameObject(void) override;

private:
	HRESULT		Add_Component(void);
	void		Rotation_View();
	void		Key_Input(const _float& fTimeDelta);
	void		Move_Camera(const _float& fTimeDelta);

	float Linear(float A, float B, float Alpha)
	{
		return A * (1 - Alpha) + B * Alpha;
	}

	_float m_fTime;
	_matrix matRotX;
	_vec3 vEye, vAt, vUp;

	_float m_fToo, m_fTop;

public:
	static CDynamiCamera*		Create(LPDIRECT3DDEVICE9 pGraphicDev);

};
//기준점에서 {기준점 y + 0 * 거리, 기준점 z + 1 * 거리},{기준점 y + 삼각함수30 * 거리, 기준점 z + 삼각함수30 * 거리} * 거리
//바라보는 방향 항상 기준점
