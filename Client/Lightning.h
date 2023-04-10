#pragma once
#include "..\Engine\GameObject.h"
#include"..\Engine\Include.h"

BEGIN(Engine)

class CRcTex;
class CTexture;
class CCollider;

END

class CLightning :
    public CGameObject
{public:
	explicit CLightning(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CLightning();

public:

	virtual HRESULT Ready_GameObject(_vec3& vPos) override;
	virtual _int Update_GameObject(const _float& fTimeDelta);
	virtual _int Update_Too(const _float& fTimeDelta) override;
	virtual _int Update_Top(const _float& fTimeDelta) override;
	virtual void		LateUpdate_GameObject(void);

	virtual void Render_Too() override;
	virtual void Render_Top() override;

	Engine::CRcTex* m_pBufferCom;
	Engine::CTexture* m_pTextureCom;
	Engine::CCollider* m_pCollider;

	virtual HRESULT		Add_Component(void);

public:
	static CLightning* Create(LPDIRECT3DDEVICE9 pGraphicDev, _vec3& vPos);
	_vec3 m_vPos;

private:
	virtual void Free() override;
};

