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

void CMonster::OnCollisionEnter(const Collision * collision)
{
	__super::OnCollisionEnter(collision);
}

void CMonster::OnCollisionStay(const Collision * collision)
{
	__super::OnCollisionStay(collision);
}

void CMonster::OnCollisionExit(const Collision * collision)
{
	__super::OnCollisionExit(collision);
}

void CMonster::Free(void)
{
	__super::Free();
}

