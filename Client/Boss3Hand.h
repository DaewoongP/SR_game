#pragma once
#include "Cube.h"

BEGIN(Engine)
class CCircularParticle;
END
class CBoss3Hand : public CCube
{
private:
	explicit CBoss3Hand(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CBoss3Hand();

public:
	void	Set_Attack(_bool TrueorFalse) { m_bAttack = TrueorFalse; }
	void	Set_Shock(_bool TF) { m_bShock = TF; }
	void	Set_Lerp() { m_bLerpMove = true; }
	void	Set_Damaged() { m_bDamaged = true; }

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

private:
	HRESULT Add_Component(void);

	void	Lerp_Moving(const _float& fTimeDelta);
	void	FollowPlayer(const _float & fTimeDelta);
	void	BossAttack(const _float & fTimeDelta);
	void	IdleMove(const _float & fTimeDelta);
	void	DamagedBoss3(const _float& fTimeDelta);

private:
	_int	m_iIndex;
	_float  m_fTimer;	// �÷��̾� ��ȯ �� x�� ȸ���� ����
	_float	m_fSpeed;

	_float  m_fCoolDown;
	_float  m_fAttackCoolDown;
	_float m_fShockCollDown;

	_bool	m_bLerpMove;		// ü�� ���� �� ���ڸ��� ���ƿ��� ���θ� �Ǵ���
	_bool	m_bShock = false;
	_bool   m_bSpin = true;

	_float  m_fIdleCycle;
	_float  m_fIdleAngle;
	_float  m_fLerpDist;		// ü�� ���� �� ���ڸ��� ���ƿ� �� ���� ��
	_bool	m_bAttack;

	_bool	m_bIdleMove;
	_bool	m_bIdleStop;

	_bool	m_bDamaged;
	_float	m_fDamagedTime;

	Engine::CShadow* m_pShadowCom;
	Engine::CCircularParticle*	m_pLandingParticle;

	Engine::CCircularParticle*	m_pSparkParticle;
	Engine::CCircularParticle*	m_pElecParticle;
public:
	static CBoss3Hand*	Create(LPDIRECT3DDEVICE9 pGraphicDev, _vec3& vPos, _int iIndex);

protected:
	virtual void Free(void) override;
};