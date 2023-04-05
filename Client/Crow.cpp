#include "stdafx.h"
#include "Crow.h"


CCrow::CCrow(LPDIRECT3DDEVICE9 pGraphicDev)
	:CMonster(pGraphicDev)
{
}


CCrow::~CCrow()
{
}

HRESULT CCrow::Ready_GameObject(void)
{

	return S_OK;
}

_int CCrow::Update_GameObject(const _float & fTimeDelta)
{
	return 0;
}

void CCrow::LateUpdate_GameObject(void)
{
}

void CCrow::Render_GameObject(void)
{
}

CCrow * CCrow::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CCrow*		pInstance = new CCrow(pGraphicDev);

	if (FAILED(pInstance->Ready_GameObject()))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CCrow::Free(void)
{
}

HRESULT CCrow::Add_Component(void)
{
	return E_NOTIMPL;
}
