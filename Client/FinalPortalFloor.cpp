#include "stdafx.h"
#include "FinalPortalFloor.h"
#include "PreStage.h"

CFinalPortalFloor::CFinalPortalFloor(LPDIRECT3DDEVICE9 pGraphicDev)
	:CGameObject(pGraphicDev)
{
}

CFinalPortalFloor::~CFinalPortalFloor()
{
}

HRESULT CFinalPortalFloor::Ready_GameObject(_vec3& vPos)
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);
	m_pTransform->m_vScale *= 4.1f;
	m_pTransform->m_vInfo[INFO_POS] = vPos;
	m_pTransform->m_vInfo[INFO_POS].z += 3.f;
	m_pPortal->Add_Anim(L"Idle", 0, 9, 1.f, true);
	m_pPortal->Switch_Anim(L"Idle");
	m_pPortal->m_bUseFrameAnimation = true;
	return S_OK;
}

_int CFinalPortalFloor::Update_GameObject(const _float & fTimeDelta)
{
	Engine::Add_RenderGroup(RENDER_ALPHA, this);
	m_pPortal->Update_Anim(fTimeDelta);
	__super::Update_GameObject(fTimeDelta);
	return 0;
}

void CFinalPortalFloor::Render_GameObject(void)
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransform->Get_WorldMatrixPointer());

	m_pPortal->Set_Texture();
	m_pBufferCom->Render_Buffer();

	__super::Render_GameObject();
}

HRESULT CFinalPortalFloor::Add_Component(void)
{
	CComponent*		pComponent = nullptr;

	pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(Engine::Clone_Proto(L"RcTex", this));
	NULL_CHECK_RETURN(m_pBufferCom, E_FAIL);
	m_vecComponent[ID_STATIC].push_back({ L"RcTex" , pComponent });

	pComponent = m_pPortal = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"FinalPortal_Floor", this));
	NULL_CHECK_RETURN(m_pPortal, E_FAIL);
	m_vecComponent[ID_STATIC].push_back({ L"FinalPortal_Floor", pComponent });

	return S_OK;
}

CFinalPortalFloor * CFinalPortalFloor::Create(LPDIRECT3DDEVICE9 pGraphicDev, _vec3 vPos)
{
	CFinalPortalFloor*		pInstance = new CFinalPortalFloor(pGraphicDev);

	if (FAILED(pInstance->Ready_GameObject(vPos)))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CFinalPortalFloor::Free(void)
{
	__super::Free();
}
