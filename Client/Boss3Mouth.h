#pragma once
#include "Include.h"
#include "GameObject.h"

BEGIN(Engine)

class CRcTex;
class CTexture;
class CCircularParticle;
END

class CBoss3Mouth : public CGameObject
{
private:
	explicit CBoss3Mouth(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CBoss3Mouth();

public:
	void	Set_Animation() { m_bShootAnimation = true; }

	virtual HRESULT Ready_GameObject(_vec3& vPos);
	virtual _int Update_GameObject(const _float& fTimeDelta) override;
	virtual void LateUpdate_GameObject(void) override;
	virtual void Render_GameObject(void) override;

private:
	HRESULT Add_Component(void);

private:
	Engine::CRcTex*			m_pBufferCom;
	Engine::CTexture*		m_pTextureCom;
	Engine::CCircularParticle*	m_pFireParticle;

	CGameObject*			m_pBoss3;

	_int					m_iIndex;	// 표정 텍스처 구분
	_bool					m_bShootAnimation;

public:
	static CBoss3Mouth*	Create(LPDIRECT3DDEVICE9 pGraphicDev, _vec3& vPos);

protected:
	virtual void Free(void) override;
};

