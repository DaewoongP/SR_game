#pragma once
#include"..\Engine\Include.h"
#include "..\Engine\GameObject.h"
class CUICamera :
    public CGameObject
{

private:
	explicit CUICamera(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CUICamera();

public:
	virtual HRESULT Ready_GameObject(void) override;
	virtual _int Update_GameObject(const _float& fTimeDelta) override;
	virtual void LateUpdate_GameObject(void) override;

private:
	void		Rotation_View();
	void		Key_Input(const _float& fTimeDelta);
	void		Move_Camera(const _float& fTimeDelta);


	_float m_fTime = 0.f;
	_matrix matRotX;
	_vec3 vEye, vAt, vUp;

	_float m_fToo, m_fTop;

public:
	static CUICamera* Create(LPDIRECT3DDEVICE9 pGraphicDev);
private:
	virtual void Free();
};

