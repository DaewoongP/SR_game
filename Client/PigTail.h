#pragma once

#include "Include.h"
#include "GameObject.h"

BEGIN(Engine)
class CRcTex;
class CTexture;
END

class CPigTail : public Engine::CGameObject
{
private:
	explicit CPigTail(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CPigTail();

public:
	virtual HRESULT Ready_GameObject(void);
	virtual _int Update_GameObject(const _float& fTimeDelta);
	virtual void LateUpdate_GameObject(void);
	virtual void Render_GameObject(void);

	virtual _int Update_Top(const _float & fTimeDelta);

	virtual void Render_Top();

private:
	virtual HRESULT		Add_Component(void);

public:
	static CPigTail*		Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	Engine::CRcTex*			m_pBufferCom;
	Engine::CTexture*		m_pTextureCom;

private:
	virtual void Free(void);
};

