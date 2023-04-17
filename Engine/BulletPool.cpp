#include "stdafx.h"
#include "BulletPool.h"

#include "Export_Function.h"
#include "../Client/Laser.h"

CBulletPool::CBulletPool(LPDIRECT3DDEVICE9 pGraphicDev)
	:CComponent(pGraphicDev)
{
	m_vecBulletPool.reserve(16);
}

CBulletPool::~CBulletPool()
{
}

HRESULT CBulletPool::Ready_BulletPool()
{
	CLayer* pLayer = Get_Layer(L"Layer_GameLogic");

	for (int i = 0; i < 16; ++i)
	{
		LPDIRECT3DDEVICE9 pGraphicDev = Get_GraphicDev();
		CGameObject* pGameObject = CLaser::Create(pGraphicDev, _vec3{0.f, 0.f, 0.f}, 0);
		NULL_CHECK_RETURN(pGameObject, E_FAIL);
		pGameObject->Sort_Component();
		FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Laser", pGameObject), E_FAIL);
	}	

	return  S_OK;
}

CBulletPool * CBulletPool::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CBulletPool *	pInstance = new CBulletPool(pGraphicDev);

	if (FAILED(pInstance->Ready_BulletPool()))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

CComponent * CBulletPool::Clone(void)
{
	return new CBulletPool(*this);
}

void CBulletPool::Free()
{
	__super::Free();
}
