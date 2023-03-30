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
	_vec3 vMin, vMax;
	// 이거 포인트 좌표를 8개를 넣어줄지.. 므ㅝ 어케할지 정해야할듯;;
	pDest->Get_Point(&vMin, &vMax);
	if (pSrc->Intersect(vMin) || pSrc->Intersect(vMax))
		return true;
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
