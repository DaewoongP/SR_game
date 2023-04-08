#pragma once
#include "Grid.h"

BEGIN(Engine)

class CCollider;

END

class CInstallGrid : public CGrid
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

private:
	virtual HRESULT Add_Component(void) override;

private:
	Engine::CCollider * m_pCollider;

	int m_iStageTileIndex;

public:
	static	CInstallGrid* Create(LPDIRECT3DDEVICE9 pGraphicDev, _vec3 & vPos);

	virtual void Free(void) override;
};