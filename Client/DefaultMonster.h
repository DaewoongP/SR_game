#pragma once
#include "Monster.h"

class CDefaultMonster : public CMonster
{
private:
	explicit CDefaultMonster(PDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CDefaultMonster();

public:
	// CMonster을(를) 통해 상속됨
	virtual HRESULT Ready_GameObject(_vec3& vPos) override;
	virtual _int Update_GameObject(const _float & fTimeDelta) override;
	virtual void LateUpdate_GameObject(void) override;
	virtual void Render_GameObject(void) override;

private:
	HRESULT Add_Component(void);
	void Mouse_Move();

public:
	static CDefaultMonster*	Create(LPDIRECT3DDEVICE9 pGraphicDev, _vec3& vPos);

private:
	virtual void Free(void) override;
};

