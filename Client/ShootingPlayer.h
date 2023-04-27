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

private:
	HRESULT		Add_Component(void);
	void		Key_Input(const _float& fTimeDelta);
	void		Default_Bullet(const _float& fTimeDelta);
	void		Quad_Bullet(const _float& fTimeDelta);
	void		Sword_Bullet(const _float& fTimeDelta);
	void		Fire_Bullet(const _float& fTimeDelta);
	void		Rot_Player();
	void		Switch_Bullet(const _float & fTimeDelta);

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

	_int						m_iBulletIndex;

public:
	static CShootingPlayer*		Create(LPDIRECT3DDEVICE9 pGraphicDev, _vec3& vPos);

protected:
	virtual void Free(void) override;
};

