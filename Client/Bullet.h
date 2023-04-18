#pragma once
#include "Include.h"
#include "GameObject.h"

BEGIN(Engine)

class CCubeTex;
class CTexture;
class CCollider;

END

class CBullet : public CGameObject
{
protected:
	explicit CBullet(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CBullet();

public:


public:
	virtual HRESULT Ready_GameObject(_vec3& vPos, _int eDir);
	virtual _int Update_GameObject(const _float& fTimeDelta) override;
	virtual void LateUpdate_GameObject(void) override;
	virtual void Render_GameObject(void) override;
	virtual void OnCollisionEnter(const class Collision* collision);

protected:
	HRESULT Add_Component(void);

protected:
	Engine::CCubeTex*		m_pBufferCom;
	Engine::CTexture*		m_pTextureCom;
	Engine::CCollider*		m_pCollider;

	_bool	m_bUse;
	_float	m_fSpeed;

public:
	static CBullet*	Create(LPDIRECT3DDEVICE9 pGraphicDev, _vec3& vPos, _int eDir);

protected:
	virtual void Free(void) override;
};

