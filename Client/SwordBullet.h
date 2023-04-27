#pragma once
#include "Include.h"
#include "Bullet.h"

class CSwordBullet : public CBullet
{
private:
	explicit CSwordBullet(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CSwordBullet();

public:
	HRESULT Ready(_vec3& vPos, _vec3& vInitPos);
	virtual _int Update_GameObject(const _float& fTimeDelta) override;
	virtual void LateUpdate_GameObject(void) override;
	virtual void Render_GameObject(void) override;
	virtual void OnCollisionEnter(const class Collision* collision);

private:
	HRESULT Add_Component(void);

private:
	_bool	m_bShoot;
public:
	static CSwordBullet*	Create(LPDIRECT3DDEVICE9 pGraphicDev, _vec3& vPos, _vec3& vInitPos);

private:
	virtual void Free(void) override;
};

