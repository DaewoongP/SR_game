#pragma once
#include "GameObject.h"
#include "Include.h"

enum GIANTHANDSTATE{
	GH_IDLE,
	GH_STUMP,
	GH_UP,
	GH_END
};

BEGIN(Engine)
class CCollider;
class CBoxParticle;
END
class CGiantHand :
	public CGameObject
{
private:
	explicit CGiantHand(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CGiantHand();

public:
	virtual HRESULT Ready_GameObject(_vec3& vPos);

	virtual _int Update_GameObject(const _float& fTimeDelta) override;
	virtual void LateUpdate_GameObject(void) override;
	virtual void Render_GameObject(void) override;

	virtual void OnCollisionEnter(const class Collision* collision);
	virtual void OnCollisionStay(const class Collision* collision);
	virtual void OnCollisionExit(const class Collision* collision);
private:
	void Do_Stump(const _float & fTimeDelta);
	void Do_Up(const _float & fTimeDelta);

public:
	void	SetState(GIANTHANDSTATE state) { m_eState = state; }
	void	Weight_Reset() { m_fweight = 0; }
protected:
	Engine::CCollider*		m_pCollider;
	Engine::CBoxParticle*	m_pBoxParticle;
	vector<CTransform*>		m_PartsVec;
	_bool					m_bInit;
	_vec3					m_vSummonPos;
	_float					m_fweight;
	GIANTHANDSTATE			m_eState;
	_bool					m_bCollision;
	_bool					m_bStop;
protected:
	HRESULT		Add_Component(void);

public:
	static CGiantHand*		Create(LPDIRECT3DDEVICE9 pGraphicDev, _vec3& vPos);
protected:
	virtual void Free(void) override;

};