#pragma once
#include "GameObject.h"
#include "Include.h"

BEGIN(Engine)
class CCollider;
END
class CBoss1Head :
	public CGameObject
{
private:
	explicit CBoss1Head(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CBoss1Head();

	virtual HRESULT Ready_GameObject(_vec3& vPos);

	virtual _int Update_GameObject(const _float& fTimeDelta) override;
	virtual void LateUpdate_GameObject(void) override;
	virtual void Render_GameObject(void) override;

protected:
	Engine::CCollider*		m_pCollider;
	vector<CTransform*>		m_PartsVec;
	_bool					m_bInit;

protected:
	HRESULT		Add_Component(void);

public:
	static CBoss1Head*		Create(LPDIRECT3DDEVICE9 pGraphicDev, _vec3& vPos);
protected:
	virtual void Free(void) override;

};