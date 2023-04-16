#pragma once
#include "Include.h"
#include "GameObject.h"

BEGIN(Engine)
class CCollider;
END

class CEarthQuake :
	public CGameObject
{
private:
	explicit CEarthQuake(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CEarthQuake();

public:
	virtual HRESULT Ready_GameObject(_vec3& vPos) override;
	virtual _int Update_GameObject(const _float& fTimeDelta) override;
	virtual _int Update_Too(const _float& fTimeDelta) override;
	virtual _int Update_Top(const _float& fTimeDelta) override;
	virtual void LateUpdate_GameObject(void) override;
	virtual void Render_GameObject(void) override;
	virtual void SwapTrigger();

	virtual void OnCollisionEnter(const class Collision* collision);
	virtual void OnCollisionStay(const class Collision* collision);
	virtual void OnCollisionExit(const class Collision* collision);

	void	SetDir(_vec3 _dir) { dir = _dir; }
	_vec3	GetDir() { return dir; }

private:
	HRESULT		Add_Component(void);

private:
	_vec3 dir;
	Engine::CCollider*		m_pCollider;

public:
	static CEarthQuake*		Create(LPDIRECT3DDEVICE9 pGraphicDev, _vec3& vPos);

private:
	virtual void Free(void) override;

};

