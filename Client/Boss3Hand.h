#pragma once
#include "Cube.h"

class CBoss3Hand : public CCube
{
private:
	explicit CBoss3Hand(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CBoss3Hand();

public:
	void	Set_Attack(_bool TrueorFalse) { m_bAttack = TrueorFalse; }
	void Set_Shock(_bool TF) { m_bShock = TF; }
public:
	virtual HRESULT Ready_GameObject(_vec3& vPos, _int iIndex);
	virtual _int Update_GameObject(const _float& fTimeDelta) override;
	virtual _int Update_Too(const _float & fTimeDelta);
	virtual _int Update_Top(const _float & fTimeDelta);
	virtual void LateUpdate_GameObject(void) override;
	virtual void Render_GameObject(void) override;
	virtual void OnCollisionEnter(const class Collision* collision);
	virtual void OnCollisionStay(const class Collision* collision);
	virtual void OnCollisionExit(const class Collision* collision);
	virtual void SwapTrigger() override;

private:
	HRESULT Add_Component(void);

	void	FollowPlayer(const _float & fTimeDelta);
	void	BossAttack(const _float & fTimeDelta);
	void	IdleMove(const _float & fTimeDelta);

private:
	_int	m_iIndex;
	_float  m_fAngle;
	_float	m_fSpeed;

	_float  m_fCoolDown;
	_float  m_fAttackCoolDown;
	_float m_fShockCollDown;

	_bool	m_bShock = false;

	_int m_iBossHp=1;
	_float  m_fIdleCycle;
	_float  m_fIdleAngle;
	_bool	m_bAttack;

	_bool	m_bIdleMove;
	_bool	m_bIdleStop;

public:
	static CBoss3Hand*	Create(LPDIRECT3DDEVICE9 pGraphicDev, _vec3& vPos, _int iIndex);

protected:
	virtual void Free(void) override;
};

#define BOSS3_CHASE 1.5f
#define BOSS3_SPIN 0.5f