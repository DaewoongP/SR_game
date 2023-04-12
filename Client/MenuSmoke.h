#pragma once

#include "Include.h"
#include "GameObject.h"

BEGIN(Engine)
class CRcAlpha;
class CTexture;
END

class CMenuSmoke :
	public CGameObject
{
private:
	explicit CMenuSmoke(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CMenuSmoke();

public:
	virtual HRESULT Ready_GameObject(_vec3& vPos) override;
	virtual _int Update_GameObject(const _float& fTimeDelta) override;
	virtual void Render_GameObject(void) override;

	HRESULT Add_Component(void);

private:
	Engine::CRcAlpha*		m_pBufferCom;
	Engine::CTexture*		m_pTextureCom;

public:
	static CMenuSmoke*	Create(LPDIRECT3DDEVICE9 pGraphicDev, _vec3& vPos);

private:
	virtual void Free(void);
};

