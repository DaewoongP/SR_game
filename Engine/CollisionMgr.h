#pragma once
#include "Base.h"
#include "Collider.h"

BEGIN(Engine)

class CCollisionMgr : public CBase
{
	DECLARE_SINGLETON(CCollisionMgr)

private:
	explicit CCollisionMgr();
	virtual ~CCollisionMgr();
public:
	void		Add_Collider(CCollider* pCollider);
	void		Check_Collision(COLGROUP eGroup1, COLGROUP eGroup2);

	_bool		Collision_Range(CCollider* pSrc, CCollider* pDest);
	_bool		Collision_Box(CCollider* pSrc, CCollider* pDest);
	_bool		Check_BoundingBox(CCollider * pSrc, CCollider * pDest, _float * pX, _float * pY, _float * pZ);
	void		Delete_Collider(CGameObject* pGameObject);

	vector<RayCollision>		Check_Collision_Ray(RAYCAST ray, CCollider* shootObj);
	_bool		Collision_Ray(RAYCAST ray, CCollider* pDest, float* pDist);

	void		Set_Collider(COLGROUP eGroup, CCollider* pCollider);
	void		Clear_Collision();
private:
	vector<CCollider*>	m_ColliderList[COL_END];
	// 충돌 감지 범위 설정 (콜라이더 박스중 가장 큰 길이에 곱해주어 사용함)
	_float				m_fRangeOffset;
private:
	virtual void Free(void) override;
};

END