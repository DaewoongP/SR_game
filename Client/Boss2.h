#pragma once
#include "Include.h"
#include "GameObject.h"

BEGIN(Engine)
class CRcTex;
class CTexture;
class CCollider;
class CRigidbody;
END

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

