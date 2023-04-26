#pragma once

#include "Include.h"
#include "GameObject.h"


class CTookee;

BEGIN(Engine)
class CRcTex;
class CTexture;
class CCollider;
class CRigidbody;
class CJumpParticle;
class CLandingParticle;
class CSparkParticle;
class CSlerpParticle;
class CShadow;
class CShader;
END
class CToodee : public CGameObject
{
protected:
	explicit CToodee(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CToodee();

public:
	virtual HRESULT Ready_GameObject(_vec3& vPos) override;
	virtual _int Update_GameObject(const _float& fTimeDelta) override;
	virtual _int Update_Too(const _float& fTimeDelta) override;
	virtual _int Update_Top(const _float& fTimedDelte) override;
	virtual void LateUpdate_GameObject(void) override;
	virtual void Render_GameObject(void) override;
	virtual void SwapTrigger();

	virtual void OnCollisionEnter(const class Collision* collision);
	virtual void OnCollisionStay(const class Collision* collision);
	virtual void OnCollisionExit(const class Collision* collision);
	void		Set_AnimDead() { m_pTextureCom->Switch_Anim(L"Die"); }
	void		SetRenderONOFF(_bool value) { m_bRender = value; }
	void		SetTookee(CTookee* third) { m_Tookee = third; }
	void		SetMovePos_zero() { m_prePos = m_pTransform->m_vInfo[INFO_POS].x; }
protected:
	HRESULT		Add_Component(void);
	void		Key_Input(const _float& fTimeDelta);
	_float		Lerp(_float v0, _float v1, _float t) { return v0 + t*(v1 - v0); }
	void		DoFlip();
	void		DoStrech();

	void Render_Particle();
	void Check_IsParticleDead();
	void Set_WalkParticle();
	void LandingParticle_logic(const _tchar* pTag);
	void Set_SlerpParticle();

protected:
	Engine::CCollider*		m_pCollider;
	Engine::CRcTex*			m_pBufferCom;
	Engine::CTexture*		m_pTextureCom;
	Engine::CRigidbody*		m_pRigid;
	Engine::CJumpParticle*	m_pJumpParticle;
	Engine::CLandingParticle*	m_pLandingParticle;
	Engine::CSparkParticle*		m_pSparkParticle;
	Engine::CSlerpParticle*		m_pSlerpParticle;
	Engine::CShadow*		m_pShadow;
	Engine::CShader*		m_pShader;
	CTookee*				m_Tookee;
	_bool					m_bRender;
	_bool					m_bInit;
	_float					m_fInitYScale;

	_float					m_prePos;

	_float					m_fSpeed = 10.f;
	bool					m_bJumpable;
	_float					m_fWalkTime = 0.f;

	//콜리젼 아님. 어떤 키 눌렀는지 확인용임.
	COL_DIR					m_eKeyState;
public:
	static CToodee*		Create(LPDIRECT3DDEVICE9 pGraphicDev, _vec3& vPos);

protected:
	virtual void Free(void) override;

};

