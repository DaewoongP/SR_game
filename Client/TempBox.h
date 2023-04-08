#pragma once
#include "Cube.h"
class CTempBox :
	public CCube
{
private:
	explicit CTempBox(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CTempBox();

public:
	virtual HRESULT Ready_GameObject(_vec3& vPos) override;
	virtual _int Update_GameObject(const _float& fTimeDelta) override;
	virtual void LateUpdate_GameObject(void) override;
	virtual void Render_GameObject(void) override;
	virtual _int Update_Too(const _float& fTimeDelta) override;

	void OnCollisionEnter(const Collision* collision);
	void OnCollisionStay(const Collision* collision);
	void OnCollisionExit(const Collision* collision);
private:
	HRESULT		Add_Component(void);

public:
	static CTempBox* Create(LPDIRECT3DDEVICE9 pGraphicDev, _vec3& vPos);

private:
	virtual void Free() override;
};



