#include "stdafx.h"
#include "ChapterCloudTex.h"
#include "PreStage.h"

CChapterCloudTex::CChapterCloudTex(LPDIRECT3DDEVICE9 pGraphicDev)
	:CGameObject(pGraphicDev)
{
}

CChapterCloudTex::~CChapterCloudTex()
{
}

HRESULT CChapterCloudTex::Ready_GameObject(void)
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);
	m_pChapterTex->Add_Anim(L"Idle", 0, 9, 1.f, true);
	m_pChapterTex->Switch_Anim(L"Idle");
	m_pChapterTex->m_bUseFrameAnimation = true;
	m_pTransform->m_vScale *= 110.f;
	m_pTransform->m_vInfo[INFO_POS] = { 0.f, 0.f , 1.5f };
	return S_OK;
}

_int CChapterCloudTex::Update_GameObject(const _float & fTimeDelta)
{
	Engine::Add_RenderGroup(RENDER_ALPHA, this);
	m_pChapterTex->Update_Anim(fTimeDelta);
	__super::Update_GameObject(fTimeDelta);
	return 0;
}

void CChapterCloudTex::Render_GameObject(void)
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransform->Get_WorldMatrixPointer());
	m_pChapterTex->Set_Texture(0);
	m_pBufferCom->Render_Buffer();

	__super::Render_GameObject();
}

HRESULT CChapterCloudTex::Add_Component(void)
{
	CComponent*		pComponent = nullptr;

	pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(Engine::Clone_Proto(L"RcTex", this));
	NULL_CHECK_RETURN(m_pBufferCom, E_FAIL);
	m_vecComponent[ID_STATIC].push_back({ L"RcTex" , pComponent });

	pComponent = m_pChapterTex = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"ChapterCloudTex", this));
	NULL_CHECK_RETURN(m_pChapterTex, E_FAIL);
	m_vecComponent[ID_STATIC].push_back({ L"ChapterCloudTex", pComponent });

	return S_OK;
}

CChapterCloudTex * CChapterCloudTex::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CChapterCloudTex*		pInstance = new CChapterCloudTex(pGraphicDev);

	if (FAILED(pInstance->Ready_GameObject()))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CChapterCloudTex::Free(void)
{
	__super::Free();
}
