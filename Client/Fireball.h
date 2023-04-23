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
	Engine::CTexture* m_pCreateTextureCom;
	Engine::CCollider* m_pCollider;

	_bool  m_bSetTarget;
	_bool  m_bStaticON;
	_float m_fSpeed;
	_float m_fStaticOFF;
	_float m_fDegree;
	_vec3  m_vDir;

public:
	static CFireball* Create(LPDIRECT3DDEVICE9 pGraphicDev, _vec3& vPos);

private:
	virtual void Free(void) override;

};
