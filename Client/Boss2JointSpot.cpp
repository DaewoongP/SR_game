#include "stdafx.h"
#include "Boss2JointSpot.h"

CBoss2JointSpot::CBoss2JointSpot(LPDIRECT3DDEVICE9 pGraphicDev)
	:CGameObject(pGraphicDev)
{
}

CBoss2JointSpot::~CBoss2JointSpot()
{
}

_int CBoss2JointSpot::Update_GameObject(const _float & fTimeDelta)
{
	__super::Update_GameObject(fTimeDelta);
	return 0;
}

CBoss2JointSpot * CBoss2JointSpot::Create(LPDIRECT3DDEVICE9 pGraphicDev, _vec3 & vPos)
{
	CBoss2JointSpot*		pInstance = new CBoss2JointSpot(pGraphicDev);

	if (FAILED(pInstance->Ready_GameObject(vPos)))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CBoss2JointSpot::Free(void)
{
	__super::Free();
}
