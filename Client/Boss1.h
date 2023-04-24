#pragma once
#include "Include.h"
#include "GameObject.h"
#include "Boss1Parts.h"

#define BOSS1_STATE_FUNC vector<void(CBoss1::*)(const _float& fTimeDelta)>

BEGIN(Engine)
class CCollider;
class CSlerpParticle;
class CShadow;
class CAnimation;
END

enum BOSS1STATE
{
	B1_FINGER,
	B1_HEAD,
	B1_GIANT,
	B1_END
};

class CBoss1 :
	public CGameObject
{
protected:
	explicit CBoss1(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CBoss1();

public:
	virtual HRESULT Ready_GameObject(_vec3& vPos) override;
	virtual _int Update_GameObject(const _float& fTimeDelta) override;
	virtual void LateUpdate_GameObject(void) override;
	virtual void Render_GameObject(void) override;
	virtual void SwapTrigger();
protected:
	HRESULT		Add_Component(void);
	void		LerpClipAdd(AnimClip* clip,_int idx,_float itv, _float osc,_float csc,_vec3 otr,_vec3 ctr,_vec3 orot,_vec3 crot,_int count);
	void		SetPattern();
	void		Do_SummonFinger(const _float& fTimeDelta);
	void		Do_EndFinger(const _float& fTimeDelta);
	void		Do_SummonHead(const _float& fTimeDelta);
	void		Do_EndHead(const _float& fTimeDelta);
	void		Do_SummonGiant(const _float& fTimeDelta);
	void		Do_IngGiant(const _float& fTimeDelta);
	void		Do_UpGiant(const _float& fTimeDelta);
	void		Do_EndGiant(const _float& fTimeDelta);
	void		Do_Rest(const _float& fTimeDelta);
	void		CheckIsLastActionIdx();
	void		ReadyPartten();

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
	vector<BOSS1_STATE_FUNC> funcAction;
	_int		m_iCurrentActionIdx;
	BOSS1STATE m_eCurrentState;

	CGameObject* m_GiantHand;

public:
	static CBoss1*		Create(LPDIRECT3DDEVICE9 pGraphicDev, _vec3& vPos);

protected:
	virtual void Free(void) override;

};

