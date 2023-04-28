#pragma once
#include "GameObject.h"
BEGIN(Engine)

class CRcTex;
class CTexture;

END

class CFinalPortal : public CGameObject
{
private:
	explicit CFinalPortal(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CFinalPortal();

public:
	virtual HRESULT Ready_GameObject(_vec3& vPos) override;
	virtual _int Update_GameObject(const _float& fTimeDelta) override;
	virtual void Render_GameObject(void) override;

private:
	HRESULT		Add_Component(void);

	void		PreRender();
	void		PostRender();

private:
	Engine::CRcTex*		m_pBufferCom;
	Engine::CTexture*	m_pPortal;

public:
	static CFinalPortal*		Create(LPDIRECT3DDEVICE9 pGraphicDev, _vec3 vPos , CLayer* pLayer);

private:
	virtual void Free(void) override;
};
