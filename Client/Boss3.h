#pragma once
#include "Include.h"
#include "Cube.h"

BEGIN(Engine)
class CRcTex;
class CCircularParticle;
class CTexParticle;
END

class CToodee;
class CTopdee;
class CBoss3Mouth;
class CBoss3Hand;
class CBoss3HandPart;

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

	virtual void Render_GameObject(void) override;
	virtual void OnCollisionEnter(const class Collision* collision);
	virtual void OnCollisionStay(const class Collision* collision);
	virtual void OnCollisionExit(const class Collision* collision);
	virtual void SwapTrigger() override;

	_int	Get_ATKCount() { return m_iATKCount; }
	_int	Get_Boss3Hp() { return m_iHp; }

	_bool Get_Dead() { return m_bDead; }
	void	Set_Lerp() { m_bLerpMove = true; }	
	void	Set_Damaged() { m_bDamaged = true; }
	void	Set_Damage() { --m_iHp; }

private:
	HRESULT Add_Component(void);
	HRESULT	CreateParts();
	void	LookAtPlayer();								// 플레이어 방향으로 몸을 돌림
	void	FollowPlayer(const _float & fTimeDelta);	// 추격 후 내려찍기를 시작할 함수
	void	BossAttack(const _float & fTimeDelta);		// 내려찍는 공격
	void	ShootBullet(const _float & fTimeDelta);		// 총알 쏘는 공격
	void	Do_Scream(const _float& fTimeDelta);
	void	End_Scream(const _float& fTimeDelta);
	void	Lerp_Moving(const _float& fTimeDelta);
	void	Chain_Spark(_float fCoolDown, const _float& fTimeDelta);
	void	DamagedBoss3(const _float& fTimeDelta);
	void	Delay(const _float& fTimeDelta);

private:
	CToodee*		m_pToodee;
	CTopdee*		m_pTopdee;

	CBoss3Mouth*	m_pBoss3Mouth;
	CBoss3Hand*		m_pBossLeftHand;
	CBoss3Hand*		m_pBossRightHand;

	CBoss3HandPart* m_pBoss3LPart;
	CBoss3HandPart* m_pBoss3RPart;
	CBoss3HandPart* m_pBoss3LPart1;
	CBoss3HandPart* m_pBoss3RPart1;
	CBoss3HandPart* m_pBoss3LPart2;
	CBoss3HandPart* m_pBoss3RPart2;
	CBoss3HandPart* m_pBoss3LPart3;
	CBoss3HandPart* m_pBoss3RPart3;

	_bool   m_bInit;			// 한번만 양손 생성 + 표정 생성
	_bool	m_bLerpMove;		// 체력 차감 시 제자리로 돌아오는 여부를 판단함

	_float	m_fTooTime;		// 투디일 때 시간 누적 수치
	_float	m_fTopTime;		// 탑디일 때 시간 누적 수치

	_float	m_fSpeed;			// 이동 속도
	_float	m_fShootterm;		// 투디일 때 전기 공격 시간
	_float  m_fShockDown;		// 탑디일 때 전기 공격 시간
	_float	m_fTimer;			// 플레이어 변환 시 보스 x축 회전 시간값
	_float  m_fLerpDist;		// 체력 차감 시 제자리로 돌아올 때 비율 값
	_float  m_fPreTop;			// 이전 탑디 값을 저장해둠

	_int	m_iATKCount;// 카운트 3이되면 전기공격
	_bool	m_bShoot = true;
	_bool	m_bSpin = true;
	_float	m_fSoundCool;
	_bool	m_bSound;

	_bool	m_bDamaged;
	_float	m_fDamagedTime;

	_bool	m_bDelay;
	_float  m_fDelay;

	Engine::CShadow* m_pShadowCom;
	Engine::CCircularParticle*	m_pLandingParticle;
	Engine::CTexParticle*		m_pScreamParticle;

public:
	static CBoss3*	Create(LPDIRECT3DDEVICE9 pGraphicDev, _vec3& vPos);

protected:
	virtual void Free(void) override;
};