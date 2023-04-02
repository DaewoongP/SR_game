#pragma once
#include "Component.h"

BEGIN(Engine)

class CCollider : public CComponent
{
protected:
	explicit CCollider(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CCollider(const CCollider& rhs);
	virtual ~CCollider();

protected:
	// 감지 영역
	BoundingBox* m_pBoundingBox;
	// 사각형 렌더링
	LPD3DXMESH			m_pMesh;
	COLGROUP			m_eGroup;
	// 현재 충돌중인 상대 콜라이더와의 정보 -> 복사생성시 복사할 필요 없음. 실시간 정보
	map<CCollider*, Collision*>	m_Colmap;
public:
	HRESULT	Ready_Collider();
	virtual _int Update_Component(const _float& fTimeDelta) override;
	virtual void LateUpdate_Component() override;
	virtual void Render_Component() override;
public:
	void Set_Group(COLGROUP eGroup);
	COLGROUP Get_Group() { return m_eGroup; }
	void Get_Point(_vec3* MinPoint, _vec3* MaxPoint)
	{
		*MinPoint = m_pBoundingBox->_min;
		*MaxPoint = m_pBoundingBox->_max;
	}
	// 현재 충돌이 일어난 상대 콜라이더, 상태값 저장
	void Insert_Collider(CCollider* pCollider, COL_DIR eDir);
	// 맵안에 해당하는 콜라이더의 상태값을 반환
	Collision* Find_ColState(class CCollider* pOtherCol);
	// 이미 리스트에 있는지 체크하는 함수
	_bool	Check_AlreadyCol(class CCollider* pOtherCol);
	_bool Delete_OtherCollider(CCollider* pOtherCol);

	bool Intersect(const _vec3& point) { return m_pBoundingBox->Intersect(point); }
	_vec3 Get_BoundCenter() { return m_pBoundingBox->Get_Center(); }
	_vec3 Get_BoundSize() { return m_pBoundingBox->Get_Size();}
	void OnCollisionEnter(const Collision* collision);
	void OnCollisionStay(const Collision* collision);
	void OnCollisionExit(const Collision* collision);

	void Set_BoundingBox(const _vec3& vSize = {2.f, 2.f, 2.f});

public:
	static CCollider* Create(LPDIRECT3DDEVICE9 pGraphicDev, _bool bIsTrigger);
	virtual CComponent * Clone(void) override;
private:
	virtual void Free(void) override;
};

END