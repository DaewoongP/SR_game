#pragma once
#include "GameObject.h"
#include "Include.h"

BEGIN(Engine)
class CRcTex;
class CTexture;
END

#define BOSETAILWAVESTART -90
#define BOSETAILWAVEEND 90


class CBoss2TailBody :
	public CGameObject
{
public:
	explicit CBoss2TailBody(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CBoss2TailBody();

	virtual HRESULT Ready_GameObject(_vec3& vPos) override;
	virtual _int Update_GameObject(const _float& fTimeDelta) override;
	virtual _int Update_Too(const _float& fTimeDelta) override;
	virtual _int Update_Top(const _float& fTimeDelta) override;
	virtual void LateUpdate_GameObject(void) override;
	virtual void Render_GameObject(void) override;
	virtual void Render_Too(void) override;
	virtual void Render_Top(void) override;

private:
	Engine::CRcTex*			m_pBufferCom;
	Engine::CTexture*		m_pTextureCom;

	_bool m_bMetronome = true;
	
	_float m_fTimer = 0.0f;

	_bool		m_bInit;

private:
	HRESULT		Add_Component(void);

public:
	static CBoss2TailBody*		Create(LPDIRECT3DDEVICE9 pGraphicDev, _vec3& vPos);

private:
	virtual void Free(void) override;
};
