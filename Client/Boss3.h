#pragma once
#include "Cube.h"

class CBoss3 : public CCube
{
private:
	explicit CBoss3(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CBoss3();

public:
	virtual HRESULT Ready_GameObject(_vec3& vPos) override;
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
	void	BossLook(const _float& fTimeDelta);
	void	BossAttack(const _float & fTimeDelta);

private:
	_bool   m_bCreateHand;
	_float  m_fAngle;
	_float	m_fSpeed;
	_float  m_fCoolDown;
	_vec3  m_vLookDot;
	_vec3 m_vDirection;
	_vec3 m_vPlayerInfo;

public:
	static CBoss3*	Create(LPDIRECT3DDEVICE9 pGraphicDev, _vec3& vPos);

protected:
	virtual void Free(void) override;

};

