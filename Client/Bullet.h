#pragma once
#include "Include.h"
#include "GameObject.h"

BEGIN(Engine)

class CRcTex;
class CCollider;
class CTexture;
END
enum BULLETTYPE
{
	DEFAULT,
	SWORD,
	FIRE,
	LASER,
	BULLET_END
};
class CBullet : public CGameObject
{
protected:
	explicit CBullet(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CBullet();

public:
	virtual HRESULT Ready_Bullet(_vec3& vPos, _vec3& vDir = _vec3(0.f, 1.f, 0.f));
	virtual void	Ready_Pool(_vec3& vPos, _vec3& vDir = _vec3(0.f, 1.f, 0.f));
	virtual _int Update_GameObject(const _float& fTimeDelta) override;
	virtual void LateUpdate_GameObject(void) override;

protected:
	HRESULT Add_Component(void);

protected:
	Engine::CRcTex*			m_pBuf;
	Engine::CTexture*		m_pTex;
	Engine::CCollider*		m_pCollider;
	_vec3	m_vDir;
	_float	m_fSpeed;

protected:
	virtual void Free(void) override;
};

