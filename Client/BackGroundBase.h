#pragma once
#include "Include.h"
#include "GameObject.h"

BEGIN(Engine)
class CRcTex;
class CTexture;
END
class CBackGroundBase :
	public CGameObject
{
protected:
	explicit CBackGroundBase(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CBackGroundBase();

public:
	virtual _int Update_GameObject(const _float& fTimeDelta) override;
	virtual _int Update_Too(const _float & fTimeDelta);
	virtual _int Update_Top(const _float & fTimeDelta);
	virtual void Render_Too();
	virtual void Render_Top();
	virtual void Render_GameObject(void) override;
	static CBackGroundBase*	Create(LPDIRECT3DDEVICE9 pGraphicDev, _vec3& vPos, _float fScale, _float fAngle);
	static CBackGroundBase*	Create(LPDIRECT3DDEVICE9 pGraphicDev, _vec3& vPos ,_float fScale);
protected:
	virtual HRESULT Ready_GameObject(_vec3& vPos, _float fScale, _float fAngle);
	virtual HRESULT Ready_GameObject(_vec3& vPos, _float fScale);
	virtual HRESULT Add_Component(void);
protected:
	Engine::CRcTex*			m_pBufferCom;
	Engine::CTexture*		m_pTextureCom;

	_float m_fStaticAngle;

	_float m_fTimer;

	_float m_fSpeed;

protected:
	virtual void Free(void);
};

