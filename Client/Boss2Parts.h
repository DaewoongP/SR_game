#pragma once
#include "GameObject.h"
#include "Include.h"

BEGIN(Engine)
class CTexture;
class CShadow;

END


class CBoss2Parts :
	public CGameObject
{
public:
	explicit CBoss2Parts(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CBoss2Parts();

	virtual HRESULT Ready_GameObject(_vec3& vPos) override;
	virtual _int Update_GameObject(const _float& fTimeDelta) override;
	virtual _int Update_Too(const _float& fTimeDelta) override;
	virtual _int Update_Top(const _float& fTimeDelta) override;
	virtual void LateUpdate_GameObject(void) override;
	virtual void Render_GameObject(void) override;
	virtual void Render_Too(void) override;
	virtual void Render_Top(void) override;

public:
	void TextureBlinkStart()
	{
		if (nullptr == m_pTextureCom)
			return;
		m_pTextureCom->m_bUseFrameAnimation = false;
		m_dwTimer = 2.0f; 
	}
	void SetTexture_Blink(_bool isWhite) { if (isWhite) m_CurrentTexture = m_WhiteTexture; else m_CurrentTexture = m_OriginTexture;}
	CTexture*	GetTextureCom() { return m_pTextureCom; }
protected:
	_float	m_dwTimer;
	_int	m_OriginTexture;
	_int	m_WhiteTexture;
	_int	m_CurrentTexture;
	_bool	m_UseAnim;
	Engine::CTexture*		m_pTextureCom;
	Engine::CShadow*		m_pShadow;


protected:
	HRESULT		Add_Component(void);

public:
	static CBoss2Parts*		Create(LPDIRECT3DDEVICE9 pGraphicDev, _vec3& vPos);

protected:
	virtual void Free(void) override;
};

