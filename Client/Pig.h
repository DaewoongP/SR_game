#pragma once
#include "Monster.h"
class CPig :
	public CMonster
{
private:
	explicit CPig(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CPig();

public:
	// CMonster을(를) 통해 상속됨
	virtual HRESULT Ready_GameObject(void) override;
	virtual _int Update_GameObject(const _float & fTimeDelta) override;
	virtual void LateUpdate_GameObject(void) override;
	virtual void Render_GameObject(void) override;

private:
	virtual HRESULT Add_Component(void) override;

	virtual _int Update_Too(const _float & fTimeDelta);
	virtual _int Update_Top(const _float & fTimeDelta);
	virtual void LateUpdate_Too();
	virtual void LateUpdate_Top();
	virtual void Render_Too();
	virtual void Render_Top();

	virtual void OnCollisionEnter(const Collision * collision);


public:
	static CPig*		Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	virtual void Free(void) override;

private:
	_bool m_bMoveLeft;
	
};

