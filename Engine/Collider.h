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
	bool m_bEnabled;
	bool m_bIsTrigger;
	bool m_bIsCollision;
	
public:
	HRESULT	Ready_Collider(_bool bIsTrigger);
	virtual _int Update_Component(const _float& fTimeDelta) override;
	virtual void LateUpdate_Component() override;
	virtual void Render_Component() override;
public:
	void Get_Point(_vec3* MinPoint, _vec3* MaxPoint)
	{
		*MinPoint = m_pBoundingBox->_min;
		*MaxPoint = m_pBoundingBox->_max;
	}
	bool Intersect(const _vec3& point) { return m_pBoundingBox->Intersect(point); }
	_vec3 Get_BoundCenter() { return m_pBoundingBox->Get_Center(); }
	_vec3 Get_BoundSize() { return m_pBoundingBox->Get_Size();}
	void OnCollisionEnter(const Collision* collision);
	void OnCollisionStay(const Collision* collision);
	void OnCollisionExit(const Collision* collision);

	void OnTriggerEnter(const CCollider* other);
	void OnTriggerStay(const CCollider* other);
	void OnTirggerExit(const CCollider* other);

	void Set_Enable(_bool bEnabled) { m_bEnabled = bEnabled; }
	void Set_BoundingBox(const _vec3& vSize = {2.f, 2.f, 2.f});

public:
	static CCollider* Create(LPDIRECT3DDEVICE9 pGraphicDev, _bool bIsTrigger);
	virtual CComponent * Clone(void) override;
private:
	virtual void Free(void) override;
};

END