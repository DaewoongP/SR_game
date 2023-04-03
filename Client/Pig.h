#pragma once
#include "Monster.h"
class CPig :
	public CMonster
{
private:
	explicit CPig(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CPig();

public:
	virtual HRESULT Ready_GameObject(void) override;
	virtual _int Update_GameObject(const _float& fTimeDelta) override;
	virtual void LateUpdate_GameObject(void) override;
	virtual void Render_GameObject(void) override;

	virtual void OnCollisionEnter(const class Collision* collision);
	virtual void OnCollisionStay(const class Collision* collision);
	virtual void OnCollisionExit(const class Collision* collision);

	virtual _int Updatae_2D(const _float& fTimeDelta);
	virtual _int Updatae_3D(const _float& fTimeDelta);

private:
	virtual HRESULT		Add_Component(void);

private:
	Engine::CRcTex*			m_pBufferCom;
	Engine::CTexture*		m_pTextureCom;
	Engine::CCollider*		m_pCollider;
	Engine::CRigidbody*		m_pRigid;

	_float					m_fSpeed = 5.f;

public:
	static CPig*		Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	virtual void Free(void) override;
};

