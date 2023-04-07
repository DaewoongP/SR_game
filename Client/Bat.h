#pragma once
#include "Monster.h"
class CBat : public CMonster
{
private:
	explicit CBat(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CBat();
public:
	virtual HRESULT Ready_GameObject(_vec3& vPos) override;
	virtual _int Update_GameObject(const _float & fTimeDelta) override;
	virtual void LateUpdate_GameObject(void) override;
	virtual void Render_GameObject(void) override;

	virtual void OnCollisionEnter(const class Collision* collsion);
	virtual void OnCollisionStay(const class Collision* collsion);
	virtual void OnCollisionExit(const class Collision* collsion);
private:
	HRESULT Add_Component(void);
public:
	static	CBat* Create(LPDIRECT3DDEVICE9 pGraphicDev, _vec3& vPos);
private:
	virtual void Free(void) override;
};

