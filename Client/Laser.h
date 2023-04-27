#pragma once
#include "Include.h"
#include "Bullet.h"

class CLaser : public CBullet
{
private:
	explicit CLaser(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CLaser();

public:
	virtual HRESULT Ready_Bullet(_vec3& vPos, _vec3& vDir = _vec3(0.f, 1.f, 0.f)) override;
	virtual void	Ready_Pool(_vec3& vPos, _vec3& vDir = _vec3(0.f, 1.f, 0.f)) override;
	virtual _int Update_GameObject(const _float& fTimeDelta) override;
	virtual void LateUpdate_GameObject(void) override;
	virtual void Render_GameObject(void) override;
	virtual void OnCollisionEnter(const class Collision* collision);

private:
	HRESULT Add_Component(void);

private:
	_bool	m_bShoot;
	_vec3 m_vPos;
	_vec3 m_vTarget;

public:
	static CLaser* Create(LPDIRECT3DDEVICE9 pGraphicDev, _vec3& vPos);

private:
	virtual void Free(void) override;
};

