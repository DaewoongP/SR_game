#pragma once
#include "GameObject.h"
#include "Include.h"

//위치정보만 가지고있을 친구임.
class CBoss2JointSpot :
	public CGameObject
{
public:
	explicit CBoss2JointSpot(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CBoss2JointSpot();

	virtual _int Update_GameObject(const _float& fTimeDelta) override;

public:
	static CBoss2JointSpot*		Create(LPDIRECT3DDEVICE9 pGraphicDev, _vec3& vPos);

private:
	virtual void Free(void) override;
};

