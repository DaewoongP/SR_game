#pragma once

#include "Include.h"
#include "GameObject.h"

BEGIN(Engine)

class CRcTex;
class CTexture;
class CCollider;
class CWaterParticle;
class CCircularParticle;
END
class CWaterPipe : public Engine::CGameObject
{
private:
	explicit CWaterPipe(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CWaterPipe();

public:
	virtual HRESULT Ready_GameObject(_vec3& vPos) override;
	virtual _int Update_GameObject(const _float& fTimeDelta) override;
	virtual _int Update_Too(const _float& fTimeDelta) override;
	virtual _int Update_Top(const _float& fTimeDelta) override;

	virtual void LateUpdate_GameObject(void) override;
	virtual void Render_GameObject(void) override;

	BoundingBox Check_BoundingBox();



	_matrix* WaterFallPos();
	_matrix* WaterSplashPos();
private:
	HRESULT		Add_Component(void);
private:
	Engine::CRcTex* m_pBufferCom;
	Engine::CRcTex* m_pwaterFall;
	Engine::CRcTex* m_pwaterSplash;
	Engine::CRcTex* m_pSucking;

	Engine::CTexture* m_pTextureCom;
	Engine::CTexture* m_pWaterFallTex;
	Engine::CTexture* m_pWaterSplashTex;
	Engine::CTexture* m_pSuckingTex;

	Engine::CCollider* m_pCollider;
	Engine::CWaterParticle* m_pWaterParticle;
	Engine::CCircularParticle* m_pCircularParticle;

public:
	static CWaterPipe* Create(LPDIRECT3DDEVICE9 pGraphicDev, _vec3& vPos);

private:
	virtual void Free(void) override;
	_bool m_bInit = true;

	BoundingBox				m_Box, m_InitBox;
	_vec3					m_vWaterFall;
	_vec3					m_vWaterSplash;

};
