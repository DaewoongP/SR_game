#pragma once
#include "GameObject.h"
BEGIN(Engine)

class CRcTex;
class CTexture;

END

class CChapterCloudTex :
	public CGameObject
{
private:
	explicit CChapterCloudTex(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CChapterCloudTex();

public:
	virtual HRESULT Ready_GameObject(void) override;
	virtual _int Update_GameObject(const _float& fTimeDelta) override;
	virtual void Render_GameObject(void) override;

private:
	HRESULT		Add_Component(void);

private:
	Engine::CRcTex*		m_pBufferCom;
	Engine::CTexture*	m_pChapterTex;

public:
	static CChapterCloudTex*		Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	virtual void Free(void) override;
};
