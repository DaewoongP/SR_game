#include "stdafx.h"
#include "Water.h"

#include"Export_Function.h"
CWater::CWater(LPDIRECT3DDEVICE9 pGraphicDev) : CGameObject(pGraphicDev)
{
}

CWater::~CWater()
{
}

HRESULT CWater::Ready_GameObject(_vec3& vPos)
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);
	m_pTransform->m_vInfo[INFO_POS] = vPos;
	m_pTransform->m_vScale = { 1.f,1.f,1.f };
	m_pTransform->m_bIsStatic = false;

	m_pTextureCom->Add_Anim(L"Idle", 0, 21, 1.f, true);
	m_pTextureCom->Switch_Anim(L"Idle");
	m_pTextureCom->m_bUseFrameAnimation = true;
	return S_OK;
}

_int CWater::Update_GameObject(const _float& fTimeDelta)
{
	return _int();
}

void CWater::LateUpdate_GameObject(void)
{
}

void CWater::Render_GameObject(void)
{
}

void CWater::OnCollisionEnter(const Collision* collision)
{
}

void CWater::OnCollisionExit(const Collision* collision)
{
}

HRESULT CWater::Add_Component(void)
{
	CComponent* pComponent = nullptr;

	pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(Engine::Clone_Proto(L"RcTex", this));
	NULL_CHECK_RETURN(m_pBufferCom, E_FAIL);
	m_vecComponent[ID_STATIC].push_back({ L"RcTex" , pComponent });

	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Water_Texture", this));
	NULL_CHECK_RETURN(m_pTextureCom, E_FAIL);
	m_vecComponent[ID_STATIC].push_back({ L"Texture",pComponent });

	pComponent = m_pCollider = dynamic_cast<CCollider*>(Engine::Clone_Proto(L"Collider", this));
	NULL_CHECK_RETURN(m_pCollider, E_FAIL);
	m_vecComponent[ID_DYNAMIC].push_back({ L"Collider",pComponent });
	
	return S_OK;
}

CWater* CWater::Create(LPDIRECT3DDEVICE9 pGraphicDev, _vec3& vPos)
{
	CWater* pInstance = new CWater(pGraphicDev);

	if (FAILED(pInstance->Ready_GameObject(vPos)))
	{
		Safe_Release(pInstance);
		return nullptr;
	}
	return pInstance;
}

void CWater::Free(void)
{
	__super::Free();
}
