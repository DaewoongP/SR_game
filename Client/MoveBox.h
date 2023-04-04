#pragma once
#include "Include.h"
#include "GameObject.h"
#include "Cube.h"

BEGIN(Engine)

class CCubeTex;
class CTexture;
class CCollider;

END

class CMoveBox :
	public CCube
{
private:
	explicit CMoveBox(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CMoveBox();

public:
	virtual HRESULT Ready_GameObject(void) override;
	virtual _int Update_GameObject(const _float& fTimeDelta) override;
	virtual void LateUpdate_GameObject(void) override;
	virtual void Render_GameObject(void) override;

private:
	HRESULT Add_Component(void);

private:
	Engine::CCubeTex*	m_pBufferCom;
	Engine::CTexture*	m_pTextureCom;
	Engine::CCollider * m_pCollider;

public:
	static CMoveBox*	Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	virtual void Free(void) override;
};

