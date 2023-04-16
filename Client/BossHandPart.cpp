#include "stdafx.h"
#include "BossHandPart.h"

CBossHandPart::CBossHandPart(LPDIRECT3DDEVICE9 pGraphicDev) : CGameObject(pGraphicDev)
{
}

CBossHandPart::~CBossHandPart()
{
}

HRESULT CBossHandPart::Ready_GameObject(_vec3& vPos)
{
	return E_NOTIMPL;
}

_int CBossHandPart::Update_GameObject(const _float& fTimeDelta)
{
	return _int();
}

_int CBossHandPart::Update_Too(const _float& fTimeDelta)
{
	return _int();
}

_int CBossHandPart::Update_Top(const _float& fTimeDelta)
{
	return _int();
}

void CBossHandPart::LateUpdate_GameObject(void)
{
}

void CBossHandPart::Render_GameObject(void)
{
}

void CBossHandPart::OnCollisionEnter(const Collision* collision)
{
}

HRESULT CBossHandPart::Add_Component(void)
{

	return S_OK;
}

CBossHandPart* CBossHandPart::Create(LPDIRECT3DDEVICE9 pGraphicDev, _vec3& vPos)
{
	CBossHandPart* pInstance = new CBossHandPart(pGraphicDev);

	if (FAILED(pInstance->Ready_GameObject(vPos)))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CBossHandPart::Free(void)
{
	__super::Free();
}
