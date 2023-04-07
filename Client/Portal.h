#pragma once

#include "Include.h"
#include "GameObject.h"

BEGIN(Engine)

class CRcTex;
class CTexture;
class CCollider;

END

class CPortal : public Engine::CGameObject
{
private:
	explicit CPortal(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CPortal();

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
	Engine::CRcTex*    m_pBufferCom;
	Engine::CTexture*  m_pTextureCom;
	Engine::CCollider* m_pCollider;

public:
	static CPortal* Create(LPDIRECT3DDEVICE9 pGraphicDev, _vec3& vPos);

private:
	virtual void Free(void) override;
};

