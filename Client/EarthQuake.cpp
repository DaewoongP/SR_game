#include "stdafx.h"
#include "EarthQuake.h"

CEarthQuake::CEarthQuake(LPDIRECT3DDEVICE9 pGraphicDev)
	:CGameObject(pGraphicDev)
{
}

CEarthQuake::~CEarthQuake()
{
}

HRESULT CEarthQuake::Ready_GameObject(_vec3 & vPos)
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);
	m_pCollider->m_bIsTrigger = true;

	return S_OK;
}

_int CEarthQuake::Update_GameObject(const _float & fTimeDelta)
{
	Engine::Add_RenderGroup(RENDER_ALPHA, this);
	__super::Update_GameObject(fTimeDelta);
	return 0;
}

_int CEarthQuake::Update_Too(const _float & fTimeDelta)
{
	return _int();
}

_int CEarthQuake::Update_Top(const _float & fTimeDelta)
{
	return _int();
}

void CEarthQuake::LateUpdate_GameObject(void)
{
}

void CEarthQuake::Render_GameObject(void)
{
}

void CEarthQuake::SwapTrigger()
{
}

void CEarthQuake::OnCollisionEnter(const Collision * collision)
{
}

void CEarthQuake::OnCollisionStay(const Collision * collision)
{
}

void CEarthQuake::OnCollisionExit(const Collision * collision)
{
}

HRESULT CEarthQuake::Add_Component(void)
{
	CComponent*		pComponent = nullptr;

	pComponent = m_pCollider = dynamic_cast<CCollider*>(Engine::Clone_Proto(L"Collider", this));
	NULL_CHECK_RETURN(m_pCollider, E_FAIL);
	m_vecComponent[ID_DYNAMIC].push_back({ L"Collider", pComponent });

	return S_OK;
}

CEarthQuake * CEarthQuake::Create(LPDIRECT3DDEVICE9 pGraphicDev, _vec3 & vPos)
{
	CEarthQuake*		pInstance = new CEarthQuake(pGraphicDev);

	if (FAILED(pInstance->Ready_GameObject(vPos)))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CEarthQuake::Free(void)
{
	__super::Free();
}
