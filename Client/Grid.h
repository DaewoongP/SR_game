#pragma once

#include "Include.h"
#include "GameObject.h"

BEGIN(Engine)

class CRcTex;
class CTexture;

END

class CGrid : public Engine::CGameObject
{
protected:
	explicit CGrid(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CGrid();

public:
	virtual HRESULT Ready_GameObject(_vec3& vPos)  PURE;
	virtual _int Update_GameObject(const _float& fTimeDelta) PURE;
	virtual void LateUpdate_GameObject(void)  PURE;
	virtual void Render_GameObject(void) PURE;

protected:
	virtual HRESULT		Add_Component(void) PURE;

protected:
	Engine::CRcTex* m_pBufferCom;
	Engine::CTexture* m_pTextureCom;
	bool m_bGridON;

//public:
//	static CGrid*		Create(LPDIRECT3DDEVICE9 pGraphicDev);

protected:
	virtual void Free(void)  PURE;
};