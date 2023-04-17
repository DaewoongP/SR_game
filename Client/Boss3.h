#pragma once
#include "Include.h"
#include "Cube.h"

BEGIN(Engine)

class CRcTex;
class CCircularParticle;
END

class CBoss3 : public CCube
{
private:
	explicit CBoss3(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CBoss3();

public:
	virtual HRESULT Ready_GameObject(_vec3& vPos) override;
	virtual _int Update_Too(const _float & fTimeDelta);
	virtual _int Update_Top(const _float & fTimeDelta);
	virtual _int Update_GameObject(const _float& fTimeDelta) override;
	virtual void LateUpdate_GameObject(void) override;
	virtual void LateUpdate_Top() override;

	virtual void Render_GameObject(void) override;
	virtual void OnCollisionEnter(const class Collision* collision);
	virtual void OnCollisionStay(const class Collision* collision);
	virtual void OnCollisionExit(const class Collision* collision);
	virtual void SwapTrigger() override;

	void MakeChain();

	_int Get_ATKCount() { return m_iATKCount; }


private:
	HRESULT Add_Component(void);
	void	LookAtPlayer();								// 플레이어 방향으로 몸을 돌림
	void	FollowPlayer(const _float & fTimeDelta);	// 추격 후 내려찍기를 시작할 함수
	void	BossAttack(const _float & fTimeDelta);		// 내려찍는 공격
	void	ShootBullet(const _float & fTimeDelta);		// 총알 쏘는 공격

private:
	_bool   m_bCreateHand;			// 한번만 양손 생성 + 표정 생성
	CGameObject* m_pBossLeft;		// 왼손 주소
	CGameObject* m_pBossRight;		// 오른손 주소
	CGameObject* m_pBossLeftPart;
	CGameObject* m_pBossRightPart;
	CGameObject* m_pBossLeftPart1;
	CGameObject* m_pBossRightPart1;



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
	_float	m_fShootterm;

	_float	m_fPreToo;			// 정규화된 이전 투디 값을 저장해둠
	_float  m_fPreTop;			// 이전 탑디 값을 저장해둠

	_float  m_fSaveToo;
	_float  m_fSaveTop;

	_bool m_bATKEnd=false;
	_int m_iBossHp = 1; //추후에 스테이지로 옮겨야함
	_bool m_bATKCnt; //전기공격을할지안할지판단
	_int m_iATKCount;// 카운트 3이되면 전기공격
	_bool m_bShoot = true;


	_vec3 m_vPrePos;

	Engine::CShadow* m_pShadowCom;
	Engine::CCircularParticle*	m_pLandingParticle;

public:
	static CBoss3*	Create(LPDIRECT3DDEVICE9 pGraphicDev, _vec3& vPos);

protected:
	virtual void Free(void) override;
};

#define BOSS3_CHASE 1.5f
#define BOSS3_SPIN 0.5f