#include "stdafx.h"
#include "Bullet.h"
#include "Export_Function.h"

CBullet::CBullet(LPDIRECT3DDEVICE9 pGraphicDev)
	:CGameObject(pGraphicDev),
	m_bUse(true),
	m_fSpeed(1.f)
{
}

CBullet::~CBullet()
{
}

HRESULT CBullet::Ready_GameObject(_vec3 & vPos, _int eDir)
{
	return S_OK;
}

_int CBullet::Update_GameObject(const _float & fTimeDelta)
{
	return _int();
}

void CBullet::LateUpdate_GameObject(void)
{
}

void CBullet::Render_GameObject(void)
{
}

void CBullet::OnCollisionEnter(const Collision * collision)
{
}

HRESULT CBullet::Add_Component(void)
{
	return E_NOTIMPL;
}

CBullet * CBullet::Create(LPDIRECT3DDEVICE9 pGraphicDev, _vec3 & vPos, _int eDir)
{
	return nullptr;
}

void CBullet::Free(void)
{
}
