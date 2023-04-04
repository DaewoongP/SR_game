#pragma once

#include "Include.h"
#include "GameObject.h"

BEGIN(Engine)

class CRcTex;
class CTexture;
class CCollider;
class CRigidbody;
class CSnow;
END
class CPlayer : public Engine::CGameObject
{
private:
	explicit CPlayer(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CPlayer();

public:
	virtual HRESULT Ready_GameObject(void) override;
	virtual _int Update_GameObject(const _float& fTimeDelta) override;
	virtual _int Update_Too(const _float& fTimeDelta) override;
	virtual _int Update_Top(const _float& fTimeDelta) override;
	virtual void LateUpdate_GameObject(void) override;
	virtual void LateUpdate_Too() override;
	virtual void LateUpdate_Top() override;
	virtual void Render_GameObject(void) override;
	virtual void Render_Too() override;
	virtual void Render_Top() override;

	virtual void OnCollisionEnter(const class Collision* collision);
	virtual void OnCollisionStay(const class Collision* collision);
	virtual void OnCollisionExit(const class Collision* collision);
private:
	HRESULT		Add_Component(void);
	void		Key_Input(const _float& fTimeDelta);
	_float		Lerp(_float v0, _float v1, _float t) { return v0 + t*(v1 - v0); }
	void		DoFlip();

private:
	Engine::CRcTex*			m_pBufferCom;
	Engine::CTexture*		m_pTextureCom;
	Engine::CCollider*		m_pCollider;
	Engine::CRigidbody*		m_pRigid;
	//Engine::CSnow * m_praticle;

	_float					m_fSpeed = 10.f;
	bool					m_bJumpalbe;
	//콜리젼 아님. 어떤 키 눌렀는지 확인용임.
	COL_DIR					m_eKeyState;
public:
	static CPlayer*		Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	virtual void Free(void) override;

};

