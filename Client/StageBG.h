#pragma once

#include "Include.h"
#include "GameObject.h"

BEGIN(Engine)

class CRcTex;
class CTexture;

END
class CStageBG : public Engine::CGameObject
{
private:
	explicit CStageBG(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CStageBG();

public:
	virtual HRESULT Ready_GameObject(void) override;
	virtual _int Update_GameObject(const _float& fTimeDelta) override;
	virtual void LateUpdate_GameObject(void) override;
	virtual void Render_GameObject(void) override;

private:
	HRESULT		Add_Component(void);

private:
	Engine::CRcTex*			m_pBufferCom;
	Engine::CTexture*		m_pTextureCom;


public:
	static CStageBG*		Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	virtual void Free(void) override;

};

