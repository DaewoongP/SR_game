#pragma once
#include"Include.h"
#include "GameObject.h"
BEGIN(Engine)

class CRcTex;
class CTexture;

END
class CFinalUI_B :
	public CGameObject
{
private:
	explicit CFinalUI_B(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CFinalUI_B();

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
	_float m_fMaxHp = 0;
	_float m_fHpPer;
	_float m_HPUI;
	_float m_fDelay;
	_bool m_bShakeDir;
	_float m_BarMax;
	_float m_BarPos;
public:
	static CFinalUI_B* Create(LPDIRECT3DDEVICE9 pGraphicDev, _float Hp);


private:
	virtual void Free(void) override;

};