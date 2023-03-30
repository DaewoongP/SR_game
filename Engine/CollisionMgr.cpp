#include "stdafx.h"
#include "CollisionMgr.h"

IMPLEMENT_SINGLETON(CCollisionMgr)

CCollisionMgr::CCollisionMgr()
{
}


CCollisionMgr::~CCollisionMgr()
{
	Free();
}

void CCollisionMgr::Add_Collider(COLGROUP eID, CCollider * pCollider)
{
	if (COL_END <= eID || nullptr == pCollider)
		return;
	m_ColliderList[eID].push_back(pCollider);
}

void CCollisionMgr::Check_Collision()
{
	if (m_ColliderList[COL_OBJ].empty())
		return;
	for (auto& iter = m_ColliderList[COL_OBJ].begin();
	iter != m_ColliderList[COL_OBJ].end() - 1; ++iter)
	{
		for (auto& iter2 = iter + 1; iter2 != m_ColliderList[COL_OBJ].end(); ++iter2)
		{
			if (Collision_Box(*iter, *iter2))
			{
				(*iter)->OnTriggerStay(*iter2);
				(*iter2)->OnTriggerStay(*iter);
			}
		}
	}
}

_bool CCollisionMgr::Collision_Box(CCollider * pSrc, CCollider * pDest)
{
	_float fX, fY, fZ;
	if (Check_BoundingBox(pSrc, pDest, &fX, &fY, &fZ))
	{
		// 상하 (src기준)
		if (fX > fY)
		{
			if (pSrc->Get_BoundCenter().y < pDest->Get_BoundCenter().y)
			{
				// 상충돌
				return true;
			}
			else
			{
				// 하충돌
				return true;
			}
		}
		else // 좌우
		{
			if (pSrc->Get_BoundCenter().x < pDest->Get_BoundCenter().x)
			{
				// 우충돌
				return true;
			}
			else
			{
				// 좌충돌
				return true;
			}
		}
	}
	return false;
}

_bool CCollisionMgr::Check_BoundingBox(CCollider * pSrc, CCollider * pDest, _float * pX, _float * pY, _float * pZ)
{
	float	fX = fabs(pDest->Get_BoundCenter().x - pSrc->Get_BoundCenter().x);
	float	fY = fabs(pDest->Get_BoundCenter().y - pSrc->Get_BoundCenter().y);
	float	fZ = fabs(pDest->Get_BoundCenter().z - pSrc->Get_BoundCenter().z);

	float	fRadiusX = (pDest->Get_BoundSize().x + pSrc->Get_BoundSize().x) * 0.5f;
	float	fRadiusY = (pDest->Get_BoundSize().y + pSrc->Get_BoundSize().y) * 0.5f;
	float	fRadiusZ = (pDest->Get_BoundSize().z + pSrc->Get_BoundSize().z) * 0.5f;

	// ???? 2D???? ?浹???? ????? Z???? =??? ???.
	if ((fRadiusX > fX) && (fRadiusY > fY) && (fRadiusZ > fZ))
	{
		*pX = fRadiusX - fX;
		*pY = fRadiusY - fY;
		*pZ = fRadiusZ - fZ;
		return true;
	}

	return false;
}

void CCollisionMgr::Clear_Collision()
{
	for (size_t i = 0; i < COL_END; ++i)
	{
		for_each(m_ColliderList[i].begin(), m_ColliderList[i].end(), CDeleteObj());
		m_ColliderList[i].clear();
	}
}

void CCollisionMgr::Free(void)
{
	Clear_Collision();
}
