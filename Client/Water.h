#pragma once

#include "Include.h"
#include "GameObject.h"

BEGIN(Engine)

class CRcTex;
class CTexture;
class CCollider;

END
class CWater : public Engine::CGameObject
{
private:
	explicit CWater(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CWater();

public:
	virtual HRESULT Ready_GameObject(_vec3& vPos) override;
	virtual _int Update_GameObject(const _float& fTimeDelta) override;
	virtual void LateUpdate_GameObject(void) override;
	virtual void Render_GameObject(void) override;

	virtual void OnCollisionEnter(const Collision* collision);
	virtual void OnCollisionExit(const Collision* collision);
private:
	HRESULT		Add_Component(void);
private:
	Engine::CRcTex* m_pBufferCom;
	Engine::CTexture* m_pTextureCom;
	Engine::CCollider* m_pCollider;
	Engine::CSparkParticle* m_pSparkParticle;
	Engine::CCircularParticle* m_pCircularParticle;



public:
	static CWater* Create(LPDIRECT3DDEVICE9 pGraphicDev, _vec3& vPos);

private:
	virtual void Free(void) override;

};
