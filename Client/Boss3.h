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
		B3_IDLE,		// ����
		B3_ATTACK,		// ���� ��� ����
		B3_SHOOT,		// �Ѿ� ��� ����
		B3_DEAD			// ���
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

	void	FollowPlayer(const _float & fTimeDelta);	// �߰� �� ������⸦ ������ �Լ�
	void	BossAttack(const _float & fTimeDelta);		// ������� ����

	void	ShootBullet(const _float & fTimeDelta);		// �Ѿ� ��� ����

private:
	_bool   m_bCreateHand;			// �ѹ��� ��� ���� + ǥ�� ����
	CGameObject* m_pBossLeft;		// �޼� �ּ�
	CGameObject* m_pBossRight;		// ������ �ּ�
	CGameObject* m_pBossLeftEye;	// �޴� �ּ�	
	CGameObject* m_pBossRightEye;	// ������ �ּ�
	CGameObject* m_pBossLeftPupil;	// �޴����� �ּ�	
	CGameObject* m_pBossRightPupil;	// ���������� �ּ�
	CGameObject* m_pLeftEyebrow;	// �� ���� �ּ�
	CGameObject* m_pRightEyebrow;	// ���� ���� �ּ�
	
	_float  m_fXAngle;			// �÷��̾� ��ȯ �� x�� ȸ�� ��
	_float	m_fSpeed;			// �̵� �ӵ�
	_float  m_fAttackCoolDown;	// ���� ��ٿ�
	_float  m_fCoolDown;		// �߰� ��ٿ�
	_float  m_fShootCoolDown;	// ��� ��ٿ�

	BOSS3	m_eCurState;		
	BOSS3	m_ePreState;

public:
	static CBoss3*	Create(LPDIRECT3DDEVICE9 pGraphicDev, _vec3& vPos);

protected:
	virtual void Free(void) override;

};

#define BOSS3_CHASE 1.5f
#define BOSS3_SPIN 0.5f