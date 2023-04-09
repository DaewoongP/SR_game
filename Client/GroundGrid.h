#pragma once
#include "GameObject.h"

BEGIN(Engine)

class CCollider;
class CRcTex;
class CTexture;

END

class CGroundGrid : public CGameObject
{
private:
	explicit CGroundGrid(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CGroundGrid();

public:
	void	Set_GridOn(bool TrueorFalse) { m_bGridON = TrueorFalse; }

public:
	virtual HRESULT Ready_GameObject(_vec3& vPos) override;
	virtual _int Update_GameObject(const _float& fTimeDelta) override;
	virtual void LateUpdate_GameObject(void) override;
	virtual void Render_GameObject(void) override;

private:
	HRESULT Add_Component(void);

private:
	Engine::CCollider * m_pCollider;
	Engine::CRcTex* m_pBufferCom;
	Engine::CTexture* m_pTextureCom;
	bool m_bGridON;

public:
	static	CGroundGrid* Create(LPDIRECT3DDEVICE9 pGraphicDev, _vec3& vPos);

private:
	virtual void Free(void) override;
};

