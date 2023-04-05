#pragma once
#include "Monster.h"
class CCrow : public CMonster
{
private:
	explicit CCrow(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CCrow();

	virtual HRESULT Ready_GameObject(void) override;
	virtual _int Update_GameObject(const _float & fTimeDelta) override;
	virtual void LateUpdate_GameObject(void) override;
	virtual void Render_GameObject(void) override;

public:
	static	CCrow* Create(LPDIRECT3DDEVICE9 pGraphicDev);
private:
	virtual void Free(void) override;

	// CMonster을(를) 통해 상속됨
	virtual HRESULT Add_Component(void) override;
};

