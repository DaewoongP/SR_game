#pragma once
#include "GameObject.h"

BEGIN(Engine)
class CLayer;
class CRcTex;
class CTexture;
class CCollider;
END
class CShootingPlayer :
	public CGameObject
{
private:
	enum SHOOTPOS
	{
		INIT,
		LEFT,
		RIGHT,
		POS_END
	};

private:
	explicit CShootingPlayer(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CShootingPlayer();

public:
	virtual HRESULT Ready_GameObject(_vec3& vPos) override;
	virtual _int Update_GameObject(const _float& fTimeDelta) override;
	virtual void LateUpdate_GameObject(void) override;
	virtual void Render_GameObject(void) override;

	void	Set_Bullet(_int iIndex) { m_iBulletIndex = iIndex; }
	void	Set_Shoot(_bool value) { m_bPossibleShoot = value; }
	void	Set_Pos_Rot(_vec3 pos, _vec3 rot) { m_OriginPos = pos; m_OriginRot = rot; }
	_vec3	Get_OriginPos() {	return m_OriginPos;}
	_vec3	Get_OriginRot() { return m_OriginRot; }
private:						
	HRESULT		Add_Component(void);
	void		Key_Input(const _float& fTimeDelta);
	void		Shoot_Bullet(const _float & fTimeDelta);
	void		Default_Bullet(const _float& fTimeDelta);
	void		Quad_Bullet(const _float& fTimeDelta);
	void		Sword_Bullet(const _float& fTimeDelta);
	void		Fire_Bullet(const _float& fTimeDelta);
	void		Laser(const _float& fTimeDelta);
	void		Rot_Player();

private:
	Engine::CRcTex*				m_pBuf;
	Engine::CTexture*			m_pTex;
	Engine::CCollider*			m_pCollider;

	_float						m_fSlerp;
	_vec3						m_vPos[POS_END];
	_vec3						m_vPrePos;

	_bool						m_bLKey;
	_bool						m_bRKey;
	CLayer*						m_pGameLogicLayer;
	_bool						m_bLaser=true;
	_float						m_fLaserTime = 0.f;

	_bool						m_bPossibleShoot;
	_vec3						m_OriginPos;
	_vec3						m_OriginRot;

	_int						m_iBulletIndex;

public:
	static CShootingPlayer*		Create(LPDIRECT3DDEVICE9 pGraphicDev, _vec3& vPos);

protected:
	virtual void Free(void) override;
};

