#pragma once
#include "Include.h"
#include "Cube.h"

BEGIN(Engine)

class CRcTex;
class CCircularParticle;
class CTexParticle;
class CShadow;

END

class JJapBoss3 : public CCube
{
private:
	explicit JJapBoss3(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~JJapBoss3();

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

	void MakeChain();

	_int Get_ATKCount() { return m_iATKCount; }

	// �׽�Ʈ�뵵
public:
	_int	Get_Boss3Hp() { return m_iBossHp; }
	void	Set_Damage() { --m_iBossHp; }

private:
	HRESULT Add_Component(void);
	void	LookAtPlayer();								// �÷��̾� �������� ���� ����
	void	FollowPlayer(const _float & fTimeDelta);	// �߰� �� ������⸦ ������ �Լ�
	void	BossAttack(const _float & fTimeDelta);		// ������� ����
	void	ShootBullet(const _float & fTimeDelta);		// �Ѿ� ��� ����
	void	Do_Scream(const _float& fTimeDelta);
	void	End_Scream(const _float& fTimeDelta);

private:
	_bool   m_bCreateHand;			// �ѹ��� ��� ���� + ǥ�� ����
	CGameObject* m_pBossLeft;		// �޼� �ּ�
	CGameObject* m_pBossRight;		// ������ �ּ�
	CGameObject* m_pBossLeftPart;
	CGameObject* m_pBossRightPart;
	CGameObject* m_pBossLeftPart1;
	CGameObject* m_pBossRightPart1;
	CGameObject* m_pMouth;			// ����3 ��

	_float  m_fXAngle;			// �÷��̾� ��ȯ �� x�� ȸ�� ��
	_float	m_fSpeed;			// �̵� �ӵ�
	_float  m_fAttackCoolDown;	// ���� ��ٿ�
	_float  m_fCoolDown;		// �߰� ��ٿ�
	_float  m_fShootCoolDown;	// ��� ��ٿ�
	_float	m_fShootterm;
	_float  m_fShockDown;

	_float	m_fPreToo;			// ����ȭ�� ���� ���� ���� �����ص�
	_float  m_fPreTop;			// ���� ž�� ���� �����ص�

	_bool m_bATKEnd = false;
	_int m_iBossHp; //���Ŀ� ���������� �Űܾ���
	_bool m_bATKCnt; //��������������������Ǵ�
	_int m_iATKCount;// ī��Ʈ 3�̵Ǹ� �������
	_bool m_bShoot = true;
	_bool	m_bSpin = true;
	_float m_fSoundCool;
	_bool m_bSound;

	_float	m_fTimer;

	Engine::CShadow* m_pShadowCom;
	Engine::CCircularParticle*	m_pLandingParticle;
	Engine::CTexParticle*		m_pScreamParticle;

public:
	static JJapBoss3*	Create(LPDIRECT3DDEVICE9 pGraphicDev, _vec3& vPos);

protected:
	virtual void Free(void) override;
};

#define BOSS3_CHASE 1.5f
#define BOSS3_SPIN 0.5f