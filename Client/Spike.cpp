#include "stdafx.h"
#include "Spike.h"

#include"..\Engine\Export_Function.h"
CSpike::CSpike(LPDIRECT3DDEVICE9 pGraphicDev)
	:CGameObject(pGraphicDev)
{

}

CSpike::~CSpike()
{
}

HRESULT CSpike::Ready_GameObject(_vec3& vPos)
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);
	m_pTransform->m_vInfo[INFO_POS] = { vPos.x, vPos.y-0.2f,vPos.z-0.5f};
	m_pTransform->m_vScale = { 1.f,1.f,1.f };
	m_pTransform->m_bIsStatic = false;
	
	m_pCollider->Set_Options({ 1.f, 0.8f, 2.f }, COL_ENV, true);
	m_pTextureCom->Add_Anim(L"Idle", 0, 0, 1.f, true);
	m_pTextureCom->Add_Anim(L"Goto2D", 7, 12, 0.3f, false);
	m_pTextureCom->Add_Anim(L"GotoTop", 0, 6, 0.3f, false);
	m_pTextureCom->Switch_Anim(L"Idle");
	m_pTextureCom->m_bUseFrameAnimation = true;
	return S_OK;
}

_int CSpike::Update_GameObject(const _float& fTimeDelta)
{
	if (m_bDead)
		return OBJ_DEAD;

	Engine::Add_RenderGroup(RENDER_ALPHA, this);
	__super::Update_GameObject(fTimeDelta);
	m_pTextureCom->Update_Anim(fTimeDelta);
	return 0;
}

_int CSpike::Update_Too(const _float& fTimeDelta)
{
	m_pTextureCom->Switch_Anim(L"Goto2D");
	if (!(m_pTransform->m_vInfo[INFO_POS].z == 9.5))
		m_pTransform->Set_Pos(_float(m_pTransform->m_vInfo[INFO_POS].x),
			_float(m_pTransform->m_vInfo[INFO_POS].y + 0.2),
			_float(m_pTransform->m_vInfo[INFO_POS].z + 0.125f));
	return S_OK;
}

_int CSpike::Update_Top(const _float& fTimeDelta)
{
	m_pTextureCom->Switch_Anim(L"GotoTop");
	if (!(m_pTransform->m_vInfo[INFO_POS].z == 8.5))
		m_pTransform->Set_Pos(_float(m_pTransform->m_vInfo[INFO_POS].x),
			_float(m_pTransform->m_vInfo[INFO_POS].y - 0.2),
			_float(m_pTransform->m_vInfo[INFO_POS].z - 0.125f));
	return S_OK;
}

void CSpike::LateUpdate_GameObject(void)
{
	__super::LateUpdate_GameObject();
}

void CSpike::Render_GameObject(void)
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransform->Get_WorldMatrixPointer());

	m_pTextureCom->Set_Texture(0);
	m_pShadow->Render_Shadow(m_pBufferCom);

	m_pBufferCom->Render_Buffer();
	__super::Render_GameObject();
}

void CSpike::OnCollisionEnter(const Collision* collision)
{
	/*if (collision->otherObj != nullptr)
		collision->otherObj->m_bDead = true;*/
	if (!lstrcmp(collision->otherObj->m_pTag, L"Pig"))
	{
		StopSound(SOUND_EFFECT_GIMMICK);
		PlaySound_Effect(L"54.wav", SOUND_EFFECT_GIMMICK, 1.f);
	}
	__super::OnCollisionEnter(collision);
}

void CSpike::OnCollisionExit(const Collision* collision)
{
	__super::OnCollisionExit(collision);
}

HRESULT CSpike::Add_Component(void)
{
	CComponent* pComponent = nullptr;

	pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(Engine::Clone_Proto(L"RcTex", this));
	NULL_CHECK_RETURN(m_pBufferCom, E_FAIL);
	m_vecComponent[ID_STATIC].push_back({ L"RcTex" , pComponent });

	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Spike_Texture", this));
	NULL_CHECK_RETURN(m_pTextureCom, E_FAIL);
	m_vecComponent[ID_STATIC].push_back({ L"Texture",pComponent });

	pComponent = m_pCollider = dynamic_cast<CCollider*>(Engine::Clone_Proto(L"Collider", this));
	NULL_CHECK_RETURN(m_pCollider, E_FAIL);
	m_vecComponent[ID_DYNAMIC].push_back({ L"Collider",pComponent });

	pComponent = m_pShadow = dynamic_cast<CShadow*>(Engine::Clone_Proto(L"Shadow", this));
	NULL_CHECK_RETURN(m_pShadow, E_FAIL);
	m_vecComponent[ID_STATIC].push_back({ L"Shadow", pComponent });

	return S_OK;
}

CSpike* CSpike::Create(LPDIRECT3DDEVICE9 pGraphicDev, _vec3& vPos)
{
	CSpike* pInstance = new CSpike(pGraphicDev);

	if (FAILED(pInstance->Ready_GameObject(vPos)))
	{
		Safe_Release(pInstance);
		return nullptr;
	}
	return pInstance;
}

void CSpike::Free(void)
{
	__super::Free();
}
