#pragma once
#include "Include.h"
#include "GameObject.h"

#define APPEAR_YPOS 50.0f

#define FALLINGDEST_YPOS 10.0f

BEGIN(Engine)
class CRcTex;
class CTexture;
class CCollider;
class CRigidbody;
END
class CBoss2Stone :
	public CGameObject
{
private:
	explicit CBoss2Stone(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CBoss2Stone();

public:
	virtual HRESULT Ready_GameObject(_vec3& vPos) override;
	virtual _int Update_GameObject(const _float& fTimeDelta) override;
	virtual void LateUpdate_GameObject(void) override;
	virtual void Render_GameObject(void) override;
	virtual void SwapTrigger();

	virtual void OnCollisionEnter(const class Collision* collision);
	virtual void OnCollisionStay(const class Collision* collision);
	virtual void OnCollisionExit(const class Collision* collision);

	void Appear(const _float& fTimeDelta);
	void Aim(const _float& fTimeDelta);
	void Rest(const _float& fTimeDelta);
	void GoToTarget(const _float& fTimeDelta);
	void Dead(const _float& fTimeDelta);
private:
	HRESULT		Add_Component(void);
private:
	Engine::CRcTex*			m_pBufferCom;
	Engine::CTexture*		m_pTextureCom;
	Engine::CCollider*		m_pCollider;
	Engine::CRigidbody*		m_pRigid;

	_vec3					m_vTarget;

	CTransform*			m_pToodeeTrans;
	CTransform*			m_pTopdeeTrans;

	vector<void(CBoss2Stone::*)(const _float& fTimeDelta)> m_vStateSequence;

	_int	m_iIndex;

	_float	m_fTimer;

public:
	static CBoss2Stone*		Create(LPDIRECT3DDEVICE9 pGraphicDev, _vec3& vPos);

private:
	virtual void Free(void) override;
};

