#pragma once

#include "Include.h"
#include "GameObject.h"

BEGIN(Engine)

class CRcTex;
class CTexture;
class CShadow;

END

class CSwallowPortal : public Engine::CGameObject
{
private:
	explicit CSwallowPortal(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CSwallowPortal();

public:
	virtual HRESULT Ready_GameObject(_vec3& vPos) override;
	virtual _int Update_GameObject(const _float& fTimeDelta) override;
	virtual void LateUpdate_GameObject(void) override;
	virtual void Render_GameObject(void) override;

private:
	HRESULT		Add_Component(void);

private:
	Engine::CRcTex*    m_pBufferCom;
	Engine::CTexture*  m_pTextureCom;
	Engine::CShadow*		m_pShadow;

public:
	static CSwallowPortal* Create(LPDIRECT3DDEVICE9 pGraphicDev, _vec3& vPos);

private:
	virtual void Free(void) override;
};