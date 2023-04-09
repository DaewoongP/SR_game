#pragma once

#include "Include.h"
#include "GameObject.h"

BEGIN(Engine)

class CRcTex;
class CTexture;
class CCollider;

END
class CFireball : public Engine::CGameObject
{
private:
	explicit CFireball(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CFireball();

public:
	virtual HRESULT Ready_GameObject(_vec3& vPos) override;
	virtual _int Update_GameObject(const _float& fTimeDelta) override;
	virtual _int Update_Too(const _float& fTimeDelta) override;
	virtual _int Update_Top(const _float& fTimeDelta) override;
	virtual void LateUpdate_GameObject(void) override;
	virtual void Render_GameObject(void) override;
	virtual void OnCollisionEnter(const Collision* collision);
private:
	HRESULT		Add_Component(void);
private:
	Engine::CRcTex* m_pBufferCom;
	Engine::CTexture* m_pTextureCom;
	Engine::CCollider* m_pCollider;
public:
	static CFireball* Create(LPDIRECT3DDEVICE9 pGraphicDev, _vec3& vPos);

private:
	virtual void Free(void) override;

};
