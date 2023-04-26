#pragma once
#include "DirectCamera.h"
class CFinalCamera_1_1 :
	public CDirectCamera
{
private:
	explicit CFinalCamera_1_1(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CFinalCamera_1_1();
	HRESULT Ready_Camera();
public:
	virtual _int Update_GameObject(const _float& fTimeDelta) override;

private:
	void Update_Direct_X(const _float& fTimeDelta);

public:
	static CFinalCamera_1_1*		Create(LPDIRECT3DDEVICE9 pGraphicDev);
private:
	virtual void Free() override;


};

