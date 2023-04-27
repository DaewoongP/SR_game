#pragma once
#include "Bullet.h"

class CDefaultBullet : public CBullet
{
private:
	explicit CDefaultBullet(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CDefaultBullet();

public:
	virtual HRESULT Ready_Bullet(_vec3& vPos, _vec3& vDir) override;
	virtual _int Update_GameObject(const _float& fTimeDelta) override;
	virtual void LateUpdate_GameObject(void) override;
	virtual void Render_GameObject(void) override;
private:
	HRESULT Add_Component(void);

public:
	static CDefaultBullet*	Create(LPDIRECT3DDEVICE9 pGraphicDev, _vec3& vPos, _vec3& vDir);
private:
	virtual void Free(void) override;
};

