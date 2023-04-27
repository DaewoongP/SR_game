#pragma once
#include "GameObject.h"
BEGIN(Engine)

class CRcTex;
class CTexture;

END
class CProduceObject :
	public CGameObject
{
protected:
	explicit CProduceObject(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CProduceObject();
protected:
	Engine::CRcTex*			m_pBufferCom;
	Engine::CTexture*		m_pTextureCom;
protected:
	virtual void		Free(void);
};

