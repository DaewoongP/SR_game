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
	void	LerpClipAdd(AnimClip* clip,_int idx,_float itv, _float osc,_float csc,_vec3 otr,_vec3 ctr,_vec3 orot,_vec3 crot,_int count);

protected:
	Engine::CCollider*		m_pCollider;
	Engine::CSlerpParticle*	m_pSlerpParticle;
	Engine::CShadow*		m_pShadow;
	Engine::CAnimation*     m_pAnimation_Whole;
	Engine::CAnimation*     m_pAnimation_Face;
	
	CTransform*				m_Player;
	// 0 ∏ˆ≈Î 1 ∏”∏Æ≈Î 2
	vector<CTransform*>		m_PartsVec;
	_bool					m_bInit;

public:
	static CBoss1*		Create(LPDIRECT3DDEVICE9 pGraphicDev, _vec3& vPos);

protected:
	virtual void Free(void) override;

};

