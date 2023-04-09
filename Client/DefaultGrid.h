#pragma once
#include "GameObject.h"

BEGIN(Engine)

class CCollider;
class CRcTex;
class CTexture;

END

class CDefaultGrid : public CGameObject
{
public:
	explicit CDefaultGrid(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CDefaultGrid();

public:
	void	Set_StageTileIndex(int _iIndex) { m_iStageTileIndex = _iIndex; }

public:
	virtual HRESULT Ready_GameObject(_vec3 & vPos) override;
	virtual _int Update_GameObject(const _float & fTimeDelta) override;
	virtual void LateUpdate_GameObject(void) override;
	virtual void Render_GameObject(void) override;

private:
	HRESULT Add_Component(void);
	void	Key_Input();

private:
	Engine::CCollider * m_pCollider;
	Engine::CRcTex* m_pBufferCom;
	Engine::CTexture* m_pTextureCom;
	bool m_bGridON;

	int m_iStageTileIndex;

public:
	static	CDefaultGrid* Create(LPDIRECT3DDEVICE9 pGraphicDev, _vec3 & vPos);

	virtual void Free(void) override;
};

