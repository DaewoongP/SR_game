#pragma once
#include "GameObject.h"

BEGIN(Engine)

class CCollider;
class CRcTex;
class CTexture;

END

class CInstallGrid : public CGameObject
{
private:
	explicit CInstallGrid(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CInstallGrid();

public:
	void	Set_StageTileIndex(int _iIndex) { m_iStageTileIndex = _iIndex; }

public:
	virtual HRESULT Ready_GameObject(_vec3 & vPos) override;
	virtual _int	Update_GameObject(const _float & fTimeDelta) override;
	virtual void	LateUpdate_GameObject(void) override;
	virtual void	Render_GameObject(void) override;

	void OnCollisionEnter(const Collision* collision);
	void OnCollisionStay(const Collision* collision);
	void OnCollisionExit(const Collision* collision);

private:
	HRESULT Add_Component(void);

private:
	Engine::CCollider * m_pCollider;
	Engine::CRcTex* m_pBufferCom;
	Engine::CTexture* m_pTextureCom;

	int m_iStageTileIndex;

public:
	static	CInstallGrid* Create(LPDIRECT3DDEVICE9 pGraphicDev, _vec3 & vPos);

	virtual void Free(void) override;
};