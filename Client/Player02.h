#pragma once

#include "Include.h"
#include "GameObject.h"
BEGIN(Engine)

class CRcTex;
class CTexture;
class CCollider;
END
class CPlayer02 : public Engine::CGameObject
{
private:
	explicit CPlayer02(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CPlayer02();

public:
	virtual HRESULT Ready_GameObject(_vec3& vPos) override;
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
	void		RayDiskey();
	void		PlayerState(const _float& fTimeDelta);
	void		Move(const _float& fTimeDelta);
	_bool		CheckCubeExist(_vec3 dir, CCollider** col);
	_bool		CheckAnythingExist(_vec3 dir, CCollider** col);

private:
	Engine::CRcTex*			m_pBufferCom;
	Engine::CTexture*		m_pTextureCom;
	Engine::CCollider*		m_pCollider;

	//      LRUD
	// 0000 0000
	_byte					m_byPlayerInputDir;
	_byte					m_byLookDir;
	_float					m_fSpeed = 10.f;
	_vec3					m_MovetoPos;
	_bool					m_bIsMoving = false;
	_bool					m_bIsMoveKeyPress = false;

	CGameObject*			m_pGrabObj = nullptr;

	TOPDEESTATE				m_eState = TD_MOVE;

	_vec3					prePos;

public:
	void Player02StateChange(TOPDEESTATE state) { m_eState = state; }
public:
	static CPlayer02*		Create(LPDIRECT3DDEVICE9 pGraphicDev, _vec3& vPos);

private:
	virtual void Free(void) override;

};

