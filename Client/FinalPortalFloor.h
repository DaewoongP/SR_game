#pragma once
#include "GameObject.h"
BEGIN(Engine)

class CRcTex;
class CTexture;

END

class CFinalPortalFloor : public CGameObject
{
private:
	explicit CFinalPortalFloor(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CFinalPortalFloor();

public:
	virtual HRESULT Ready_GameObject(_vec3& vPos) override;
	virtual _int Update_GameObject(const _float& fTimeDelta) override;
	virtual void Render_GameObject(void) override;

private:
	HRESULT		Add_Component(void);

private:
	Engine::CRcTex*		m_pBufferCom;
	Engine::CTexture*	m_pPortal;

public:
	static CFinalPortalFloor*		Create(LPDIRECT3DDEVICE9 pGraphicDev, _vec3 vPos);

private:
	virtual void Free(void) override;
};
