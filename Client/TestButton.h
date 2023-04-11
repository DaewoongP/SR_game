#pragma once
#include "Include.h"
#include "GameObject.h"

BEGIN(Engine)
class CRcTex;
class CTexture;
END

class CTestButton :
	public CGameObject
{
private:
	explicit CTestButton(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CTestButton();
public:
	virtual HRESULT Ready_GameObject(void) override;
	virtual _int Update_GameObject(const _float & fTimeDelta) override;
	virtual void Render_GameObject(void) override;
private:
	HRESULT Add_Component(void);

	Engine::CRcTex*			m_pBufferCom;
	Engine::CTexture*		m_pTextureCom;
public:
	static CTestButton*	Create(LPDIRECT3DDEVICE9 pGraphicDev);
private:
	virtual void Free(void);
};

