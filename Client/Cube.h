#pragma once
#include "Include.h"
#include "GameObject.h"

BEGIN(Engine)

class CCubeTex;
class CTexture;
class CCollider;

END

class CCube :
	public CGameObject
{
protected:
	explicit CCube(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CCube();

public:
	virtual HRESULT Ready_GameObject(void) override;
	virtual _int Update_GameObject(const _float& fTimeDelta) override;
	virtual void LateUpdate_GameObject(void) override;
	virtual void Render_GameObject(void) override;
	virtual void OnCollisionEnter(const class Collision* collision);
	virtual void OnCollisionStay(const class Collision* collision);
	virtual void OnCollisionExit(const class Collision* collision);

protected:
	HRESULT Add_Component(void);

protected:
	Engine::CCubeTex*	m_pBufferCom;
	Engine::CTexture*	m_pTextureCom;
	Engine::CCollider * m_pCollider;

public:
	static CCube*	Create(LPDIRECT3DDEVICE9 pGraphicDev);

protected:
	virtual void Free(void) override;
};

