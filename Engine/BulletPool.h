#pragma once
#include "Component.h"

BEGIN(Engine)

class CBulletPool : public CComponent
{
private:
	explicit CBulletPool(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CBulletPool();

public:
	HRESULT			Ready_BulletPool();

	CGameObject*	Use_Bullet(_vec3& vPos, _int eDir);
	void			UnUse_Bullet();

private:
	vector<pair<CGameObject*, _bool>>	m_vecBulletPool;
	_bool	m_bCreate;

public:
	static  CBulletPool* Create(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual CComponent * Clone(void);

private:
	virtual void Free() override;
};

END