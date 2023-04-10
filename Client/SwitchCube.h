#pragma once
#include "Cube.h"

class CSwitchCube : public CCube
{
private:
	explicit CSwitchCube(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CSwitchCube();

public:
	void	Set_SwitchON(bool _TrueorFalse) { m_bSwitchON = _TrueorFalse; }

public:
	virtual HRESULT Ready_GameObject(_vec3& vPos) override;
	virtual _int Update_GameObject(const _float& fTimeDelta) override;
	virtual void LateUpdate_GameObject(void) override;
	virtual void Render_GameObject(void) override;

	virtual void OnCollisionEnter(const class Collision* collision);

private:
	HRESULT		Add_Component(void);

public:
	static CSwitchCube* Create(LPDIRECT3DDEVICE9 pGraphicDev, _vec3& vPos);

private:
	bool	m_bSwitchON;

private:
	virtual void Free() override;
};

