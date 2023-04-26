#include "stdafx.h"
#include "BulletPlayer.h"


CBulletPlayer::CBulletPlayer(LPDIRECT3DDEVICE9 pGraphicDev)
	:
	CGameObject(pGraphicDev)
{
}

CBulletPlayer::~CBulletPlayer()
{
}

HRESULT CBulletPlayer::Ready_GameObject(_vec3 & vPos)
{
	return E_NOTIMPL;
}

_int CBulletPlayer::Update_GameObject(const _float & fTimeDelta)
{
	return _int();
}

_int CBulletPlayer::Update_Too(const _float & fTimeDelta)
{
	return _int();
}

_int CBulletPlayer::Update_Top(const _float & fTimedDelte)
{
	return _int();
}

void CBulletPlayer::Render_GameObject(void)
{
}

void CBulletPlayer::OnCollisionEnter(const Collision * collision)
{
}

void CBulletPlayer::OnCollisionStay(const Collision * collision)
{
}

void CBulletPlayer::OnCollisionExit(const Collision * collision)
{
}

void CBulletPlayer::Set_Die()
{
}

void CBulletPlayer::Key_Input(const _float & fTimeDelta)
{
}

HRESULT CBulletPlayer::Add_Component(void)
{
	return E_NOTIMPL;
}

CBulletPlayer * CBulletPlayer::Create(LPDIRECT3DDEVICE9 pGraphicDev, _vec3 & vPos)
{
	return nullptr;
}

void CBulletPlayer::Free(void)
{
}
