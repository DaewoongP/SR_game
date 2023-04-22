#pragma once
#include "GameObject.h"
#include "Include.h"

class CBoss1HeadSub :
	public CGameObject
{
private:
	explicit CBoss1HeadSub(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CBoss1HeadSub();

	virtual HRESULT Ready_GameObject(_vec3& vPos, _vec3 & vtoward);

	virtual _int Update_GameObject(const _float& fTimeDelta) override;
	virtual void LateUpdate_GameObject(void) override;
	virtual void Render_GameObject(void) override;

protected:
	Engine::CCollider*		m_pCollider;
	vector<CTransform*>		m_PartsVec;
	_bool					m_bInit;

	_vec3					m_vMoveDir;
	_float					m_fSpeed;

protected:
	HRESULT		Add_Component(void);

public:
	static CBoss1HeadSub*		Create(LPDIRECT3DDEVICE9 pGraphicDev, _vec3& vPos, _vec3 & vtoward);
protected:
	virtual void Free(void) override;

};

