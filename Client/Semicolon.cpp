#include "stdafx.h"
#include "Semicolon.h"
#include "PreStage.h"

CSemicolon::CSemicolon(LPDIRECT3DDEVICE9 pGraphicDev)
	:CGameObject(pGraphicDev)
{
}

CSemicolon::~CSemicolon()
{
}

HRESULT CSemicolon::Ready_GameObject(_vec3& vPos)
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);
	m_pTransform->m_vScale = { 2.f, 4.f, 2.f };
	m_pTransform->m_vInfo[INFO_POS] = vPos;
	m_pSemicolon->Add_Anim(L"Idle", 0, 23, 1.f, true);
	m_pSemicolon->Switch_Anim(L"Idle");
	m_pSemicolon->m_bUseFrameAnimation = true;
	return S_OK;
}

_int CSemicolon::Update_GameObject(const _float & fTimeDelta)
{
	m_pTransform->m_vAngle.x = D3DXToRadian(-90);
	m_pTransform->m_vAngle.y = D3DXToRadian(90);
	// 직접 렌더스테이트 변경할거임.
	Engine::Add_RenderGroup(RENDER_ALPHA, this);
	m_pSemicolon->Update_Anim(fTimeDelta);
	__super::Update_GameObject(fTimeDelta);
	return 0;
}

void CSemicolon::Render_GameObject(void)
{
	PreRender();

	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransform->Get_WorldMatrixPointer());

	m_pSemicolon->Set_Texture();
	m_pBufferCom->Render_Buffer();

	__super::Render_GameObject();

	PostRender();
}

HRESULT CSemicolon::Add_Component(void)
{
	CComponent*		pComponent = nullptr;

	pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(Engine::Clone_Proto(L"RcTex", this));
	NULL_CHECK_RETURN(m_pBufferCom, E_FAIL);
	m_vecComponent[ID_STATIC].push_back({ L"RcTex" , pComponent });

	pComponent = m_pSemicolon = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Semicolon", this));
	NULL_CHECK_RETURN(m_pSemicolon, E_FAIL);
	m_vecComponent[ID_STATIC].push_back({ L"Semicolon", pComponent });

	return S_OK;
}

void CSemicolon::PreRender()
{
	m_pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	m_pGraphicDev->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	m_pGraphicDev->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
}

void CSemicolon::PostRender()
{
	m_pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
}

CSemicolon * CSemicolon::Create(LPDIRECT3DDEVICE9 pGraphicDev, _vec3 vPos)
{
	CSemicolon*		pInstance = new CSemicolon(pGraphicDev);

	if (FAILED(pInstance->Ready_GameObject(vPos)))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CSemicolon::Free(void)
{
	__super::Free();
}
