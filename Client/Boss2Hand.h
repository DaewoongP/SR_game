#pragma once
#include "GameObject.h"

BEGIN(Engine)
class CRcTex;
class CTexture;
class CCollider;
class CRigidbody;
END



#define B2H_APPEAR_YPOS 40.0f

#define B2H_DROP_YPOS 25.0f


enum BOSS2HAND_STATE
{
	B2H_APPEAR,
	B2H_READY_LITTLEUP,
	B2H_LITTLEUP,
	B2H_DROP,
	B2H_READY_DISAPPEAR,
	B2H_DISAPPEAR,
	B2H_DIE
};

class CBoss2Hand : public CGameObject
{
private:
	explicit CBoss2Hand(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CBoss2Hand();

public:
	virtual HRESULT Ready_GameObject(_vec3& vPos);
	virtual _int Update_GameObject(const _float& fTimeDelta) override;
	virtual _int Update_Too(const _float & fTimeDelta);
	virtual _int Update_Top(const _float & fTimeDelta);
	virtual void LateUpdate_GameObject(void) override;
	virtual void Render_GameObject(void) override;
	virtual void OnCollisionEnter(const class Collision* collision);
	virtual void OnCollisionStay(const class Collision* collision);
	virtual void OnCollisionExit(const class Collision* collision);
	virtual void SwapTrigger() override;

private:
	HRESULT Add_Component(void);
	void	DoActionSwitch(const _float& fTimeDelta);
	void	D0_Appear(const _float& fTimeDelta);
	void	D0_Ready_LittleUp(const _float& fTimeDelta);
	void	D0_LittleUp(const _float& fTimeDelta);
	void	D0_Drop(const _float& fTimeDelta);
	void	D0_Ready_DisAppear(const _float& fTimeDelta);
	void	D0_DisAppear(const _float& fTimeDelta);
	void	Next_State() { m_state = BOSS2HAND_STATE(m_state + 1); m_dwActionTimer = 0.0f; }
	void	Set_BoundOffSetZ(_float fOffSet)
	{
		m_pCollider->Set_BoundOffset(_vec3(m_pCollider->Get_BoundOffset().x, m_pCollider->Get_BoundOffset().y, fOffSet));
	}

private:
	Engine::CRcTex*			m_pBufferCom;
	Engine::CTexture*		m_pTextureCom;
	Engine::CCollider*		m_pCollider;
	Engine::CRigidbody*		m_pRigid;

private:
	BOSS2HAND_STATE m_state;
	_float			m_dwActionTimer;
	_float			m_fStartY = 0.0f;

public:
	static CBoss2Hand*	Create(LPDIRECT3DDEVICE9 pGraphicDev, _vec3& vPos);

protected:
	virtual void Free(void) override;
};

