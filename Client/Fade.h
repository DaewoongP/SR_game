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
	virtual HRESULT Ready_GameObject(void) override;
	virtual _int Update_GameObject(const _float& fTimeDelta) override;
	virtual void Render_GameObject(void) override;

	_bool	Get_Restart() { return m_bReStart; }
private:
	HRESULT		Add_Component(void);

private:
	Engine::CRcTex*		m_pBufferCom;
	Engine::CTexture*	m_pFadeTex;

	_float				m_fFadeSpeed;

	_bool				m_bReStart;
public:
	static CFade*		Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	virtual void Free(void) override;
};
