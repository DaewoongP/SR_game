#pragma once
#include "Include.h"
#include "GameObject.h"

//						반환?�?? 보스?�의 ?�수 ?�용?�기 ?�함, 매개변??
#define BOSS2_STATE_FUNC vector<void(CBoss2::*)(const _float& fTimeDelta)>

BEGIN(Engine)
class CRcTex;
class CTexture;
class CCollider;
class CRigidbody;
class CAnimation;
class CCircleParticle;
END

enum BOSS2STATE
{
	B2_THROW,
	B2_JUMPING,
	B2_SCREAM,
	B2_PUNCH,
	B2_STUMP,
	B2_END
};

//보스??머리?�임. 모든 ?�각/?�동??중추�??�당??
//보스??중력???��? ?�을거임. 보니�?중력 ?��?.
class CBoss2 :
	public CGameObject
{
private:
	explicit CBoss2(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CBoss2();

public:
	virtual HRESULT Ready_GameObject(_vec3& vPos) override;
	virtual _int Update_GameObject(const _float& fTimeDelta) override;
	virtual _int Update_Too(const _float& fTimeDelta) override;
	virtual _int Update_Top(const _float& fTimeDelta) override;
	virtual void LateUpdate_GameObject(void) override;
	virtual void Render_GameObject() override;
	virtual void SwapTrigger();

	virtual void OnCollisionEnter(const class Collision* collision);
	virtual void OnCollisionStay(const class Collision* collision);
	virtual void OnCollisionExit(const class Collision* collision);

private:
	HRESULT		Add_Component(void);
	HRESULT		Find_PlayerBoth();
	void		CheckZFloor();
	//?�프 ?�수 기본�??�정
	void		Do_Jump_Ready(const _float& fTimeDelta);
	//?�로 ?�승?�다
	void		Do_Jump_01(const _float& fTimeDelta);
	//착�??�다.
	void		Do_Jump_02(const _float& fTimeDelta);
	//?�식?�턴
	void		Do_Rest(const _float& fTimeDelta);
	void		Do_Rest_grivity(const _float& fTimeDelta);
	//벨로?�티 초기???�턴
	void		Do_ResetVelocity(const _float& fTimeDelta) { m_pRigid->m_Velocity = _vec3(0, 0, 0); m_pRigid->m_AngularVelocity = _vec3(0, 0, 0); m_pRigid->m_bUseGrivaty = false; CheckIsLastActionIdx(); }

	void		Do_Idle(const _float& fTimeDelta) { m_pAnimation_Body->SetAnimation(L"Idle"); m_dwRestTime = 2.0f; CheckIsLastActionIdx();}
	
	//?��????�수 기본�??�정
	void		Do_Stump_Ready(const _float& fTimeDelta);
	//toodee ?��? topdee�??�해 n초간 가?�는 ?�턴
	void		Do_Chase_Player(const _float& fTimeDelta);
	//y�??�짝 ?�라가�? ?�전??주는 ?�턴
	void		Do_LittleUp_Turn(const _float& fTimeDelta);
	//착�??�다.
	void		Do_Stump_02(const _float& fTimeDelta);
	//?�전 감소 ?�턴
	void		Do_Turn_Minus(const _float& fTimeDelta);
	//만세!
	void        Do_Hurray(const _float& fTimeDelta);
	//주먹 ?�환
	void        Do_SummonFist(const _float& fTimeDelta);
	void        Do_Scream(const _float& fTimeDelta);
	void		Do_ScreamEnd(const _float& fTimeDelta);
	void		Do_ResetRot(const _float& fTimeDelta) { m_pTransform->m_vAngle = _vec3(0, 0, 0); CheckIsLastActionIdx();}

	void		DoFlip();
	void		SetPartten();
	void		ReadyPartten();
	void		CheckIsLastActionIdx();

	//������
	void		Do_Standing(const _float& fTimeDelta);
	//����
	//���� ��ȯ 
	void		Do_SummonRock(const _float& fTimeDelta);
	//����
	//�ո� ������
	void		Do_Throw(const _float& fTimeDelta);
	//�ٽ� ����ġ
	void		Do_ThrowEnd(const _float& fTimeDelta);
private:	
	BOSS2STATE m_eCurrentState;
	BOSS2STATE m_ePreState;
	_int		m_iCurrentActionIdx;
	_float		m_dwRestTime;
	_float		m_dwActionTime;
	_bool		m_bFlip_Y;

	//state, ?�수?�인??vec�?가지??vector
	vector<BOSS2_STATE_FUNC> funcAction;

	//?�프???�동??x �?
	_vec3	   m_fJumpPos[3];
	_int	   m_iJumpPosidx;

	//찍기???�용??player???�치�?받아?�기 ?�함.
	CTransform* m_pPlayer01_trans;
	CTransform* m_pPlayer02_trans;

	_bool		m_bIsOnGround;
	_bool		m_bInit;

private:
	Engine::CCollider*			m_pCollider;
	Engine::CRigidbody*			m_pRigid;
	Engine::CAnimation*			m_pAnimation_Face;
	Engine::CAnimation*			m_pAnimation_Body;
	Engine::CCircleParticle*	m_pCircleParticle;
public:
	static CBoss2*		Create(LPDIRECT3DDEVICE9 pGraphicDev, _vec3& vPos);

private:
	virtual void Free(void) override;
};

