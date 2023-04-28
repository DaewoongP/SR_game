#pragma once
#include "Monster.h"

#define BATSCALE 4.0f
#define BATTOOZ 8.0f
#define BATTOPZ 5.0f
class CCube;
BEGIN(Engine)

class CTexture;
class CShadow;

END

class CMiniGameBat :
	public CMonster
{
private:
	explicit CMiniGameBat(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CMiniGameBat();

public:
	virtual HRESULT Ready_GameObject(_vec3& vPos) override;
	virtual _int Update_GameObject(const _float & fTimeDelta) override;
	virtual void LateUpdate_GameObject(void) override;
	virtual void Render_GameObject(void) override;
	virtual void SwapTrigger();

private:
	HRESULT Add_Component(void);

	virtual _int Update_Too(const _float & fTimeDelta);
	virtual _int Update_Top(const _float & fTimeDelta);
	virtual void LateUpdate_Too();

	virtual void Render_Too();
	virtual void Render_Top();

	virtual void OnCollisionEnter(const Collision * collision);
	virtual void OnCollisionStay(const Collision * collision);
	virtual void OnCollisionExit(const Collision* collision);

public:
	static	CMiniGameBat* Create(LPDIRECT3DDEVICE9 pGraphicDev, _vec3& vPos);
private:
	virtual void Free(void) override;

private:
	Engine::CTexture* m_pTextureCom_Back;
	Engine::CShadow* m_pShadowCom;

	_bool m_bMoveLeft;
	_bool m_bBackSprite;
	_vec3 m_vEscapeDir;
	vector<_tchar*> m_vecCol;
	_bool m_bStart2D;
	_float m_fTimer;
	_bool m_bInit;

	CCube* m_GrabCube;
};

