#pragma once
#include "GameObject.h"
#include "Include.h"

//위치정보만 가지고있을 친구임.
class CBoss2JointSpot :
	public CGameObject
{
private:
	explicit CBoss2JointSpot(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CBoss2JointSpot();

	virtual HRESULT Ready_GameObject(_vec3& vPos) override;
	virtual _int Update_GameObject(const _float& fTimeDelta) override;

public:
	static CBoss2JointSpot*		Create(LPDIRECT3DDEVICE9 pGraphicDev, _vec3& vPos);

private:
	HRESULT		Add_Component(void);

private:
	virtual void Free(void) override;
};

