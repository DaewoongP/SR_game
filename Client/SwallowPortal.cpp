#include "stdafx.h"
#include "SwallowPortal.h"

#include"Export_Function.h"

CSwallowPortal::CSwallowPortal(LPDIRECT3DDEVICE9 pGraphicDev)
	:CGameObject(pGraphicDev)
{
}

CSwallowPortal::~CSwallowPortal()
{
}

HRESULT CSwallowPortal::Ready_GameObject(_vec3 & vPos)
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	m_pTransform->m_vScale = { 6.f, 6.f, 1.f };
	m_pTransform->m_vInfo[INFO_POS] = vPos;
	m_pTransform->m_bIsStatic = false;

	m_pTextureCom->Add_Anim(L"Idle_Swallow", 0, 16, 1.f, false);
	m_pTextureCom->Switch_Anim(L"Idle_Swallow");
	m_pTextureCom->m_bUseFrameAnimation = true;

	return S_OK;
}

_int CSwallowPortal::Update_GameObject(const _float & fTimeDelta)
{
	if (m_bDead)
		return OBJ_DEAD;

	__super::Update_GameObject(fTimeDelta);

	Engine::Add_RenderGroup(RENDER_ALPHA, this);
	m_pTextureCom->Update_Anim(fTimeDelta);

	return 0;
}

void CSwallowPortal::LateUpdate_GameObject(void)
{
	__super::LateUpdate_GameObject();
}

void CSwallowPortal::Render_GameObject(void)
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransform->Get_WorldMatrixPointer());

	m_pTextureCom->Set_Texture(0);

	m_pBufferCom->Render_Buffer();

	__super::Render_GameObject();
}

HRESULT CSwallowPortal::Add_Component(void)
{
	CComponent* pComponent = nullptr;

	pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(Engine::Clone_Proto(L"RcTex", this));
	NULL_CHECK_RETURN(m_pBufferCom, E_FAIL);
	m_uMapComponent[ID_STATIC].insert({ L"RcTex" , pComponent });

	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Swallow_Portal_Texture", this));
	NULL_CHECK_RETURN(m_pTextureCom, E_FAIL);
	m_uMapComponent[ID_STATIC].insert({ L"Texture",pComponent });

	return S_OK;
}

CSwallowPortal * CSwallowPortal::Create(LPDIRECT3DDEVICE9 pGraphicDev, _vec3 & vPos)
{
	CSwallowPortal* pInstance = new CSwallowPortal(pGraphicDev);

	if (FAILED(pInstance->Ready_GameObject(vPos)))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CSwallowPortal::Free(void)
{
	__super::Free();
}
