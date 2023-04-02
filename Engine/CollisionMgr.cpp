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
	
	m_ColliderList.push_back(pCollider);
	pCollider->AddRef();
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
			// 충돌범위 판정인데.. 거의 뭐 하나마나 ㅜㅜ..
			/*if (false == Collision_Range((*iter), (*iter2)))
				continue;*/
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

_bool CCollisionMgr::Collision_Range(CCollider* pSrc, CCollider* pDest)
{
	_vec3 vSrcCenter, vSrcSize, vDstCenter, vDstSize;
	vSrcCenter = pSrc->Get_BoundCenter();
	vDstCenter = pDest->Get_BoundCenter();
	vSrcSize = pSrc->Get_BoundSize() * m_fRangeOffset;
	vDstSize = pDest->Get_BoundSize() * m_fRangeOffset;

	// 바운딩박스의 너비, 높이, 깊이중 가장 큰값 가져옴
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
	// 충돌 판정 원 내부
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
	for (auto iter = m_ColliderList.begin(); iter != m_ColliderList.end();)
	{
		if ((*iter)->m_pGameObject == pGameObject)
		{
			Safe_Release(*iter);
			iter = m_ColliderList.erase(iter);
		}
		else
			++iter;
	}
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
