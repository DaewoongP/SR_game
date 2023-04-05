#include "stdafx.h"
#include "Monster.h"

#include "Export_Function.h"

CMonster::CMonster(LPDIRECT3DDEVICE9 pGraphicDev)
	: CGameObject(pGraphicDev)
{
}

CMonster::~CMonster()
{
}

_int CMonster::Update_GameObject(const _float & fTimeDelta)
{
	__super::Update_GameObject(fTimeDelta);
	return 0;
}

void CMonster::LateUpdate_GameObject(void)
{
	__super::LateUpdate_GameObject();
}

void CMonster::Render_GameObject(void)
{
	__super::Render_GameObject();
}

void CMonster::Free(void)
{
	__super::Free();
}

