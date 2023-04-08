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
	void Set_TopCol(bool _tf) { m_bTopCol = _tf; }
	void Set_TooCol(bool _tf) { m_bTooCol = _tf; }

public:
	virtual HRESULT Ready_GameObject(_vec3& vPos) override;
	virtual _int Update_GameObject(const _float& fTimeDelta) override;
	virtual _int Update_Too(const _float& fTimeDelta) override;
	virtual _int Update_Top(const _float& fTimeDelta) override;
	virtual void LateUpdate_GameObject(void) override;
	virtual void Render_GameObject(void) override;

	virtual void OnCollisionEnter(const Collision* collision);
	virtual void OnCollisionStay(const Collision* collision);

private:
	HRESULT		Add_Component(void);
private:
	Engine::CRcTex*    m_pBufferCom;
	Engine::CTexture*  m_pTextureCom;
	Engine::CCollider* m_pCollider;

	CGameObject* m_pPlayer1;
	CGameObject* m_pPlayer2;

	bool m_bTooCol;
	bool m_bTopCol;
	bool m_bCreateSwallowPortal;

public:
	static CPortal* Create(LPDIRECT3DDEVICE9 pGraphicDev, _vec3& vPos);

private:
	virtual void Free(void) override;
};

