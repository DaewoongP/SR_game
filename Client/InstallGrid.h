#pragma once
#include "Grid.h"

class CInstallGrid : public CGrid
{
private:
	explicit CInstallGrid(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CInstallGrid();

public:
	void	Set_StageTileIndex(int _iIndex) { m_iStageTileIndex = _iIndex; }

public:
	virtual HRESULT Ready_GameObject(void) override;
	virtual _int	Update_GameObject(const _float & fTimeDelta) override;
	virtual void	LateUpdate_GameObject(void) override;
	virtual void	Render_GameObject(void) override;

private:
	virtual HRESULT Add_Component(void) override;

private:
	int m_iStageTileIndex;

public:
	static	CInstallGrid* Create(LPDIRECT3DDEVICE9 pGraphicDev);

	virtual void Free(void) override;
};

