#pragma once

#include "Include.h"
#include "GameObject.h"

BEGIN(Engine)

class CCubeTex;
class CTexture;
class CStarParticle;
END
class CStarBox : public Engine::CGameObject
{
private:
	explicit CStarBox(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CStarBox();

public:
	virtual HRESULT Ready_GameObject(void) override;
	virtual _int Update_GameObject(const _float& fTimeDelta) override;
	virtual void LateUpdate_GameObject(void) override;
	virtual void Render_GameObject(void) override;

private:
	HRESULT		Add_Component(void);

	_matrix		GetRotationMatrix(const _matrix& OriginMatrix);

private:
	Engine::CCubeTex*		m_pBufferCom;
	Engine::CTexture*		m_pTextureCom;
	Engine::CStarParticle*	m_pStarParticle;

	_matrix					m_matWorld;
public:
	static CStarBox*		Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	virtual void Free(void) override;
};

