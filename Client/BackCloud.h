#pragma once

#include "Include.h"
#include "BackGroundBase.h"

BEGIN(Engine)
class CRcTex;
class CTexture;
class CTransform;
END

class CBackCloud :
	public CBackGroundBase
{
private:
	explicit CBackCloud(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CBackCloud();

public:
	virtual HRESULT Ready_GameObject(_vec3& vPos) override;
	virtual _int Update_GameObject(const _float& fTimeDelta) override;
	virtual void Render_GameObject(void) override;

	HRESULT Add_Component(void);

private:
	Engine::CRcTex* m_pBufferCom;
	Engine::CTexture* m_pTextureCom;
public:
	static CBackCloud* Create(LPDIRECT3DDEVICE9 pGraphicDev, _vec3& vPos);

private:
	virtual void Free(void);
};

