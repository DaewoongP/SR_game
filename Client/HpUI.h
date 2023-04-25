#pragma once
#include"Include.h"
#include "GameObject.h"
BEGIN(Engine)

class CRcTex;
class CTexture;

END
class CHpUI :
	public CGameObject
{
private:
	explicit CHpUI(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CHpUI();

public:
	 HRESULT Ready_GameObject(_int Hp,_int i);
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
	_int m_iHp=0;
	_float m_HPUI;
public:
	static CHpUI* Create(LPDIRECT3DDEVICE9 pGraphicDev,_int Hp,_int i);

private:
	virtual void Free(void) override;

};