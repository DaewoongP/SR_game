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

void CCollisionMgr::Add_Collider(CCollider * pCollider)
{
	if (nullptr == pCollider)
		return;
	
	m_ColliderList.push_back(pCollider);
}

void CCollisionMgr::Check_Collision()
{
	if (m_ColliderList.empty())
		return;
	for (auto& iter = m_ColliderList.begin();
	iter != m_ColliderList.end() - 1; ++iter)
	{
		for (auto& iter2 = iter + 1; iter2 != m_ColliderList.end(); ++iter2)
		{
			// 콜라이더의 그룹 값을 통해 충돌처리 판단
			if (COL_ENV == (*iter)->Get_Group() && COL_ENV == (*iter2)->Get_Group())
				continue;

			if (Collision_Box(*iter, *iter2))
			{
				// 충돌상태 작성된거 대로 호출
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

_bool CCollisionMgr::Collision_Box(CCollider * pSrc, CCollider * pDest)
{
	_float fX, fY, fZ;
	_bool bChk = false;
	if (Check_BoundingBox(pSrc, pDest, &fX, &fY, &fZ))
	{
		// 상하 (src기준)
		if (fX > fY)
		{
			if (pSrc->Get_BoundCenter().y < pDest->Get_BoundCenter().y)
			{
				// 상충돌
				pSrc->Insert_Collider(pDest, COL_DIR::DIR_UP);
				pDest->Insert_Collider(pSrc, COL_DIR::DIR_UP);
				return true;
			}
			else
			{
				// 하충돌
				pSrc->Insert_Collider(pDest, COL_DIR::DIR_DOWN);
				pDest->Insert_Collider(pSrc, COL_DIR::DIR_DOWN);
				return true;
			}
		}
		else // 좌우
		{
			if (pSrc->Get_BoundCenter().x < pDest->Get_BoundCenter().x)
			{
				// 우충돌
				pSrc->Insert_Collider(pDest, COL_DIR::DIR_RIGHT);
				pDest->Insert_Collider(pSrc, COL_DIR::DIR_RIGHT);
				return true;
			}
			else
			{
				// 좌충돌
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
	

	// 현재 충돌이 일어나지 않았는데, 이전프레임에 충돌이 있었다면 Exit 호출
	// 둘중 하나라도 true 일경우 충돌 나가기 처리 해야함
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

void CCollisionMgr::Clear_Collision()
{
	for_each(m_ColliderList.begin(), m_ColliderList.end(), CDeleteObj());
	m_ColliderList.clear();
}

void CCollisionMgr::Free(void)
{
	Clear_Collision();
}
