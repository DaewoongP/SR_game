#pragma once

#include "Include.h"
#include "GameObject.h"

BEGIN(Engine)

class CRcTex;
class CTexture;
class CLeafParticle;
END
class CStage1BG : public Engine::CGameObject
{
private:
	explicit CStage1BG(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CStage1BG();

public:
	virtual HRESULT Ready_GameObject(void) override;
	virtual _int Update_GameObject(const _float& fTimeDelta) override;
	virtual void LateUpdate_GameObject(void) override;
	virtual void Render_GameObject(void) override;

private:
	HRESULT		Add_Component(void);
	void		Render_Particle();

private:
	Engine::CRcTex*			m_pBufferCom;
	Engine::CTexture*		m_pTextureCom;
	Engine::CLeafParticle*	m_pLeafParticle0;
	Engine::CLeafParticle*	m_pLeafParticle1;
	Engine::CLeafParticle*	m_pLeafParticle2;

public:
	static CStage1BG*		Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	virtual void Free(void) override;

};

