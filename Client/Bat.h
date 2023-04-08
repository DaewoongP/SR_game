#pragma once
#include "Monster.h"

#define BATSCALE 4.0f
#define BATTOOZ 11.0f
#define BATTOPZ 8.0f

BEGIN(Engine)

class CTexture;

END

class CBat :
	public CMonster
{
private:
	explicit CBat(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CBat();

public:
	virtual HRESULT Ready_GameObject(_vec3& vPos) override;
	virtual _int Update_GameObject(const _float & fTimeDelta) override;
	virtual void LateUpdate_GameObject(void) override;
	virtual void Render_GameObject(void) override;

private:
	HRESULT Add_Component(void);

	virtual _int Update_Too(const _float & fTimeDelta);
	virtual _int Update_Top(const _float & fTimeDelta);
	virtual void LateUpdate_Too();
	virtual void LateUpdate_Top();
	virtual void Render_Too();
	virtual void Render_Top();

	virtual void OnCollisionEnter(const Collision * collision);
	virtual void OnCollisionStay(const Collision * collision);
public:
	static	CBat* Create(LPDIRECT3DDEVICE9 pGraphicDev, _vec3& vPos);
private:
	virtual void Free(void) override;

private:
	CTexture* m_pTextureCom_Back;

	_bool m_bMoveLeft;

	_bool m_bBackSprite;

	_float m_fZRatio;

};

