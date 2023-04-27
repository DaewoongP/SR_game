#pragma once
#include "Include.h"
#include "..\Client\Bullet.h"
BEGIN(Engine)

class CBulletPool
{
	DECLARE_SINGLETON(CBulletPool)
private:
	explicit CBulletPool() {}
	virtual ~CBulletPool() {}

public:
	CBullet*		Reuse_Bullet(LPDIRECT3DDEVICE9 pGraphicDev, _vec3& vPos, BULLETTYPE eType);
	void			Release_Bullet(CBullet* pBullet);
private:
	_bool			IsEmpty(BULLETTYPE eType);
private:
	list<CBullet*>	m_PoolList[BULLET_END];
};

END