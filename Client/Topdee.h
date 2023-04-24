#pragma once

#include "Include.h"
#include "GameObject.h"
#include "TopdeeParts.h"
#include "TopdeeJoint.h"

class CTookee;

BEGIN(Engine)
class CRcTex;
class CCollider;
class CSlerpParticle;
class CShadow;
class CAnimation;
END
class CTopdee :
	public CGameObject
{
protected:
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
protected:
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
	void		SetRenderONOFF(_bool value) { m_bRender = value; for (int i = 0; i < m_partVec.size(); i++)m_partVec[i]->SetRenderState(value);	}
	void		SetMovePos(COL_DIR dir);
	void		SetMovePos_zero();
	void		SetTookee(CTookee* third) { m_Tookee=third; }

	void		SetDie();

protected:
	Engine::CCollider*		m_pCollider;
	Engine::CSlerpParticle*	m_pSlerpParticle;
	Engine::CShadow*		m_pShadow;
	Engine::CAnimation*     m_pAnimation_Arm;
	Engine::CAnimation*     m_pAnimation_Leg;
	//      LRUD
	// 0000 0000
	_byte					m_byPlayerInputDir;
	_byte					m_byLookDir;
	_vec3					m_LookVec;
	_float					m_fSpeed = 10.f;
	_vec3					m_MovetoPos;
	_bool					m_bIsMoving = false;
	_bool					m_bIsMoveKeyPress = false;

	CGameObject*			m_pGrabObj = nullptr;
	_bool					m_bRender;
	TOPDEESTATE				m_eState = TD_MOVE;

	vector<CTopdeeParts*>   m_partVec;
	_bool					m_bInit;
	_bool					m_bWalkingAnim;
	CTookee*				m_Tookee;

public:
	void TopdeeStateChange(TOPDEESTATE state) { m_eState = state; }
public:
	static CTopdee*		Create(LPDIRECT3DDEVICE9 pGraphicDev, _vec3& vPos);

protected:
	virtual void Free(void) override;

};

