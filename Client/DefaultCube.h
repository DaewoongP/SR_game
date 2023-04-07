#pragma once
#include "Cube.h"

class CDefaultCube :
	public CCube
{
private:
	explicit CDefaultCube(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CDefaultCube();

public:
	void	Set_CubeIndex(int _iIndex) { m_iCubeIndex = _iIndex; }

public:
	virtual HRESULT Ready_GameObject(_vec3& vPos) override;
	virtual _int Update_GameObject(const _float& fTimeDelta) override;
	virtual void LateUpdate_GameObject(void) override;
	virtual void Render_GameObject(void) override;
	virtual void OnCollisionEnter(const class Collision* collision);
	virtual void OnCollisionStay(const class Collision* collision);
	virtual void OnCollisionExit(const class Collision* collision);

private:
	HRESULT Add_Component(void);
	void	On_Grid();

private:
	int m_iCubeIndex;

public:
	static CDefaultCube*	Create(LPDIRECT3DDEVICE9 pGraphicDev, _vec3& vPos);

private:
	virtual void Free(void) override;
};

