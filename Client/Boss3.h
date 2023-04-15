#pragma once
#include "Include.h"
#include "Cube.h"

BEGIN(Engine)

class CRcTex;

END

class CBoss3 : public CCube
{
	typedef enum Boss3State
	{
		B3_IDLE,		// 평상시
		B3_ATTACK,		// 내려 찍기 공격
		B3_SHOOT,		// 총알 쏘는 공격
		B3_DEAD			// 사망
	}BOSS3;

private:
	explicit CBoss3(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CBoss3();

public:
	virtual HRESULT Ready_GameObject(_vec3& vPos) override;
	virtual _int Update_Too(const _float & fTimeDelta);
	virtual _int Update_Top(const _float & fTimeDelta);
	virtual _int Update_GameObject(const _float& fTimeDelta) override;
	virtual void LateUpdate_GameObject(void) override;
	virtual void Render_GameObject(void) override;
	virtual void OnCollisionEnter(const class Collision* collision);
	virtual void OnCollisionStay(const class Collision* collision);
	virtual void OnCollisionExit(const class Collision* collision);
	virtual void SwapTrigger() override;

private:
	HRESULT Add_Component(void);
	void	State_Change(const _float & fTimeDelta);
	void	LookAtPlayer();								// 플레이어 방향으로 몸을 돌림

	void	FollowPlayer(const _float & fTimeDelta);	// 추격 후 내려찍기를 시작할 함수
	void	BossAttack(const _float & fTimeDelta);		// 내려찍는 공격

	void	ShootBullet(const _float & fTimeDelta);		// 총알 쏘는 공격

private:
	_bool   m_bCreateHand;			// 한번만 양손 생성 + 표정 생성
	CGameObject* m_pBossLeft;		// 왼손 주소
	CGameObject* m_pBossRight;		// 오른손 주소
	CGameObject* m_pBossLeftEye;	// 왼눈 주소	
	CGameObject* m_pBossRightEye;	// 오른눈 주소
	CGameObject* m_pBossLeftPupil;	// 왼눈동자 주소	
	CGameObject* m_pBossRightPupil;	// 오른눈동자 주소
	CGameObject* m_pLeftEyebrow;	// 왼 눈썹 주소
	CGameObject* m_pRightEyebrow;	// 오른 눈썹 주소
	CGameObject* m_pMouth;			// 보스3 입
	
	_float  m_fXAngle;			// 플레이어 전환 시 x축 회전 값
	_float	m_fSpeed;			// 이동 속도
	_float  m_fAttackCoolDown;	// 공격 쿨다운
	_float  m_fCoolDown;		// 추격 쿨다운
	_float  m_fShootCoolDown;	// 사격 쿨다운
	_float	m_fPreToo;			// 정규화된 이전 투디 값을 저장해둠
	_float  m_fPreTop;			// 이전 탑디 값을 저장해둠

	BOSS3	m_eCurState;		
	BOSS3	m_ePreState;

public:
	static CBoss3*	Create(LPDIRECT3DDEVICE9 pGraphicDev, _vec3& vPos);

protected:
	virtual void Free(void) override;
};

#define BOSS3_CHASE 1.5f
#define BOSS3_SPIN 0.5f