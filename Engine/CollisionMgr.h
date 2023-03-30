#pragma once
#include "Base.h"
#include "Collider.h"

BEGIN(Engine)
enum COLGROUP
{
	COL_ENV,
	COL_OBJ,
	COL_END
};

class CCollisionMgr : public CBase
{
	DECLARE_SINGLETON(CCollisionMgr)

private:
	explicit CCollisionMgr();
	virtual ~CCollisionMgr();
public:
	void		Add_Collider(COLGROUP eID, CCollider* pCollider);
	void		Check_Collision();

	_bool		Collision_Box(CCollider* pSrc, CCollider* pDest);
	_bool		Check_BoundingBox(CCollider * pSrc, CCollider * pDest, _float * pX, _float * pY, _float * pZ);
	void		Clear_Collision();
private:
	vector<CCollider*>	m_ColliderList[COL_END];

private:
	virtual void Free(void) override;
};

END