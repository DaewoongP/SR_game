#pragma once
#include "Monster.h"

#define PIGSCALE 2.0f

BEGIN(Engine)

class CTexture;

END

class CPig :
	public CMonster
{
private:
	explicit CPig(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CPig();

public:
	virtual HRESULT Ready_GameObject(_vec3& vPos) override;
	virtual _int Update_GameObject(const _float & fTimeDelta) override;
	virtual void LateUpdate_GameObject(void) override;
	virtual void Render_GameObject(void) override;

private:
	HRESULT Add_Component(void);

	virtual _int Update_Too(const _float & fTimeDelta);
	virtual _int Update_Top(const _float & fTimeDelta);
	virtual void Render_Too();
	virtual void Render_Top();

	virtual void OnCollisionEnter(const Collision * collision);
	virtual void OnCollisionStay(const Collision * collision);
public:
	static CPig*	Create(LPDIRECT3DDEVICE9 pGraphicDev, _vec3& vPos);

private:
	virtual void Free(void) override;

private:
	Engine::CTexture* m_pTextureCom_Back;
	Engine::CShadow* m_pShadowCom;

	_bool m_bMoveLeft;

	_bool m_bBackSprite;

};

