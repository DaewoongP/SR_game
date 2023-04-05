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
	Render_Priority(pGraphicDev);
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

void CRenderer::Render_Priority(LPDIRECT3DDEVICE9 & pGraphicDev)
{
	for (auto& iter : m_RenderGroup[RENDER_PRIORITY])
	{
		iter->Render_GameObject();
		if (g_Is2D)
			iter->Render_Too();
		else
			iter->Render_Top();
	}

}

void CRenderer::Render_NonAlpha(LPDIRECT3DDEVICE9 & pGraphicDev)
{
	for (auto& iter : m_RenderGroup[RENDER_NONALPHA])
	{
		iter->Render_GameObject();
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

	for (auto& iter : m_RenderGroup[RENDER_ALPHA])
	{
		iter->Render_GameObject();
		if (g_Is2D)
		{
			pGraphicDev->SetTexture(0, nullptr);
			iter->Render_Too();
		}
		else
		{
			pGraphicDev->SetTexture(0, nullptr);
			iter->Render_Top();
		}
	}

	pGraphicDev->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
}

void CRenderer::Render_UI(LPDIRECT3DDEVICE9 & pGraphicDev)
{
	for (auto& iter : m_RenderGroup[RENDER_UI])
	{
		iter->Render_GameObject();
		if (g_Is2D)
			iter->Render_Too();
		else
			iter->Render_Top();
	}
}

void CRenderer::Free(void)
{
	Clear_RenderGroup();
}
