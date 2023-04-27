#pragma once
#include"Include.h"
#include "GameObject.h"
BEGIN(Engine)

class CRcTex;
class CTexture;

END
class CFinalUI :
	public CGameObject
{
private:
	explicit CFinalUI(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CFinalUI();

public:
	HRESULT Ready_GameObject(_float Hp);
	virtual _int Update_GameObject(const _float& fTimeDelta) override;
	virtual void LateUpdate_GameObject(void) override;
	virtual void Render_GameObject(void) override;

private:
	HRESULT		Add_Component(void);

private:
	Engine::CRcTex* m_pBufferCom;
	Engine::CTexture* m_pTextureCom;

	_matrix m_matProjection;
	_matrix m_matViewSpace;

	_int	m_iPreBoss1Hp;

	_bool m_bShakeDir;
	_float m_BarMax;

public:
	static CFinalUI* Create(LPDIRECT3DDEVICE9 pGraphicDev, _float Hp);


private:
	virtual void Free(void) override;

};