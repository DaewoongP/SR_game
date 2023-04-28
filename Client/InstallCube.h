#pragma once
#include "Cube.h"

class CInstallCube :
	public CCube
{
private:
	explicit CInstallCube(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CInstallCube();

public:
	virtual HRESULT Ready_GameObject(_vec3& vPos, int iIndex);
	virtual _int Update_GameObject(const _float& fTimeDelta) override;
	virtual void LateUpdate_GameObject(void) override;
	virtual void Render_GameObject(void) override;
	virtual void OnCollisionEnter(const class Collision* collision);
	virtual void OnCollisionStay(const class Collision* collision);
	virtual void OnCollisionExit(const class Collision* collision);

private:
	HRESULT Add_Component(void);

private:
	int m_iCubeIndex;

public:
	static CInstallCube*	Create(LPDIRECT3DDEVICE9 pGraphicDev, _vec3& vPos, int iIndex);

private:
	virtual void Free(void) override;
};

