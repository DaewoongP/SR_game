#pragma once

#include "Include.h"
#include "GameObject.h"

BEGIN(Engine)

class CRcTex;
class CTexture;
class CShadow;
class CCollider;
class CLine;
class CCircleParticle;

END

class CLaserTurret : public Engine::CGameObject
{
public:
	enum Laser_Dir
	{
		LASER_UP,
		LASER_RIGHT,
		LASER_LEFT,
		LASER_END
	};

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
	Engine::CRcTex*				m_pBufferCom;
	Engine::CTexture*			m_pTextureCom;
	Engine::CShadow*			m_pShadow;
	Engine::CCollider*			m_pCollider;
	Engine::CLine*				m_pRedLine;
	Engine::CLine*				m_pWhiteLine;
	Engine::CCircleParticle*	m_pColParticle;
	
	_int		m_iIndex;
	_float		m_fCoolDown;
	_float		m_fColdist;

	_vec3		m_vPos;
	_vec3		m_vEnd;

	CGameObject*	m_pBoss3;

public:
	static CLaserTurret* Create(LPDIRECT3DDEVICE9 pGraphicDev, _vec3& vPos, _int iIndex);

private:
	virtual void Free(void) override;
};

