#pragma once
#include "GameObject.h"

BEGIN(Engine)
class CLayer;
class CRcTex;
class CTexture;

END
class CShootingPlayer :
	public CGameObject
{
private:
	explicit CShootingPlayer(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CShootingPlayer();

public:
	virtual HRESULT Ready_GameObject(_vec3& vPos) override;
	virtual _int Update_GameObject(const _float& fTimeDelta) override;
	virtual void LateUpdate_GameObject(void) override;
	virtual void Render_GameObject(void) override;

	virtual void OnCollisionEnter(const class Collision* collision);
	virtual void OnCollisionStay(const class Collision* collision);
	virtual void OnCollisionExit(const class Collision* collision);

	void		Key_Input(const _float& fTimeDelta);
private:
	HRESULT		Add_Component(void);
	void		Fire_bullet();
private:
	Engine::CRcTex*				m_pBuf;
	Engine::CTexture*			m_pTex;

	CLayer*						m_pGameLogicLayer;
public:
	static CShootingPlayer*		Create(LPDIRECT3DDEVICE9 pGraphicDev, _vec3& vPos);

protected:
	virtual void Free(void) override;
};

