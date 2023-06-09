#pragma once
#include "GameObject.h"
#include "Include.h"

class CTopdeeJoint :
	public CGameObject
{
private:
	explicit CTopdeeJoint(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CTopdeeJoint();

	virtual HRESULT Ready_GameObject(_vec3& vPos) override;
	virtual _int Update_GameObject(const _float& fTimeDelta) override;

public:
	static CTopdeeJoint*		Create(LPDIRECT3DDEVICE9 pGraphicDev, _vec3& vPos);

private:
	HRESULT		Add_Component(void);

private:
	virtual void Free(void) override;
};

