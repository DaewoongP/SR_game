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
	return E_NOTIMPL;
}

_int CCrow::Update_GameObject(const _float & fTimeDelta)
{
	return _int();
}

void CCrow::LateUpdate_GameObject(void)
{
}

void CCrow::Render_GameObject(void)
{
}

CCrow * CCrow::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	return nullptr;
}

void CCrow::Free(void)
{
}
