#pragma once

#include "Include.h"
#include "GameObject.h"

BEGIN(Engine)

class CRcTex;

END

class CBackgroundSpr :
	public CGameObject
{
private:
	explicit CBackgroundSpr(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CBackgroundSpr();

public:
	virtual HRESULT Ready_GameObject(void) override;
	virtual _int Update_GameObject(const _float& fTimeDelta) override;
	virtual void Render_GameObject(void) override;

private:
	HRESULT		Add_Component(void);

private:
	Engine::CRcGradation*	m_pBufferCom;
public:
	static CBackgroundSpr*		Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	virtual void Free(void) override;
};

