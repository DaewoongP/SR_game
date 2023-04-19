#include "Include.h"
#include "GameObject.h"
#include "Topdee.h"
#include "Toodee.h"

class CTookee : public  CGameObject
{
	friend CTopdee;
	friend CToodee;
protected:
	explicit CTookee(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CTookee();

public:
	virtual HRESULT Ready_GameObject(_vec3& vPos) override;
	virtual _int Update_GameObject(const _float& fTimeDelta) override;
	_int Update_Too(const _float& fTimeDelta) override;
	virtual _int Update_Top(const _float& fTimedDelte) override;
	virtual void LateUpdate_GameObject(void) override;
	virtual void Render_GameObject(void) override;
	virtual void SwapTrigger();

	virtual void OnCollisionEnter(const class Collision* collision);
	virtual void OnCollisionStay(const class Collision* collision);
	virtual void OnCollisionExit(const class Collision* collision);

#pragma region TooDee
public:
	virtual void		Set_AnimDead() { m_pTextureCom->Switch_Anim(L"Die"); }
	virtual void		SetRenderONOFF_too(_bool value) { m_bRender_too = value; }

protected:
	virtual void		Key_Input(const _float& fTimeDelta);
	virtual void		DoFlip();
	virtual void		DoStrech();

	virtual void Render_Particle();
	virtual void Check_IsParticleDead();
	virtual void Set_WalkParticle();
	virtual void LandingParticle_logic(const _tchar* pTag);

private:
	Engine::CRcTex*			m_pBufferCom;
	Engine::CTexture*		m_pTextureCom;

protected:
	Engine::CRigidbody*		m_pRigid;
	Engine::CJumpParticle*	m_pJumpParticle;
	Engine::CLandingParticle*	m_pLandingParticle;
	Engine::CSparkParticle*		m_pSparkParticle;

	_bool					m_bRender_too;
	_bool					m_bJumpable;
	_float					m_fSpeed = 10.f;
	_float					m_fWalkTime = 0.f;
	COL_DIR					m_eKeyState;
	_bool					m_bInit2;
	_bool					m_moveTrue;
#pragma endregion

#pragma region Topdee
protected:
	virtual void		Key_Input2(const _float& fTimeDelta);
	virtual void		RayDiskey();
	virtual void		RayDisKey_part(COL_MOVEDIR dir);
	virtual void		PlayerState(const _float& fTimeDelta);
	virtual void		Move(const _float& fTimeDelta);
	virtual _bool		CheckCubeExist(_vec3 dir, CCollider** col);
	virtual _bool		CheckAnythingExist(_vec3 dir, CCollider** col);
	virtual void		DirApply(_int dir, _int& x, _int& y);

public:
	virtual void		SetRenderONOFF(_bool value) { m_bRender = value; for (int i = 0; i < m_partVec.size(); i++)m_partVec[i]->SetRenderState(value); }
	virtual void		SetMovePos(COL_DIR dir);
	virtual void		SetMovePos_zero();

protected:
	Engine::CCollider*		m_pCollider;
	Engine::CShadow*		m_pShadow;
	Engine::CAnimation*     m_pAnimation_Arm;
	Engine::CAnimation*     m_pAnimation_Leg;
	//      LRUD
	// 0000 0000
	_byte					m_byPlayerInputDir;
	_byte					m_byLookDir;
	_vec3					m_LookVec;
	_vec3					m_MovetoPos;
	_bool					m_bIsMoving = false;
	_bool					m_bIsMoveKeyPress = false;

	CGameObject*			m_pGrabObj = nullptr;
	_bool					m_bRender;
	TOPDEESTATE				m_eState = TD_MOVE;

	vector<CTopdeeParts*>   m_partVec;
	_bool					m_bInit;
	_bool					m_bWalkingAnim;

public:
	virtual void TopdeeStateChange(TOPDEESTATE state) { m_eState = state; }

#pragma endregion

private:
	HRESULT		Add_Component(void);

public:
	static CTookee*		Create(LPDIRECT3DDEVICE9 pGraphicDev, _vec3& vPos);

protected:
	virtual void Free(void) override;
};

