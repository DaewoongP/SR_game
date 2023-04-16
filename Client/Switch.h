#pragma once
#include "GameObject.h"

BEGIN(Engine)

class CCollider;
class CRcTex;
class CTexture;
class CShadow;

END

class CSwitch : public Engine::CGameObject
{
private:
	explicit CSwitch(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CSwitch();

public:
	virtual HRESULT Ready_GameObject(_vec3& vPos) override;
	virtual _int Update_GameObject(const _float& fTimeDelta) override;
	virtual void LateUpdate_GameObject(void) override;
	virtual void Render_GameObject(void) override;

	virtual void OnCollisionEnter(const class Collision* collision);
	virtual void OnCollisionExit(const class Collision* collision);

private:
	HRESULT		Add_Component(void);

private:
	Engine::CRcTex*    m_pBufferCom;
	Engine::CTexture*  m_pTextureCom;
	Engine::CCollider* m_pCollider;
	Engine::CShadow*		m_pShadow;

	CGameObject* m_pSwitchCube;

	int m_iTextureIndex;

public:
	static bool m_bSwtichON;

public:
	static CSwitch* Create(LPDIRECT3DDEVICE9 pGraphicDev, _vec3& vPos);

private:
	virtual void Free() override;
};

