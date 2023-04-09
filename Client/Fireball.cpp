#include "stdafx.h"
#include "Fireball.h"

#include"..\Engine\Export_Function.h"
CFireball::CFireball(LPDIRECT3DDEVICE9 pGraphicDev):CGameObject(pGraphicDev)
{

}

CFireball::~CFireball()
{
}

HRESULT CFireball::Ready_GameObject(_vec3& vPos)
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);
	m_pTransform->m_vInfo[INFO_POS] = { vPos.x, vPos.y,vPos.z };
	m_pTransform->m_vScale = {5.5f,3.f,3.f };
	m_pTransform->m_bIsStatic = false;

	m_pTextureCom->Add_Anim(L"Idle", 0, 8, 0.5f, true);
	m_pTextureCom->Switch_Anim(L"Idle");
	m_pTextureCom->m_bUseFrameAnimation = true;

	m_pCollider->Set_Options({ 8.5f, 3.8f, 4.f }, COL_ENV, true);

	return S_OK;

}

_int CFireball::Update_GameObject(const _float& fTimeDelta)
{
	if (m_bDead)
		return OBJ_DEAD;
	Engine::Add_RenderGroup(RENDER_ALPHA, this);
	__super::Update_GameObject(fTimeDelta);
	m_pTextureCom->Update_Anim(fTimeDelta);
	m_pTransform->m_vInfo[INFO_POS].x -= 0.5f;
	m_pTransform->m_vScale = { m_pTransform->m_vScale.x + 0.1f,
	m_pTransform->m_vScale.y,
	m_pTransform->m_vScale.z };
	return 0;
}

_int CFireball::Update_Too(const _float& fTimeDelta)
{

	return S_OK;
}

_int CFireball::Update_Top(const _float& fTimeDelta)
{
	return S_OK;
}

void CFireball::LateUpdate_GameObject(void)
{
	__super::LateUpdate_GameObject();

}

void CFireball::Render_GameObject(void)
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransform->Get_WorldMatrixPointer());

	m_pTextureCom->Set_Texture(0);
	m_pBufferCom->Render_Buffer();
	__super::Render_GameObject();
}

void CFireball::OnCollisionEnter(const Collision* collision)
{
	if (!lstrcmp(collision->otherObj->m_pTag, L"Toodee")&& (g_Is2D == true))
		m_bDead = true;
	if (!lstrcmp(collision->otherObj->m_pTag, L"Topdee")&& (g_Is2D == false))
		m_bDead = true;
	
}


HRESULT CFireball::Add_Component(void)
{
	CComponent* pComponent = nullptr;

	pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(Engine::Clone_Proto(L"RcTex", this));
	NULL_CHECK_RETURN(m_pBufferCom, E_FAIL);
	m_uMapComponent[ID_STATIC].insert({ L"RcTex" , pComponent });

	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Fireball_Texture", this));
	NULL_CHECK_RETURN(m_pTextureCom, E_FAIL);
	m_uMapComponent[ID_STATIC].insert({ L"Texture",pComponent });

	pComponent = m_pCollider = dynamic_cast<CCollider*>(Engine::Clone_Proto(L"Collider", this));
	NULL_CHECK_RETURN(m_pCollider, E_FAIL);
	m_uMapComponent[ID_DYNAMIC].insert({ L"Collider",pComponent });
	return S_OK;
}

CFireball* CFireball::Create(LPDIRECT3DDEVICE9 pGraphicDev, _vec3& vPos)
{
	CFireball* pInstance = new CFireball(pGraphicDev);

	if (FAILED(pInstance->Ready_GameObject(vPos)))
	{
		Safe_Release(pInstance);
		return nullptr;
	}
	return pInstance;
}

void CFireball::Free(void)
{
	__super::Free();
}
