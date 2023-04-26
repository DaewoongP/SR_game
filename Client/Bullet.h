#pragma once
#include "Include.h"
#include "GameObject.h"

BEGIN(Engine)

class CRcTex;
class CTexture;
class CCollider;

END

class CBullet : public CGameObject
{
private:
	explicit CBullet(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CBullet();

public:
	HRESULT Ready_GameObject(_vec3& vPos, _vec3& vDir);
	virtual _int Update_GameObject(const _float& fTimeDelta) override;
	virtual void LateUpdate_GameObject(void) override;
	virtual void Render_GameObject(void) override;
	virtual void OnCollisionEnter(const class Collision* collision);

protected:
	HRESULT Add_Component(void);

protected:
	Engine::CRcTex*			m_pBuf;
	Engine::CTexture*		m_pTex;

	_vec3	m_vDir;
	_float	m_fSpeed;

public:
	static CBullet*	Create(LPDIRECT3DDEVICE9 pGraphicDev, _vec3& vPos, _vec3& vDir);

protected:
	virtual void Free(void) override;
};

