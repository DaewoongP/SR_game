#pragma once
#include "GameObject.h"
BEGIN(Engine)

class CRcTex;
class CTexture;

END

class CFade : public CGameObject
{
private:
	explicit CFade(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CFade();

public:
	HRESULT Ready(_bool isFadeIn);
	virtual _int Update_GameObject(const _float& fTimeDelta) override;
	virtual void Render_GameObject(void) override;
private:
	HRESULT		Add_Component(void);

private:
	Engine::CRcTex*		m_pBufferCom;
	Engine::CTexture*	m_pFadeTex;

	_float				m_fFadeSpeed;
public:
	static CFade*		Create(LPDIRECT3DDEVICE9 pGraphicDev, _bool isFadeIn);

private:
	virtual void Free(void) override;
};
