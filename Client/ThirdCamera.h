#pragma once
#include "Include.h"
#include "GameObject.h"

class CThirdCamera : public CGameObject
{
	enum MyView
	{
		EYE,
		AT,
		UP,
		VIEW_END
	};
private:
	explicit CThirdCamera(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CThirdCamera();
public:
	virtual HRESULT		Ready_GameObject(void)override;
	virtual _int		Update_GameObject(const _float& fTimeDelta) override;	

private:
	void	Swap_Camera(const _float& fTimeDelta);
	void	Set_ToodeeCam(const _float& fTimeDelta);
	void	Set_TopdeeCam(const _float& fTimeDelta);
	void	Update_Matrix();

	void	Set_PlayerPos();
private:
	_vec3		m_vPlayerPos;
	_matrix		m_matRot;
	_matrix		m_matView;
	_matrix		m_matProj;
	_vec3		m_vViewInfo[VIEW_END];
	_vec3		m_vTooView[VIEW_END];
	_vec3		m_vTopView[VIEW_END];

	_float		m_fs;
	_float		m_fCamChangeSpeed;
	
public:
	static CThirdCamera*	Create(LPDIRECT3DDEVICE9 pGraphicDev);
private:
	virtual void Free(void) override;
};

