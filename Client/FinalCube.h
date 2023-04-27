#pragma once
#include "Cube.h"

class CFinalCube :
	public CCube
{
private:
	explicit CFinalCube(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CFinalCube();

public:
	virtual HRESULT Ready_GameObject(_vec3& vPos, _int iIndex);
	virtual _int Update_GameObject(const _float& fTimeDelta) override;
	virtual void LateUpdate_GameObject(void) override;
	virtual void Render_GameObject(void) override;

	virtual void OnCollisionEnter(const class Collision* collision);

private:
	HRESULT		Add_Component(void);

private:
	_int		m_iCubeIndex;

public:
	static CFinalCube* Create(LPDIRECT3DDEVICE9 pGraphicDev, _vec3& vPos, _int iIndex);

private:
	virtual void Free() override;
};

