#include "stdafx.h"
#include "FinalPortal.h"
#include "PreStage.h"
#include "FinalPortalFloor.h"

CFinalPortal::CFinalPortal(LPDIRECT3DDEVICE9 pGraphicDev)
	:CGameObject(pGraphicDev)
{
}

CFinalPortal::~CFinalPortal()
{
}

HRESULT CFinalPortal::Ready_GameObject(_vec3& vPos)
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);
	m_pTransform->m_vScale *= 4.f;
	m_pTransform->m_vInfo[INFO_POS] = vPos;
	return S_OK;
}

_int CFinalPortal::Update_GameObject(const _float & fTimeDelta)
{
	m_pTransform->m_vAngle.x = D3DXToRadian(-90);
	m_pTransform->m_vAngle.y = D3DXToRadian(90);
	// 직접 렌더스테이트 변경할거임.
	Engine::Add_RenderGroup(RENDER_NONALPHA, this);
	__super::Update_GameObject(fTimeDelta);
	return 0;
}

void CFinalPortal::Render_GameObject(void)
{
	PreRender();

	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransform->Get_WorldMatrixPointer());

	m_pPortal->Set_Texture(0);
	m_pBufferCom->Render_Buffer();

	__super::Render_GameObject();

	PostRender();
}

HRESULT CFinalPortal::Add_Component(void)
{
	CComponent*		pComponent = nullptr;

	pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(Engine::Clone_Proto(L"RcTex", this));
	NULL_CHECK_RETURN(m_pBufferCom, E_FAIL);
	m_vecComponent[ID_STATIC].push_back({ L"RcTex" , pComponent });

	pComponent = m_pPortal = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"FinalPortal", this));
	NULL_CHECK_RETURN(m_pPortal, E_FAIL);
	m_vecComponent[ID_STATIC].push_back({ L"FinalPortal", pComponent });

	return S_OK;
}

void CFinalPortal::PreRender()
{
	m_pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	m_pGraphicDev->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	m_pGraphicDev->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
}

void CFinalPortal::PostRender()
{
	m_pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
}

CFinalPortal * CFinalPortal::Create(LPDIRECT3DDEVICE9 pGraphicDev, _vec3 vPos, CLayer* pLayer)
{
	CFinalPortal*		pInstance = new CFinalPortal(pGraphicDev);

	if (FAILED(pInstance->Ready_GameObject(vPos)))
	{
		Safe_Release(pInstance);
		return nullptr;
	}
	CGameObject* pGameObject = CFinalPortalFloor::Create(pGraphicDev, vPos);
	pLayer->Add_GameObject(L"FinalPortalFloor", pGameObject);

	return pInstance;
}

void CFinalPortal::Free(void)
{
	__super::Free();
}
