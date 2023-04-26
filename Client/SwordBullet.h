#pragma once
#include "Include.h"
#include "GameObject.h"

BEGIN(Engine)

class CRcTex;
class CTexture;
class CCollider;

END

class CSwordBullet : public CGameObject
{
private:
	explicit CSwordBullet(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CSwordBullet();

public:
	HRESULT Ready_GameObject(_vec3& vPos, _vec3& vInitPos);
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

	_float	m_fSpeed;
	_bool	m_bShoot;
public:
	static CSwordBullet*	Create(LPDIRECT3DDEVICE9 pGraphicDev, _vec3& vPos, _vec3& vInitPos);

protected:
	virtual void Free(void) override;
};

