#include "stdafx.h"
#include "CollisionMgr.h"
#include "Collision.h"

IMPLEMENT_SINGLETON(CCollisionMgr)

CCollisionMgr::CCollisionMgr()
	:m_fRangeOffset(1.5f)
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
}

vector<CCollider*> CCollisionMgr::Collision_CheckRange(CCollider* col, vector<CCollider*> vecRange)
{
	// ������ ���� ����
	vector<CCollider*>	vecOut;
	// ������� ����������	
	_vec3 vThis = col->Get_BoundCenter();
	_vec3 vSize = col->Get_BoundSize();
	// fRange�� �ݶ��̴� �� ���� ��κ� ������
	_float fRange = 0.f;
	if (fRange < vSize.x)
		fRange = vSize.x;
	if (fRange < vSize.y)
		fRange = vSize.y;
	if (fRange < vSize.z)
		fRange = vSize.z;
	// �����¿���
	fRange *= m_fRangeOffset;
	_vec3 vOther = { 0.f, 0.f, 9999.f };
	for (auto& iter : vecRange)
	{
		_vec3 vOther = iter->Get_BoundCenter();
		// �ٸ���ü�� �Ÿ��� �����º��� ������ ���ο� ���Ϳ� Ǫ��
		if (fRange > fabs(D3DXVec3Length(&_vec3(vThis - vOther))))
		{
			vecOut.push_back(iter);
		}
	}

	return vecOut;
}

void CCollisionMgr::Check_Collision(COLGROUP eGroup1, COLGROUP eGroup2)
{
	vector<CCollider*>	rangeList;
	if (m_ColliderList[eGroup1].empty())
		return;
	if (m_ColliderList[eGroup2].empty())
		return;
	for (auto& iter : m_ColliderList[eGroup1])
	{
		rangeList = Collision_CheckRange(iter, m_ColliderList[eGroup2]);
		for(auto& iter2 : rangeList)
		{
			if (iter == iter2)
				continue;
			if (Collision_Box(iter, iter2))
			{
				Collision* pCollision = nullptr;
				iter->Find_ColList(iter2, &pCollision);
				if (pCollision == nullptr)
					continue;
				pCollision->Set_PreCol();
				pCollision->otherObj = iter2->m_pGameObject;
				switch (pCollision->_CurState)
				{
				case Engine::COLSTATE_ENTER:
					iter->OnCollisionEnter(pCollision);
					break;
				case Engine::COLSTATE_STAY:
					iter->OnCollisionStay(pCollision);
					break;
				case Engine::COLSTATE_EXIT:
					iter->OnCollisionExit(pCollision);
					break;
				case Engine::COLSTATE_NONE:
					break;
				}
				pCollision = nullptr;
				iter2->Find_ColList(iter, &pCollision);
				if (pCollision == nullptr)
					continue;
				pCollision->Set_PreCol();
				pCollision->otherObj = iter->m_pGameObject;
				switch (pCollision->_CurState)
				{
				case Engine::COLSTATE_ENTER:
					iter2->OnCollisionEnter(pCollision);
					break;
				case Engine::COLSTATE_STAY:
					iter2->OnCollisionStay(pCollision);
					break;
				case Engine::COLSTATE_EXIT:
					iter2->OnCollisionExit(pCollision);
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
	if (Check_BoundingBox(pSrc, pDest, &fX, &fY, &fZ))
	{
  		if (fX >= fY)
		{
			if (fZ > fY) // Y���� ����������
			{
				// src ���浹
				if (pSrc->Get_BoundCenter().y < pDest->Get_BoundCenter().y)
				{
					pSrc->Insert_Collider(pDest, COL_DIR::DIR_UP);
					pDest->Insert_Collider(pSrc, COL_DIR::DIR_DOWN);
					return true;
				}
				// src ���浹
				else
				{
					pSrc->Insert_Collider(pDest, COL_DIR::DIR_DOWN);
					pDest->Insert_Collider(pSrc, COL_DIR::DIR_UP);
					return true;
				}
			}
			else // Z���� ���� ������
			{
				// src �ĸ��浹
				if (pSrc->Get_BoundCenter().z < pDest->Get_BoundCenter().z)
				{
					pSrc->Insert_Collider(pDest, COL_DIR::DIR_BACK);
					pDest->Insert_Collider(pSrc, COL_DIR::DIR_FRONT);
					return true;
				}
				// src �����浹
				else
				{
					pSrc->Insert_Collider(pDest, COL_DIR::DIR_FRONT);
					pDest->Insert_Collider(pSrc, COL_DIR::DIR_BACK);
					return true;
				}
			}
		}
		else if (fY > fX)
		{
			if (fZ > fX) // X���� ����������
			{
				// src ���浹
				if (pSrc->Get_BoundCenter().x < pDest->Get_BoundCenter().x)
				{
					pSrc->Insert_Collider(pDest, COL_DIR::DIR_RIGHT);
					pDest->Insert_Collider(pSrc, COL_DIR::DIR_LEFT);
					return true;
				}
				// src ���浹
				else
				{
					pSrc->Insert_Collider(pDest, COL_DIR::DIR_LEFT);
					pDest->Insert_Collider(pSrc, COL_DIR::DIR_RIGHT);
					return true;
				}
			}
			else // Z�� ���� ������
			{
				// src �ĸ��浹
				if (pSrc->Get_BoundCenter().z < pDest->Get_BoundCenter().z)
				{
					pSrc->Insert_Collider(pDest, COL_DIR::DIR_BACK);
					pDest->Insert_Collider(pSrc, COL_DIR::DIR_FRONT);
					return true;
				}
				// src �����浹
				else
				{
					pSrc->Insert_Collider(pDest, COL_DIR::DIR_FRONT);
					pDest->Insert_Collider(pSrc, COL_DIR::DIR_BACK);
					return true;
				}
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
		*pX = (fRadiusX - fX)/ fRadiusX;
		*pY = (fRadiusY - fY)/ fRadiusY;
		*pZ = (fRadiusZ - fZ)/ fRadiusZ;
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
				iter = m_ColliderList[i].erase(iter);
				break;
			}
			else
				++iter;
		}
	}
}

vector<RayCollision> CCollisionMgr::Check_Collision_Ray(RAYCAST ray, CCollider* shootObj, vector<_tchar*> tagName)
{
	//obj�� �Ÿ��� �����ϰ�, �Ÿ������� �������ָ� �ɵ�.

	vector<RayCollision> colList;
	//�浹������ �׷��� ������ ����
	for (int i = 0; i < COL_END; i++)
	{
		if (m_ColliderList[i].empty())
			continue;

		_float pDist;
		//�Ÿ� �ȿ� �ִ� ��� ����Ʈ�� ��ȸ
		for (auto& iter = m_ColliderList[i].begin();
		iter != m_ColliderList[i].end(); ++iter)
		{
			//�ڱ� �ڽ��̸� Ž������ ����
			if (shootObj == *iter)
				continue;

			//�����Ÿ� ���̸� Ž������ ����
			if (ray._Length * 3 <= D3DXVec3Length(&_vec3(ray._origin - (*iter)->m_pGameObject->m_pTransform->m_vInfo[INFO_POS])))
				continue;

			if (tagName.size() == 0)
			{
				if (Collision_Ray(ray, *iter, &pDist))
					colList.push_back(RayCollision{ (*iter)->m_pGameObject->m_pTag,*iter,pDist });
				continue;
			}

			//for ���� ���� ã�� iter�� �迭�� ��ϵ� �±����� Ȯ��
 			for (int i = 0; i < tagName.size(); i++)
			{
				if (!lstrcmp((*iter)->m_pGameObject->m_pTag, tagName[i]))
				{
					if (Collision_Ray(ray, *iter, &pDist))
						colList.push_back(RayCollision{ (*iter)->m_pGameObject->m_pTag,*iter,pDist });
				}
			}

		}
	}

	sort(colList.begin(), colList.end(), [](RayCollision& iter, RayCollision& iter2)->_bool {
		return iter.dist < iter2.dist;
	});

	return colList;
}

_bool CCollisionMgr::Collision_Ray(RAYCAST ray, CCollider * pDest,float* pDist)
{
	BOOL returnValue;

	//ray�� �� ������Ʈ�� local��ǥ�� ��������.
	RAYCAST __ray = ray;
	_matrix matWorld;
	D3DXMatrixInverse(&matWorld, 0, &(pDest->m_pGameObject->m_pTransform->m_matWorld));
	D3DXVec3TransformCoord(&__ray._origin, &ray._origin, &matWorld);
	D3DXVec3TransformNormal(&__ray._direction, &ray._direction, &matWorld);

	D3DXIntersect(pDest->Get_Mesh(),
		&__ray._origin,
		&__ray._direction,
		&returnValue,
		NULL,
		NULL,
		NULL,
		pDist,
		NULL,
		NULL);

	if (returnValue)
		if (*pDist < ray._Length)
			return true;
			
	return false;
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
		//for_each(m_ColliderList[i].begin(), m_ColliderList[i].end(), CDeleteObj());
		m_ColliderList[i].clear();
	}
}

void CCollisionMgr::Free(void)
{
	Clear_Collision();
}