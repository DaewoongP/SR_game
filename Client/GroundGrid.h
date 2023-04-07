#pragma once
#include "Grid.h"

class CGroundGrid : public CGrid
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
	virtual HRESULT Add_Component(void) override;

public:
	static	CGroundGrid* Create(LPDIRECT3DDEVICE9 pGraphicDev, _vec3& vPos);

private:
	virtual void Free(void) override;
};

