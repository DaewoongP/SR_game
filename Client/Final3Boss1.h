#pragma once
#include "Include.h"
#include "GameObject.h"
#include "Boss1Parts.h"

BEGIN(Engine)
class CCollider;
class CSlerpParticle;
class CShadow;
class CAnimation;
END
class CFinal3Boss1 :
	public CGameObject
{
protected:
	explicit CFinal3Boss1(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CFinal3Boss1();

public:
	virtual HRESULT Ready_GameObject(_vec3& vPos) override;
	virtual _int Update_GameObject(const _float& fTimeDelta) override;
	virtual void LateUpdate_GameObject(void) override;
	virtual void Render_GameObject(void) override;
	virtual void SwapTrigger();
protected:
	HRESULT		Add_Component(void);
	void		LerpClipAdd(AnimClip* clip,_int idx,_float itv, _vec3 osc, _vec3 csc,_vec3 otr,_vec3 ctr,_vec3 orot,_vec3 crot,_int count);
	void		Move(const _float& fTimeDelta);

protected:
	Engine::CCollider*		m_pCollider;
	Engine::CSlerpParticle*	m_pSlerpParticle;
	Engine::CShadow*		m_pShadow;
	Engine::CAnimation*     m_pAnimation_Whole;
	Engine::CAnimation*     m_pAnimation_Face;
	
	CTransform*				m_Player;
	vector<CTransform*>		m_PartsVec;
	_bool					m_bInit;

	_bool					m_bTurn_x;
	_bool					m_bTurn_y;
	_float					m_fOffset_x;
	_float					m_fOffset_y;
	_vec3					m_vOriginPos;

	//공격 사용 주기(보스가 공격을 그리 자주쓰진 않음.)
	_float					m_dwRestTime;
public:
	static CFinal3Boss1*		Create(LPDIRECT3DDEVICE9 pGraphicDev, _vec3& vPos);

protected:
	virtual void Free(void) override;

};

