#pragma once

#include "Include.h"
#include "GameObject.h"

BEGIN(Engine)

class CRcTex;
class CTexture;
class CCollider;
class CRigidbody;
END
class CPlayer02 : public Engine::CGameObject
{
private:
	explicit CPlayer02(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CPlayer02();

public:
	virtual HRESULT Ready_GameObject(void) override;
	virtual _int Update_GameObject(const _float& fTimeDelta) override;
	virtual void LateUpdate_GameObject(void) override;
	virtual void Render_GameObject(void) override;

	virtual void OnCollisionEnter(const class Collision* collision);
	virtual void OnCollisionStay(const class Collision* collision);
	virtual void OnCollisionExit(const class Collision* collision);
private:
	HRESULT		Add_Component(void);
	void		Key_Input(const _float& fTimeDelta);
	void		PlayerMove(const _float& fTimeDelta);
	_float		Lerp(_float v0, _float v1, _float t) { return v0 + t*(v1 - v0); }
	_bool		IsMoveDone(const _float& fTimeDelta);

private:
	Engine::CRcTex*			m_pBufferCom;
	Engine::CTexture*		m_pTextureCom;
	Engine::CCollider*		m_pCollider;

	//      LRUD
	// 0000 0000
	_byte					m_byPlayerInputDir;
	_byte					m_byPlayerMoveDir;
	_float					m_fSpeed = 10.f;
	_vec3					m_MovetoPos;
	_bool					m_bIsMoving = false;
	_bool					m_bIsMoveKeyPress = false;

public:
	static CPlayer02*		Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	virtual void Free(void) override;

};

