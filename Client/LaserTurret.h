#pragma once

#include "Include.h"
#include "GameObject.h"

BEGIN(Engine)

class CRcTex;
class CTexture;
class CShadow;
class CBulletPool;

END

class CLaserTurret : public Engine::CGameObject
{
private:
	explicit CLaserTurret(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CLaserTurret();

public:
	virtual HRESULT Ready_GameObject(_vec3& vPos, _int iIndex);
	virtual _int Update_GameObject(const _float& fTimeDelta) override;
	virtual void LateUpdate_GameObject(void) override;
	virtual void Render_GameObject(void) override;

private:
	HRESULT		Add_Component(void);

	void		Shoot_Laser(const _float& fTimeDelta);

private:
	Engine::CRcTex* m_pBufferCom;
	Engine::CTexture* m_pTextureCom;
	Engine::CShadow*		m_pShadow;
	Engine::CBulletPool*	m_pBulletPool;
	
	_int		m_iIndex;
	_float		m_fCoolDown;

public:
	static CLaserTurret* Create(LPDIRECT3DDEVICE9 pGraphicDev, _vec3& vPos, _int iIndex);

private:
	virtual void Free(void) override;
};

