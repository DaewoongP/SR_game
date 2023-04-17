#pragma once

#include "Include.h"
#include "GameObject.h"
#include "TopdeeParts.h"
BEGIN(Engine)

class CRcTex;
class CTexture;
class CCollider;
class CSlerpParticle;
class CShadow;
END
class CTopdee : public Engine::CGameObject
{
private:
	explicit CTopdee(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CTopdee();

public:
	virtual HRESULT Ready_GameObject(_vec3& vPos) override;
	virtual _int Update_GameObject(const _float& fTimeDelta) override;
	virtual _int Update_Too(const _float& fTimeDelta) override;
	virtual _int Update_Top(const _float& fTimeDelta) override;
	virtual void LateUpdate_GameObject(void) override;
	virtual void Render_GameObject(void) override;
	virtual void OnCollisionEnter(const class Collision* collision);
	virtual void OnCollisionStay(const class Collision* collision);
	virtual void SwapTrigger();
private:
	HRESULT		Add_Component(void);
	void		Key_Input(const _float& fTimeDelta);
	void		RayDiskey();
	void		RayDisKey_part(COL_MOVEDIR dir);
	void		PlayerState(const _float& fTimeDelta);
	void		Move(const _float& fTimeDelta);
	_bool		CheckCubeExist(_vec3 dir, CCollider** col);

	_bool		CheckAnythingExist(_vec3 dir, CCollider** col);
	void		DirApply(_int dir,_int& x,_int& y);

	void		Set_SlerpParticle();
public:
	void		SetRenderONOFF(_bool value) { m_bRender = value; }
	void		SetMovePos(COL_DIR dir);
	void		SetMovePos_zero();
	void		Set_AnimDead() { m_pTextureCom->Switch_Anim(L"Die"); }

private:
	Engine::CRcTex*			m_pBufferCom;
	Engine::CTexture*		m_pTextureCom;
	Engine::CCollider*		m_pCollider;
	Engine::CSlerpParticle*	m_pSlerpParticle;
	Engine::CShadow*		m_pShadow;

	//      LRUD
	// 0000 0000
	_byte					m_byPlayerInputDir;
	_byte					m_byLookDir;
	_float					m_fSpeed = 10.f;
	_vec3					m_MovetoPos;
	_bool					m_bIsMoving = false;
	_bool					m_bIsMoveKeyPress = false;

	CGameObject*			m_pGrabObj = nullptr;
	_bool					m_bRender;
	TOPDEESTATE				m_eState = TD_MOVE;

	vector<CTopdeeParts*>   m_partVec;
	_bool					m_bInit;

public:
	void TopdeeStateChange(TOPDEESTATE state) { m_eState = state; }
public:
	static CTopdee*		Create(LPDIRECT3DDEVICE9 pGraphicDev, _vec3& vPos);

private:
	virtual void Free(void) override;

};

