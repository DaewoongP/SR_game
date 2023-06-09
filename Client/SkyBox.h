#pragma once

#include "Include.h"
#include "GameObject.h"

BEGIN(Engine)

class CCubeTex;
class CTexture;
class CSkyParticle;
END
class CSkyBox : public Engine::CGameObject
{
private:
	explicit CSkyBox(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CSkyBox();

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
	Engine::CSkyParticle*	m_pSkyParticle;

	_matrix					m_matWorld;
public:
	static CSkyBox*		Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	virtual void Free(void) override;
};

