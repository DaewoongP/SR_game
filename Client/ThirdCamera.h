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
	void	Swap_Camera();
	void	Set_ToodeeCam(_vec3 vPos);
	void	Set_TopdeeCam(_vec3 vPos);
	void	Update_Matrix();
private:
	_matrix		m_matRot;
	_matrix		m_matView;
	_matrix		m_matProj;
	_vec3		m_vViewInfo[VIEW_END];

public:
	static CThirdCamera*	Create(LPDIRECT3DDEVICE9 pGraphicDev);
private:
	virtual void Free(void) override;
};

