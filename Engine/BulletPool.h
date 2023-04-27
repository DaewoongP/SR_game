#pragma once
#include "Include.h"
#include "..\Client\Bullet.h"
BEGIN(Engine)

class CBulletPool
{
	DECLARE_SINGLETON(CBulletPool)
private:
	explicit CBulletPool() {}
	~CBulletPool();

public:
	CBullet*		Reuse_Bullet(LPDIRECT3DDEVICE9& pGraphicDev, _vec3& vPos, BULLETTYPE eType, _vec3& vDir = _vec3(0, 1, 0));
	void			Release_Bullet(CBullet* pBullet);

	void			Reserve_Bullet(LPDIRECT3DDEVICE9& pGraphicDev, BULLETTYPE eType);
private:
	_bool			IsEmpty(BULLETTYPE eType);
private:
	list<CBullet*>	m_PoolList[BULLET_END];
};

END