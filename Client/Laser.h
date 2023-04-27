#pragma once
#include "Include.h"
#include "Bullet.h"
BEGIN(Engine)
class CCollider;
END

class CLaser : public CBullet
{
private:
	explicit CLaser(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CLaser();

public:
	virtual HRESULT Ready_Bullet(_vec3& vPos, _vec3& vDir = _vec3(0.f, 1.f, 0.f)) override;
	virtual void	Ready_Pool(_vec3& vPos, _vec3& vDir = _vec3(0.f, 1.f, 0.f)) override;
	virtual _int Update_GameObject(const _float& fTimeDelta) override;
	virtual void LateUpdate_GameObject(void) override;
	virtual void Render_GameObject(void) override;
	virtual void OnCollisionEnter(const class Collision* collision);
	_bool Set_OverP() { return m_bOverPower = true; }
	_bool Get_OverP() { return m_bOverPower; }
private:
	HRESULT Add_Component(void);

private:
	Engine::CCollider* m_pCollider;
	_bool	m_bShoot;
	_vec3 m_vPos;
	_vec3 m_vTarget;
	_float m_fEndTime;
public:
	_bool m_bOverPower = false;

public:
	static CLaser* Create(LPDIRECT3DDEVICE9 pGraphicDev, _vec3& vPos);

private:
	virtual void Free(void) override;
};

