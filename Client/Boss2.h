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

//������ �Ӹ�����. ��� ����/�ൿ�� ���߸� �����.
//������ �߷��� ���� ��������. ���ϱ� �߷� �Ⱦ�.
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
	//���� �Լ� �⺻�� ����
	void		Do_Jump_Ready(const _float& fTimeDelta);
	//���� ����Ѵ�
	void		Do_Jump_01(const _float& fTimeDelta);
	//�����Ѵ�.
	void		Do_Jump_02(const _float& fTimeDelta);
	//�޽�����
	void		Do_Rest(const _float& fTimeDelta);
	//���ν�Ƽ �ʱ�ȭ ����
	void		Do_ResetVelocity(const _float& fTimeDelta) {m_pRigid->m_Velocity = _vec3(0, 0, 0);CheckIsLastActionIdx();}
	
	//������ �Լ� �⺻�� ����
	void		Do_Stump_Ready(const _float& fTimeDelta);
	//toodee Ȥ�� topdee�� ���� n�ʰ� ���ִ� ����
	void		Do_Chase_Player(const _float& fTimeDelta);
	//y�� ��¦ �ö󰡸�, ȸ���� �ִ� ����
	void		Do_LittleUp_Turn(const _float& fTimeDelta);
	//����!
	void        Do_Hurray(const _float& fTimeDelta);
	//�ָ� ��ȯ
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

	//state, �Լ������� vec�� ������ vector
	vector<BOSS2_STATE_FUNC> funcAction;

	//������ �̵��� x ��
	_vec3	   m_fJumpPos[3];
	_int	   m_iJumpPosidx;

	//��⶧ ����� player�� ��ġ�� �޾ƿ��� ����.
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

