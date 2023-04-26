#pragma once
#include "Monster.h"

#define MINIPIGSCALE 1.2f

BEGIN(Engine)

class CTexture;

END

class CMiniGamePig :
	public CMonster
{
private:
	explicit CMiniGamePig(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CMiniGamePig();

public:
	virtual HRESULT Ready_GameObject(_vec3& vPos) override;
	virtual _int Update_GameObject(const _float & fTimeDelta) override;
	virtual void LateUpdate_GameObject(void) override;
	virtual void Render_GameObject(void) override;

private:
	HRESULT Add_Component(void);

	virtual _int Update_Too(const _float & fTimeDelta);
	virtual _int Update_Top(const _float & fTimeDelta);
	virtual void Render_Too();
	virtual void Render_Top();

	virtual void OnCollisionEnter(const Collision * collision);
	virtual void OnCollisionStay(const Collision * collision);
	virtual void OnCollisionExit(const Collision * collision);
public:
	static CMiniGamePig*	Create(LPDIRECT3DDEVICE9 pGraphicDev, _vec3& vPos);

private:
	_bool ShootRay(_vec3 dir,_vec3 pos);

private:
	virtual void Free(void) override;

private:
	list<CCollider*> m_colvec;
	_bool m_IsOnGround;
	_float m_dwDieTimer;
	_bool					m_DoStop_Mini;
	_vec3					m_vFinalLerpPos;
	CTransform*				m_pPortalTrans;
	_float					m_fSpinAngle;
	_float					m_fSpinDist;
	_float					m_fScale;
public:
	void SetDoStop_Mini(CTransform* portalTrans) {
		m_fSpinDist = 1.f;
		m_fScale = 0.5f;
		m_DoStop_Mini = true;
		m_vFinalLerpPos = portalTrans->m_vInfo[INFO_POS];
		m_pPortalTrans = portalTrans;
		m_pRigid->m_bUseGrivaty = false;
		m_pRigid->m_Velocity = _vec3(0, 0, 0);
		m_pTransform->m_vScale.y = 1.8f;
		m_pTextureCom->Switch_Anim(L"Idle");
	}
private:
	Engine::CTexture* m_pTextureCom_Back;
	Engine::CShadow* m_pShadowCom;

	_bool m_bMoveLeft;

	_bool m_bBackSprite;

};

