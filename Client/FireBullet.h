#pragma once
#include "Include.h"
#include "Bullet.h"

class CFireBullet : public CBullet
{
private:
	explicit CFireBullet(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CFireBullet();

public:
	virtual HRESULT Ready_Bullet(_vec3& vPos, _vec3& vDir = _vec3(0, 1, 0)) override;
	virtual _int Update_GameObject(const _float& fTimeDelta) override;
	virtual void LateUpdate_GameObject(void) override;
	virtual void Render_GameObject(void) override;
	virtual void OnCollisionEnter(const class Collision* collision);

private:
	HRESULT Add_Component(void);

public:
	static CFireBullet*	Create(LPDIRECT3DDEVICE9 pGraphicDev, _vec3& vPos);

private:
	virtual void Free(void) override;
};

