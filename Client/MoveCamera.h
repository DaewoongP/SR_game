#pragma once
#include "Include.h"
#include "GameObject.h"

class CMoveCamera : public CGameObject
{
	enum MyView
	{
		EYE,
		AT,
		UP,
		VIEW_END
	};
private:
	explicit CMoveCamera(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CMoveCamera();
public:
	virtual HRESULT		Ready_GameObject(void)override;
	virtual _int		Update_GameObject(const _float& fTimeDelta) override;

private:
	void	Update_Matrix();
	void	Move_Cam(const _float& fTimeDelta);
private:
	_matrix		m_matRot;
	_matrix		m_matView;
	_matrix		m_matProj;
	_vec3		m_vViewInfo[VIEW_END];

	_float		m_fSpeed;

public:
	static CMoveCamera*	Create(LPDIRECT3DDEVICE9 pGraphicDev);
private:
	virtual void Free(void) override;
};

