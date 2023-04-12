#pragma once
#include "Include.h"
#include "GameObject.h"

#define BOSS2_STATE_FUNC vector<void(CBoss2::*)(const _float& fTimeDelta)>

BEGIN(Engine)
class CRcTex;
class CTexture;
class CCollider;
class CRigidbody;
END

enum BOSS2STATE
{
	B2_IDLE,
	B2_JUMPING,
	B2_SCREAM,
	B2_PUNCH,
	B2_STUMP,
	B2_END
};

//보스의 머리통임. 모든 생각/행동의 중추를 담당함.
//보스는 중력을 쓰지 않을거임. 보니까 중력 안씀.
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
	virtual void Render_GameObject(void) override;
	virtual void Render_Too(void) override;
	virtual void Render_Top(void) override;
	virtual void SwapTrigger();

	virtual void OnCollisionEnter(const class Collision* collision);
	virtual void OnCollisionStay(const class Collision* collision);
	virtual void OnCollisionExit(const class Collision* collision);

private:
	HRESULT		Add_Component(void);
	HRESULT		Find_PlayerBoth();
	//점프 함수 기본값 설정
	void		Do_Jump_Ready(const _float& fTimeDelta);
	//위로 상승한다
	void		Do_Jump_01(const _float& fTimeDelta);
	//착지한다.
	void		Do_Jump_02(const _float& fTimeDelta);
	//휴식패턴
	void		Do_Rest(const _float& fTimeDelta);
	//벨로시티 초기화 패턴
	void		Do_ResetVelocity(const _float& fTimeDelta) {m_pRigid->m_Velocity = _vec3(0, 0, 0);CheckIsLastActionIdx();}
	
	//스텀프 함수 기본값 설정
	void		Do_Stump_Ready(const _float& fTimeDelta);
	//toodee 혹은 topdee를 향해 n초간 가있는 패턴
	void		Do_Chase_Player(const _float& fTimeDelta);
	//y로 살짝 올라가며, 회전을 주는 패턴
	void		Do_LittleUp_Turn(const _float& fTimeDelta);
	//만세!
	void        Do_Hurray(const _float& fTimeDelta);
	//주먹 소환
	void        Do_SummonFist(const _float& fTimeDelta);

	void		SetPartten();
	void		ReadyPartten();
	void		CheckIsLastActionIdx();

private:
	BOSS2STATE m_eCurrentState;
	BOSS2STATE m_ePreState;
	_int		m_iCurrentActionIdx;
	_float		m_dwRestTime;
	_float		m_dwActionTime;

	//state, 함수포인터 vec를 가지는 vector
	vector<BOSS2_STATE_FUNC> funcAction;

	//점프시 이동할 x 값
	_vec3	   m_fJumpPos[3];
	_int	   m_iJumpPosidx;

	//찍기때 사용할 player의 위치를 받아오기 위함.
	CTransform* m_pPlayer01_trans;
	CTransform* m_pPlayer02_trans;

	_bool		m_bInit;

private:
	Engine::CRcTex*			m_pBufferCom;
	Engine::CTexture*		m_pTextureCom;
	Engine::CCollider*		m_pCollider;
	Engine::CRigidbody*		m_pRigid;

public:
	static CBoss2*		Create(LPDIRECT3DDEVICE9 pGraphicDev, _vec3& vPos);

private:
	virtual void Free(void) override;
};

