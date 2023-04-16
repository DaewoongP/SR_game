#pragma once
#include "Component.h"
#include "Transform.h"
#include "GameObject.h"

BEGIN(Engine)
enum COLOR
{
	RED,
	GREEN
};
class CCollider : public CComponent
{
protected:
	explicit CCollider(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CCollider(const CCollider& rhs);
	virtual ~CCollider();

protected:
	// 감지 영역
	BoundingBox*		m_pBoundingBox;
	// 사각형 렌더링
	LPD3DXMESH			m_pMesh;
	COLGROUP			m_eGroup;
	// 현재 충돌중인 상대 콜라이더와의 정보 -> 복사생성시 복사할 필요 없음. 실시간 정보
	map<CCollider*, Collision>	m_ColList;
public:
	_bool	m_bIsTrigger;
public:
	HRESULT	Ready_Collider();
	virtual _int Update_Component(const _float& fTimeDelta) override;
	virtual void LateUpdate_Component() override;
	virtual void Render_Component() override;
public:
	// 콜라이더 옵션설정 
	// 1. 충돌범위 사이즈 vec3type(width, height, depth)
	// 2. 콜라이더 그룹 설정 기본값 OBJ
	// 3. 트리거 설정 기본값 false
	HRESULT Set_Options(const _vec3& vSize = { 2.f, 2.f, 2.f }, COLGROUP eGroup = COL_OBJ, _bool isTrigger = false);
	void Set_Group(COLGROUP eGroup);
	COLGROUP Get_Group() { return m_eGroup; }
	void Get_Point(_vec3* MinPoint, _vec3* MaxPoint)
	{
		*MinPoint = m_pBoundingBox->_min;
		*MaxPoint = m_pBoundingBox->_max;
	}
	// 현재 충돌이 일어난 상대 콜라이더, 상태값 저장
	void Insert_Collider(CCollider* pCollider, COL_DIR eDir);
	// 콜라이더가 리스트에 있는지 확인후 반환, 콜리전 output
	_bool	Find_ColList(class CCollider* pOtherCol, Collision** collision);
	_bool Delete_OtherCollider(CCollider* pOtherCol);

	bool Intersect(const _vec3& point) { return m_pBoundingBox->Intersect(point); }
	void Set_BoundOffset(_vec3& pOffset) { m_pBoundingBox->Set_Offset(pOffset); }
	_vec3 Get_BoundOffset() { return m_pBoundingBox->Get_Offset(); }
	_vec3 Get_BoundCenter() { return m_pBoundingBox->Get_Center(); }
	_vec3 Get_BoundSize() { return _vec3(	m_pBoundingBox->Get_Size().x,	m_pBoundingBox->Get_Size().y,
		m_pBoundingBox->Get_Size().z);}
	void OnCollisionEnter(const Collision* collision);
	void OnCollisionStay(const Collision* collision);
	void OnCollisionExit(const Collision* collision);

	void Set_BoundingBox(const _vec3& vSize = { 2.f, 2.f, 2.f }, const _vec3& vOffsetPos = {0.f, 0.f, 0.f});
	LPD3DXMESH Get_Mesh() { return m_pMesh; }

public:
	static CCollider* Create(LPDIRECT3DDEVICE9 pGraphicDev, _bool bIsTrigger);
	virtual CComponent * Clone(void) override;
private:
	void DrawColor();

private:
	LPDIRECT3DBASETEXTURE9		m_pRedTex;
	LPDIRECT3DBASETEXTURE9		m_pGreenTex;
	_matrix						m_matWorld;
	COLOR						m_eColor;
	_bool						m_bRender;
private:
	virtual void Free(void) override;
};

END