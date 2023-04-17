#pragma once
#include "Component.h"

BEGIN(Engine)

class CBulletPool : public CComponent
{
private:
	explicit CBulletPool(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CBulletPool();

public:
	HRESULT		Ready_BulletPool();

private:
	vector<CGameObject*>	m_vecBulletPool;

public:
	static  CBulletPool* Create(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual CComponent * Clone(void) override;

private:
	virtual void Free() override;
};

END