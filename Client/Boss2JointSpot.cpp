#include "stdafx.h"
#include "Boss2JointSpot.h"

CBoss2JointSpot::CBoss2JointSpot(LPDIRECT3DDEVICE9 pGraphicDev)
	:CGameObject(pGraphicDev)
{
}

CBoss2JointSpot::~CBoss2JointSpot()
{
}

HRESULT CBoss2JointSpot::Ready_GameObject(_vec3 & vPos)
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);
	m_pTransform->m_vInfo[INFO_POS] = vPos;
	return S_OK;
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

HRESULT CBoss2JointSpot::Add_Component(void)
{
	return S_OK;
}

void CBoss2JointSpot::Free(void)
{
	__super::Free();
}
