#pragma once
#include "Include.h"
#include "GameObject.h"

BEGIN(Engine)

class CCubeTex;
class CTexture;
class CRigidbody;
class CCollider;

END

class CCube :
	public CGameObject
{
private:
	explicit CCube(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CCube();

public:
	virtual HRESULT Ready_GameObject(void) override;
	virtual _int Update_GameObject(const _float& fTimeDelta) override;
	virtual void LateUpdate_GameObject(void) override;
	virtual void Render_GameObject(void) override;

private:
	HRESULT Add_Component(void);

private:
	Engine::CCubeTex*	m_pBufferCom;
	Engine::CTexture*	m_pTextureCom;
	Engine::CRigidbody*	m_pRigid;
	Engine::CCollider * m_pCollider;

public:
	static CCube*	Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	virtual void Free(void) override;
};

