#pragma once

#include "Include.h"
#include "GameObject.h"

#define CLOUDTOOZ 10.5f
#define CLOUDTOPZ 6.0f

BEGIN(Engine)

class CRcTex;
class CTexture;
class CCollider;

END

class CCloud :
	public CGameObject
{
protected:
	explicit CCloud(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CCloud();

protected:
	virtual HRESULT		Add_Component(void) PURE;

	virtual _int Update_Too(const _float& fTimeDelta) override;
	virtual _int Update_Top(const _float& fTimeDelta) override;

	Engine::CRcTex*			m_pBufferCom;
	Engine::CTexture*		m_pTextureCom;
	Engine::CCollider*		m_pCollider;

	_float m_fZRatio;
};

