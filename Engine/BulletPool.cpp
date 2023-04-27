#include "stdafx.h"
#include "BulletPool.h"

#include "Export_Function.h"
#include "..\Client\DefaultBullet.h"
#include "..\Client\SwordBullet.h"
#include "..\Client\FireBullet.h"
IMPLEMENT_SINGLETON(CBulletPool)

CBullet* CBulletPool::Reuse_Bullet(LPDIRECT3DDEVICE9 pGraphicDev, _vec3& vPos, BULLETTYPE eType)
{
	CBullet* pBullet = nullptr;
	if (IsEmpty(eType))
	{
		switch (eType)
		{
		case DEFAULT:
			pBullet = CDefaultBullet::Create(pGraphicDev, vPos);
			break;
		case SWORD:
			pBullet = CSwordBullet::Create(pGraphicDev, vPos);
			break;
		case FIRE:
			pBullet = CFireBullet::Create(pGraphicDev, vPos);
			break;
		}
		return pBullet;
	}
	else
	{
		pBullet = m_PoolList[eType].back();
		m_PoolList[eType].pop_back();
		pBullet->Ready_Bullet(vPos);
		return pBullet;
	}

	return nullptr;
}

void CBulletPool::Release_Bullet(CBullet* pBullet)
{
	if (nullptr == pBullet)
		return;
	if (dynamic_cast<CDefaultBullet*>(pBullet))
		m_PoolList[DEFAULT].push_back(pBullet);
	else if (dynamic_cast<CSwordBullet*>(pBullet))
		m_PoolList[SWORD].push_back(pBullet);
	else if (dynamic_cast<CFireBullet*>(pBullet))
		m_PoolList[FIRE].push_back(pBullet);
}

_bool CBulletPool::IsEmpty(BULLETTYPE eType)
{
	return m_PoolList[eType].empty();
}
