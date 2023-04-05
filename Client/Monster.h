#pragma once

#include "Include.h"
#include "GameObject.h"

BEGIN(Engine)
class CRcTex;
class CTexture;
class CCollider;
class CRigidbody;
END
class CMonster : public CGameObject
{
protected:
	explicit CMonster(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CMonster();

public:
	virtual HRESULT Ready_GameObject(void) PURE;
	virtual _int Update_GameObject(const _float& fTimeDelta) override;
	virtual void LateUpdate_GameObject(void) override;
	virtual void Render_GameObject(void) override;

	virtual void OnCollisionEnter(const Collision* collision);
	virtual void OnCollisionStay(const Collision* collision);
	virtual void OnCollisionExit(const Collision* collision);

protected:
	Engine::CRcTex*			m_pBufferCom;
	Engine::CTexture*		m_pTextureCom;
	Engine::CCollider*		m_pCollider;
	Engine::CRigidbody*		m_pRigid;
	_float					m_fSpeed;
protected:
	virtual void Free(void);
};