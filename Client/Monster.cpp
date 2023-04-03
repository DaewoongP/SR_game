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

HRESULT CMonster::Ready_GameObject(void)
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);
	m_pTransform->m_vScale = { 1.f, 1.f, 1.f };
	m_pTransform->m_vInfo[INFO_POS] = _vec3(10.f, 7.f, 10.f);

	return S_OK;
}
_int CMonster::Update_GameObject(const _float& fTimeDelta)
{
	__super::Update_GameObject(fTimeDelta);

	if (g_Is2D)
	{
		Updatae_2D(fTimeDelta);
	}
	else if (!g_Is2D)
	{
		Updatae_3D(fTimeDelta);
	}

	Engine::Add_RenderGroup(RENDER_NONALPHA, this);

	return 0;
}
void CMonster::LateUpdate_GameObject(void)
{
	__super::LateUpdate_GameObject();
}

void CMonster::Render_GameObject(void)
{
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransform->Get_WorldMatrixPointer());

	m_pBufferCom->Render_Buffer();

	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);

	__super::Render_GameObject();
}

void CMonster::OnCollisionEnter(const Collision * collision)
{
}

void CMonster::OnCollisionStay(const Collision * collision)
{
}

void CMonster::OnCollisionExit(const Collision * collision)
{
}

_int CMonster::Updatae_2D(const _float & fTimeDelta)
{
	return _int();
}

_int CMonster::Updatae_3D(const _float & fTimeDelta)
{
	return _int();
}

HRESULT CMonster::Add_Component(void)
{
	/*CComponent*		pComponent = nullptr;

	pComponent = m_pBufferCom = dynamic_cast<CTriCol*>(Engine::Clone_Proto(L"TriCol",this));
	NULL_CHECK_RETURN(m_pBufferCom, E_FAIL);
	m_uMapComponent[ID_STATIC].insert({ L"TriCol", pComponent });

	pComponent = m_pTransform = dynamic_cast<CTransform*>(Engine::Clone_Proto(L"Transform",this));
	NULL_CHECK_RETURN(m_pTransform, E_FAIL);
	m_uMapComponent[ID_STATIC].insert({ L"Transform", pComponent });

	pComponent = m_pCollider = dynamic_cast<CCollider*>(Engine::Clone_Proto(L"Collider", this));
	NULL_CHECK_RETURN(m_pCollider, E_FAIL);
	m_uMapComponent[ID_DYNAMIC].insert({ L"Monster_Collider", pComponent });*/
	return S_OK;
}



CMonster* CMonster::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CMonster*		pInstance = new CMonster(pGraphicDev);

	if (FAILED(pInstance->Ready_GameObject()))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CMonster::Free(void)
{
	__super::Free();
}

