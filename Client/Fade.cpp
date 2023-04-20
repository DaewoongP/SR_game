#include "stdafx.h"
#include "Fade.h"
#include "PreStage.h"

CFade::CFade(LPDIRECT3DDEVICE9 pGraphicDev)
	:CGameObject(pGraphicDev), m_bReStart(false)
{
}

CFade::~CFade()
{
}

HRESULT CFade::Ready_GameObject(void)
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);
	m_fFadeSpeed = 70.f;
	m_pTransform->m_vScale *= 80.f;
	return S_OK;
}

_int CFade::Update_GameObject(const _float & fTimeDelta)
{
	if (1.5f >= m_pTransform->m_vScale.x && 0 < m_fFadeSpeed)
	{
		m_fFadeSpeed *= -1;
		m_bReStart = true;
	}
		
	m_pTransform->m_vScale -= _vec3(1.f, 1.f, 1.f) * fTimeDelta * m_fFadeSpeed;

	if (m_pTransform->m_vScale.x > 80.f)
		return STAGE_FADE;
	Engine::Add_RenderGroup(RENDER_UI, this);
	__super::Update_GameObject(fTimeDelta);
	return 0;
}

void CFade::Render_GameObject(void)
{
	m_pGraphicDev->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	m_pGraphicDev->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);
	m_pGraphicDev->SetRenderState(D3DRS_ALPHAREF, 0xc0);
	m_pGraphicDev->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	_matrix mat;
	D3DXMatrixIdentity(&mat);
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransform->Get_WorldMatrixPointer());
	m_pGraphicDev->SetTransform(D3DTS_VIEW, &mat);
	m_pGraphicDev->SetTransform(D3DTS_PROJECTION, &mat);
	m_pFadeTex->Set_Texture();
	m_pBufferCom->Render_Buffer();

	__super::Render_GameObject();

	m_pGraphicDev->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}

HRESULT CFade::Add_Component(void)
{
	CComponent*		pComponent = nullptr;

	pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(Engine::Clone_Proto(L"RcTex", this));
	NULL_CHECK_RETURN(m_pBufferCom, E_FAIL);
	m_vecComponent[ID_STATIC].push_back({ L"RcTex" , pComponent });

	pComponent = m_pFadeTex = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Fade_Texture", this));
	NULL_CHECK_RETURN(m_pFadeTex, E_FAIL);
	m_vecComponent[ID_STATIC].push_back({ L"Fade_Texture", pComponent });

	return S_OK;
}

CFade * CFade::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CFade*		pInstance = new CFade(pGraphicDev);

	if (FAILED(pInstance->Ready_GameObject()))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CFade::Free(void)
{
	__super::Free();
}
