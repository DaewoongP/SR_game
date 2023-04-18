#pragma once

#include "Include.h"
#include "GameObject.h"

BEGIN(Engine)

class CRcTex;
class CTexture;
class CCollider;
class CShadow;
class CCircleParticle;

END

class CLaser : public Engine::CGameObject
{
	typedef enum
	{
		LASER_LEFT,
		LASER_RIGHT,
		LASER_DIR_END
	}LASERDIR;

private:
	explicit CLaser(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CLaser();

public:
	_bool	Get_Use() { return m_bUse; }
	void	Set_Use(_bool tf) { m_bUse = tf; }

public:
	virtual HRESULT Ready_GameObject(_vec3& vPos, _int eDir);
	virtual _int Update_GameObject(const _float& fTimeDelta) override;
	virtual void LateUpdate_GameObject(void) override;
	virtual void Render_GameObject(void) override;
	virtual void OnCollisionEnter(const Collision* collision);

private:
	HRESULT		Add_Component(void);

private:
	Engine::CRcTex* m_pBufferCom;
	Engine::CTexture* m_pTextureCom;
	Engine::CCollider* m_pCollider;
	Engine::CShadow*		m_pShadow;
	Engine::CCircleParticle*	m_pColParticle;

	LASERDIR m_eLaserDir;
	_float	 m_fSpeed;
	_bool	 m_bUse;

public:
	static CLaser* Create(LPDIRECT3DDEVICE9 pGraphicDev, _vec3& vPos, _int eDir);

private:
	virtual void Free(void) override;
};

