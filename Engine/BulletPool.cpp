#include "stdafx.h"
#include "BulletPool.h"

#include "Export_Function.h"
#include "..\Client\DefaultBullet.h"
#include "..\Client\SwordBullet.h"
#include "..\Client\FireBullet.h"
#include"..\Client\Laser.h"
IMPLEMENT_SINGLETON(CBulletPool)
CBulletPool::~CBulletPool()
{
	for (size_t i = 0; i < BULLET_END; ++i)
		for_each(m_PoolList[i].begin(), m_PoolList[i].end(), CDeleteObj());
}
CBullet* CBulletPool::Reuse_Bullet(LPDIRECT3DDEVICE9& pGraphicDev, _vec3& vPos, BULLETTYPE eType, _vec3& vDir)
{
	CBullet* pBullet = nullptr;
	if (IsEmpty(eType))
	{
		switch (eType)
		{
		case DEFAULT:
			pBullet = CDefaultBullet::Create(pGraphicDev, vPos, vDir);
			break;
		case SWORD:
			pBullet = CSwordBullet::Create(pGraphicDev, vPos);
			break;
		case FIRE:
			pBullet = CFireBullet::Create(pGraphicDev, vPos);
		case LASER:
			pBullet = CLaser::Create(pGraphicDev, vPos);
	
		}
		return pBullet;
	}
	else
	{
		pBullet = m_PoolList[eType].back();
		m_PoolList[eType].pop_back();
		pBullet->Ready_Pool(vPos, vDir);
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
	else if (dynamic_cast<CFireBullet*>(pBullet))
		m_PoolList[LASER].push_back(pBullet);
}

void CBulletPool::Reserve_Bullet(LPDIRECT3DDEVICE9& pGraphicDev, BULLETTYPE eType)
{
	CBullet* pBullet = nullptr;
	switch (eType)
	{
	case DEFAULT:
		pBullet = CDefaultBullet::Create(pGraphicDev, _vec3(0,0,0), _vec3(0, 0, 0));
		break;
	case SWORD:
		pBullet = CSwordBullet::Create(pGraphicDev, _vec3(0, 0, 0));
		break;
	case FIRE:
		pBullet = CFireBullet::Create(pGraphicDev, _vec3(0, 0, 0));
		break;
	case LASER:
		pBullet = CLaser::Create(pGraphicDev, _vec3(0, 0, 0));
		break;
	}
	Release_Bullet(pBullet);
}

_bool CBulletPool::IsEmpty(BULLETTYPE eType)
{
	return m_PoolList[eType].empty();
}
