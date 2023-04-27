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

	void	Set_PlayerPos(const _float& fTimeDelta);
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
	_float		m_fOffsetScale;
	_bool		m_RePosition;
public:
	void	SetOffset(_float vec) { m_fOffsetScale = vec; m_RePosition = false; }
	_float	GetOffset() { return m_fOffsetScale;}
	void	SetRePosition() { m_RePosition = true; }
	static CThirdCamera*	Create(LPDIRECT3DDEVICE9 pGraphicDev);

	void Start_Camera_Shake(_float fDuration, _int iPower, SHAKE_AXIS eShake_Axis = Engine::SHAKE_ALL, _bool bUseWeak = true)
	{
		m_pTransform->m_bUseWeak = bUseWeak;
		m_pTransform->Set_Shake(fDuration, iPower, eShake_Axis);
	}
private:
	virtual void Free(void) override;
};

