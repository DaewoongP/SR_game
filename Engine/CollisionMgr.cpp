#include "stdafx.h"
#include "CollisionMgr.h"

IMPLEMENT_SINGLETON(CCollisionMgr)

CCollisionMgr::CCollisionMgr()
	:m_fRangeOffset(1.f)
{
}


CCollisionMgr::~CCollisionMgr()
{
	Free();
}

void CCollisionMgr::Add_Collider(CCollider * pCollider)
{
	if (nullptr == pCollider)
		return;
	
	m_ColliderList[COL_OBJ].push_back(pCollider);
	pCollider->AddRef();
}

void CCollisionMgr::Check_Collision(COLGROUP eGroup1, COLGROUP eGroup2)
{
	if (m_ColliderList[eGroup1].empty())
		return;
	if (m_ColliderList[eGroup2].empty())
		return;
	if (eGroup1 == eGroup2 && m_ColliderList[eGroup1].size() < 2)
		return;
	for (auto& iter = m_ColliderList[eGroup1].begin();
	iter != m_ColliderList[eGroup1].end(); ++iter)
	{
		for (auto& iter2 = m_ColliderList[eGroup2].begin(); 
		iter2 != m_ColliderList[eGroup2].end(); ++iter2)
		{
			if ((*iter) == (*iter2))
				continue;
			/*if (false == Collision_Range((*iter), (*iter2)))
				continue;*/
			if (Collision_Box(*iter, *iter2))
			{
				Collision* pCollision = (*iter)->Find_ColState(*iter2);
				pCollision->Set_PreCol();
				pCollision->otherObj = (*iter2)->m_pGameObject;
				switch (pCollision->_CurState)
				{
				case Engine::COLSTATE_ENTER:
					(*iter)->OnCollisionEnter(pCollision);
					break;
				case Engine::COLSTATE_STAY:
					(*iter)->OnCollisionStay(pCollision);
					break;
				case Engine::COLSTATE_EXIT:
					(*iter)->OnCollisionExit(pCollision);
					break;
				case Engine::COLSTATE_NONE:
					break;
				}

				pCollision = (*iter2)->Find_ColState(*iter);
				pCollision->Set_PreCol();
				pCollision->otherObj = (*iter)->m_pGameObject;
				switch (pCollision->_CurState)
				{
				case Engine::COLSTATE_ENTER:
					(*iter2)->OnCollisionEnter(pCollision);
					break;
				case Engine::COLSTATE_STAY:
					(*iter2)->OnCollisionStay(pCollision);
					break;
				case Engine::COLSTATE_EXIT:
					(*iter2)->OnCollisionExit(pCollision);
					break;
				case Engine::COLSTATE_NONE:
					break;
				}
			}
		}
	}
}

_bool CCollisionMgr::Collision_Range(CCollider* pSrc, CCollider* pDest)
{
	_vec3 vSrcCenter, vSrcSize, vDstCenter, vDstSize;
	vSrcCenter = pSrc->Get_BoundCenter();
	vDstCenter = pDest->Get_BoundCenter();
	vSrcSize = pSrc->Get_BoundSize() * m_fRangeOffset;
	vDstSize = pDest->Get_BoundSize() * m_fRangeOffset;

	_float fSrcLong = 9999.f;
	_float fDstLong = 9999.f;
	if (fSrcLong > vSrcSize.x)
		fSrcLong = vSrcSize.x;
	if (fSrcLong > vSrcSize.y)
		fSrcLong = vSrcSize.y;
	if (fSrcLong > vSrcSize.z)
		fSrcLong = vSrcSize.z;

	if (fDstLong > vDstSize.x)
		fDstLong = vDstSize.x;
	if (fDstLong > vDstSize.y)
		fDstLong = vDstSize.y;
	if (fDstLong > vDstSize.z)
		fDstLong = vDstSize.z;

	_float fDistance;
	fDistance = D3DXVec3Length(&(vDstCenter - vSrcCenter));

	if (fDistance <= fSrcLong + fDstLong)
		return true;

	return false;
}

_bool CCollisionMgr::Collision_Box(CCollider * pSrc, CCollider * pDest)
{
	_float fX, fY, fZ;
	_bool bChk = false;
	if (Check_BoundingBox(pSrc, pDest, &fX, &fY, &fZ))
	{
		// ?í•˜ (srcê¸°ì?)
		if (fX > fY)
		{
			if (pSrc->Get_BoundCenter().y < pDest->Get_BoundCenter().y)
			{
				// ?ì¶©??
				pSrc->Insert_Collider(pDest, COL_DIR::DIR_UP);
				pDest->Insert_Collider(pSrc, COL_DIR::DIR_UP);
				return true;
			}
			else
			{
				// ?˜ì¶©??
				pSrc->Insert_Collider(pDest, COL_DIR::DIR_DOWN);
				pDest->Insert_Collider(pSrc, COL_DIR::DIR_DOWN);
				return true;
			}
		}
		else // ì¢Œìš°
		{
			if (pSrc->Get_BoundCenter().x < pDest->Get_BoundCenter().x)
			{
				// ?°ì¶©??
				pSrc->Insert_Collider(pDest, COL_DIR::DIR_RIGHT);
				pDest->Insert_Collider(pSrc, COL_DIR::DIR_RIGHT);
				return true;
			}
			else
			{
				// ì¢Œì¶©??
				pSrc->Insert_Collider(pDest, COL_DIR::DIR_LEFT);
				pDest->Insert_Collider(pSrc, COL_DIR::DIR_LEFT);
				return true;
			}
		}
	}
	else
	{
		if (pSrc->Delete_OtherCollider(pDest) | pDest->Delete_OtherCollider(pSrc))
			return true;
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

	if ((fRadiusX >= fX) && (fRadiusY >= fY) && (fRadiusZ >= fZ))
	{
		*pX = fRadiusX - fX;
		*pY = fRadiusY - fY;
		*pZ = fRadiusZ - fZ;
		return true;
	}

	return false;
}

void CCollisionMgr::Delete_Collider(CGameObject* pGameObject)
{
	for (size_t i = 0; i < COL_END; ++i)
	{
		for (auto iter = m_ColliderList[i].begin(); iter != m_ColliderList[i].end();)
		{
			if ((*iter)->m_pGameObject == pGameObject)
			{
				Safe_Release(*iter);
				iter = m_ColliderList[i].erase(iter);
			}
			else
				++iter;
		}
	}
}

void CCollisionMgr::Set_Collider(COLGROUP eGroup, CCollider * pCollider)
{
	for (size_t i = 0; i < COL_END; ++i)
	{
		if (pCollider->Get_Group() == eGroup)
			return;
		for (auto iter = m_ColliderList[i].begin(); iter != m_ColliderList[i].end();)
		{
			if ((*iter) == pCollider)
			{
				iter = m_ColliderList[i].erase(iter);
				m_ColliderList[eGroup].push_back(pCollider);
				return;
			}
			else
				++iter;
		}
	}
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
