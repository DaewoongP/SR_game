#include "stdafx.h"
#include "Key.h"

#include"..\Engine\Export_Function.h"
CKey::CKey(LPDIRECT3DDEVICE9 pGraphicDev) : CGameObject(pGraphicDev)
{
}

CKey::~CKey()
{
}

HRESULT CKey::Ready_GameObject(void)
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	m_pTransform->m_vScale = { 0.6f,0.6f,1.f };
	
	m_pTransform->m_vInfo[INFO_POS] = _vec3{ 30.f,5.f,11.f };
	m_pTransform->m_bIsStatic = false;
	return S_OK;
}

_int CKey::Update_GameObject(const _float& fTimeDelta)
{	
	if (m_bDead)
		return OBJ_DEAD;

	__super::Update_GameObject(fTimeDelta);
	Engine::Add_RenderGroup(RENDER_ALPHA, this);

	return 0;
}

_int CKey::Update_Too(const _float & fTimeDelta)
{
	m_pTransform->m_vInfo[INFO_POS] = _vec3{ 30.f,5.f,10.f };
	return 0;
}

_int CKey::Update_Top(const _float & fTimeDelta)
{
	m_pTransform->m_vInfo[INFO_POS] = _vec3{ 30.f,5.f,11.f };
	return 0;
}

void CKey::LateUpdate_GameObject(void)
{
	__super::LateUpdate_GameObject();
}

void CKey::Render_GameObject(void)
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransform->Get_WorldMatrixPointer());

	m_pTextureCom->Set_Texture(0);

	m_pBufferCom->Render_Buffer();
	__super::Render_GameObject();
}

void CKey::OnCollisionEnter(const Collision* collision)
{
	m_bDead = true;
}

void CKey::OnCollisionStay(const Collision * collision)
{
}

HRESULT CKey::Add_Component(void)
{
	CComponent* pComponent = nullptr;

	pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(Engine::Clone_Proto(L"RcTex", this));
	NULL_CHECK_RETURN(m_pBufferCom, E_FAIL);
	m_uMapComponent[ID_STATIC].insert({ L"RcTex" , pComponent});

	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Key_Texture", this));
	NULL_CHECK_RETURN(m_pTextureCom, E_FAIL);
	m_uMapComponent[ID_STATIC].insert({ L"Texture",pComponent });
	m_pTextureCom->Add_Anim(L"Idle", 0, 11, 1.f, true);
	m_pTextureCom->Switch_Anim(L"Idle");

	pComponent = m_pCollider = dynamic_cast<CCollider*>(Engine::Clone_Proto(L"Collider", this));
	NULL_CHECK_RETURN(m_pCollider, E_FAIL);
	m_uMapComponent[ID_DYNAMIC].insert({ L"Collider",pComponent });
	m_pCollider->Set_BoundingBox({1.f, 1.f, 0.2f});

	return S_OK;
}

CKey* CKey::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CKey* pInstance = new CKey(pGraphicDev);

	if (FAILED(pInstance->Ready_GameObject()))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
	
}

void CKey::Free(void)
{
	__super::Free();
}
