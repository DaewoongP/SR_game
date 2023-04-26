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

private:
	Engine::CTexture* m_pTextureCom_Back;
	Engine::CShadow* m_pShadowCom;

	_bool m_bMoveLeft;
	_bool m_bSoundStack=true;
	_bool m_bBackSprite;
};

