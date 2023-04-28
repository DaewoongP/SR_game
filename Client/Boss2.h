#pragma once
#include "Include.h"
#include "GameObject.h"

#define BOSS2_STATE_FUNC vector<void(CBoss2::*)(const _float& fTimeDelta)>
BEGIN(Engine)
class CRcTex;
class CTexture;
class CCollider;
class CRigidbody;
class CAnimation;
class CCircleParticle;
class CJumpParticle;
class CTexParticle;
class CSuperLandingParticle;
END

enum BOSS2STATE
{
	B2_JUMPING,
	B2_SCREAM,
	B2_PUNCH,
	B2_STUMP,
	B2_END
};

class CBoss2 :
	public CGameObject
{
private:
	explicit CBoss2(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CBoss2();

public:
	virtual HRESULT Ready_GameObject(_vec3& vPos, _int trigger);

	_int Update_GameObject_A0(const _float& fTimeDelta);
	void LateUpdate_GameObject_A0(void);
	void Render_GameObject_A0();

	_int Update_GameObject_A1(const _float& fTimeDelta);
	void LateUpdate_GameObject_A1(void);
	void Render_GameObject_A1();

	virtual _int Update_GameObject(const _float& fTimeDelta) override;
	virtual void LateUpdate_GameObject(void) override;
	virtual void Render_GameObject() override;

	virtual void OnCollisionEnter(const class Collision* collision);
	virtual void OnCollisionStay(const class Collision* collision);
	virtual void OnCollisionExit(const class Collision* collision);

	virtual void SwapTrigger();

private:
	_bool		Initialize_Update();

	HRESULT		Add_Component(void);
	HRESULT		Find_PlayerBoth();
	void		CheckZFloor();

	void		Do_Jump_Ready(const _float& fTimeDelta);
	void		Do_Jump_01(const _float& fTimeDelta);
	void		Do_Jump_02(const _float& fTimeDelta);
	void		Do_Rest(const _float& fTimeDelta);
	void		Do_ResetVelocity(const _float& fTimeDelta) { m_pRigid->m_Velocity = _vec3(0, 0, 0); m_pRigid->m_AngularVelocity = _vec3(0, 0, 0); m_pRigid->m_bUseGrivaty = false; CheckIsLastActionIdx(); }
	void		Do_Idle(const _float& fTimeDelta) { m_pAnimation_Body->SetAnimation(L"Idle"); m_dwRestTime = 2.0f; CheckIsLastActionIdx();}
	void		Do_Stump_Ready(const _float& fTimeDelta);
	void		Do_Chase_Player(const _float& fTimeDelta);
	void		Do_LittleUp_Turn(const _float& fTimeDelta);
	void		Do_Stump_02(const _float& fTimeDelta);
	void		Do_Turn_Minus(const _float& fTimeDelta);
	void        Do_Hurray(const _float& fTimeDelta);
	void        Do_SummonFist(const _float& fTimeDelta);
	void        Do_Scream(const _float& fTimeDelta);
	void		Do_ScreamEnd(const _float& fTimeDelta);
	void		Do_ResetRot(const _float& fTimeDelta) { m_pTransform->m_vAngle = _vec3(0, 0, 0); CheckIsLastActionIdx();}

	void		DoFlip();
	void		SetPartten();
	void		ReadyPartten();
	void		CheckIsLastActionIdx();
	void		Do_Standing(const _float& fTimeDelta);
	void		Do_SummonRock(const _float& fTimeDelta);
	void		Do_Throw(const _float& fTimeDelta);
	void		Do_ThrowEnd(const _float& fTimeDelta);
	void		Check_CircleParticle();
	_bool		AppearanceAction(const _float& fTimeDelta);
public:
	_bool		Get_Flip_Y() { return m_bFlip_Y; }

private:	
	BOSS2STATE m_eCurrentState;
	BOSS2STATE m_ePreState;
	_int		m_iCurrentActionIdx;
	_float		m_dwRestTime;
	_float		m_dwActionTime;
	_bool		m_bFlip_Y;
	vector<BOSS2_STATE_FUNC> funcAction;

	//?�프???�동??x �?
	_vec3	   m_fJumpPos[3];
	_int	   m_iJumpPosidx;

	CTransform* m_pPlayer01_trans;
	CTransform* m_pPlayer02_trans;

	_bool		m_bIsOnGround;
	_bool		m_bInit;
	_bool		m_bAttackAble;
	_int		m_iJumpCount;

	_float		m_dwApperance_Timer =9;
	_int		m_iAppearanceCnt = 0;
	_bool		m_bDeadAnim;

	_int		m_iSpecialActionTrigger;
	_int		m_JumpCnt;

private:
	Engine::CCollider*			m_pCollider;
	Engine::CRigidbody*			m_pRigid;
	Engine::CAnimation*			m_pAnimation_Face;
	Engine::CAnimation*			m_pAnimation_Body;
	Engine::CCircleParticle*	m_pCircleParticle;
	Engine::CJumpParticle*		m_pJumpParticle;
	Engine::CTexParticle*		m_pScreamParticle;
	Engine::CSuperLandingParticle*	m_pLandingParticle;
public:
	static CBoss2*		Create(LPDIRECT3DDEVICE9 pGraphicDev, _vec3& vPos);
	static CBoss2*		Create(LPDIRECT3DDEVICE9 pGraphicDev, _vec3& vPos,_int trigger);

private:
	virtual void Free(void) override;
};

