#include "stdafx.h"
#include "Renderer.h"

IMPLEMENT_SINGLETON(CRenderer)

CRenderer::CRenderer()
{
}

CRenderer::~CRenderer()
{
	Free();
}

void CRenderer::Add_RenderGroup(RENDERID eID, CGameObject * pGameObject)
{
	if (RENDER_END <= eID || nullptr == pGameObject)
		return;

	m_RenderGroup[eID].push_back(pGameObject);
	pGameObject->AddRef();
}

void CRenderer::Render_GameObject(LPDIRECT3DDEVICE9 & pGraphicDev)
{
	Render_None(pGraphicDev);
	Render_AlphaBlend(pGraphicDev);
	Render_Alpha(pGraphicDev);
	Render_NonAlpha(pGraphicDev);
	Render_UI(pGraphicDev);

	Clear_RenderGroup();
}

void CRenderer::Clear_RenderGroup(void)
{
	for (size_t i = 0; i < RENDER_END; ++i)
	{
		for_each(m_RenderGroup[i].begin(), m_RenderGroup[i].end(), CDeleteObj());
		m_RenderGroup[i].clear();
	}
}

void CRenderer::Render_None(LPDIRECT3DDEVICE9 & pGraphicDev)
{
	for (auto& iter : m_RenderGroup[RENDER_NONE])
	{
		iter->Render_GameObject();
		pGraphicDev->SetTexture(0, nullptr);
		if (g_Is2D)
			iter->Render_Too();
		else
			iter->Render_Top();
	}
}

void CRenderer::Render_AlphaBlend(LPDIRECT3DDEVICE9 & pGraphicDev)
{
	pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	pGraphicDev->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pGraphicDev->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	m_RenderGroup[RENDER_ALPHABLEND].sort([](CGameObject* pSrc, CGameObject* pDest)->_bool
	{
		if (!lstrcmp(pSrc->m_pTag, pDest->m_pTag))
			return false;
		return !lstrcmp(pSrc->m_pTag, L"InstallGrid");
	});
	for (auto& iter : m_RenderGroup[RENDER_ALPHABLEND])
	{
		iter->Render_GameObject();
		pGraphicDev->SetTexture(0, nullptr);
		if (g_Is2D)
			iter->Render_Too();
		else
			iter->Render_Top();
	}
	pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
}

void CRenderer::Render_NonAlpha(LPDIRECT3DDEVICE9 & pGraphicDev)
{
	for (auto& iter : m_RenderGroup[RENDER_NONALPHA])
	{
		iter->Render_GameObject();
		pGraphicDev->SetTexture(0, nullptr);
		if (g_Is2D)
			iter->Render_Too();
		else
			iter->Render_Top();
	}
}	

void CRenderer::Render_Alpha(LPDIRECT3DDEVICE9 & pGraphicDev)
{
	pGraphicDev->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	pGraphicDev->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);
	pGraphicDev->SetRenderState(D3DRS_ALPHAREF, 0xc0);
	pGraphicDev->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
	pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	for (auto& iter : m_RenderGroup[RENDER_ALPHA])
	{
		iter->Render_GameObject();
		pGraphicDev->SetTexture(0, nullptr);
		if (g_Is2D)
		{
			iter->Render_Too();
		}
		else
		{
			iter->Render_Top();
		}
	}
	pGraphicDev->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
	pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}

void CRenderer::Render_UI(LPDIRECT3DDEVICE9 & pGraphicDev)
{
	pGraphicDev->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	pGraphicDev->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);
	pGraphicDev->SetRenderState(D3DRS_ALPHAREF, 0xc0);
	pGraphicDev->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
	pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	for (auto& iter : m_RenderGroup[RENDER_UI])
	{
		iter->Render_GameObject();
		pGraphicDev->SetTexture(0, nullptr);
		if (g_Is2D)
			iter->Render_Too();
		else
			iter->Render_Top();
	}	

	pGraphicDev->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
	pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}

void CRenderer::Free(void)
{
	Clear_RenderGroup();
}
