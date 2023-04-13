#pragma once

#include "Include.h"
#include "GameObject.h"

BEGIN(Engine)

class CRcTex;
class CTexture;
class CCollider;
class CBlockExp;
END
class CKey : public Engine::CGameObject
{
private:
	explicit CKey(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CKey();

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
	Engine::CBlockExp *	m_pParticle;
	
public:
	static CKey* Create(LPDIRECT3DDEVICE9 pGraphicDev, _vec3& vPos);

private:
	virtual void Free(void) override;

public:
	static size_t iKeyCnt;
};
