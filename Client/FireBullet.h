#pragma once
#include "Include.h"
#include "GameObject.h"

BEGIN(Engine)

class CRcTex;
class CTexture;
class CCollider;

END

class CFireBullet : public CGameObject
{
private:
	explicit CFireBullet(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CFireBullet();

public:
	virtual HRESULT Ready_GameObject(_vec3& vPos) override;
	virtual _int Update_GameObject(const _float& fTimeDelta) override;
	virtual void LateUpdate_GameObject(void) override;
	virtual void Render_GameObject(void) override;
	virtual void OnCollisionEnter(const class Collision* collision);

protected:
	HRESULT Add_Component(void);

protected:
	Engine::CRcTex*			m_pBuf;
	Engine::CTexture*		m_pTex;
	Engine::CCollider*		m_pCollider;
	_vec3	m_vDir;
	_float	m_fSpeed;

public:
	static CFireBullet*	Create(LPDIRECT3DDEVICE9 pGraphicDev, _vec3& vPos);

protected:
	virtual void Free(void) override;
};

