#pragma once
#include "MoveCube.h"

BEGIN(Engine)
class CRigidbody;
END

class CGravityCube :
	public CMoveCube
{
public:
	explicit CGravityCube(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CGravityCube();

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

public:
	static CGravityCube*	Create(LPDIRECT3DDEVICE9 pGraphicDev, _vec3& vPos);
	void SetGravity(_bool value) { m_bIsStone = !value; }

private:
	void Do_CheckRay_Down();

private:
	_bool	m_bUseGraivty;
	CRigidbody* m_pRigid;

private:
	virtual void Free(void) override;

private:
	HRESULT		Add_Component(void);
};

