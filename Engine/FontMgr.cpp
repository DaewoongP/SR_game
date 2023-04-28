#include "stdafx.h"
#include "FontMgr.h"

#include "Export_Function.h"

IMPLEMENT_SINGLETON(CFontMgr)

CFontMgr::CFontMgr()
{
}


CFontMgr::~CFontMgr()
{
	Free();
}

HRESULT CFontMgr::Ready_Font(LPDIRECT3DDEVICE9 pGraphicDev, const _tchar * pFontTag, const _tchar * pFontType, const _uint & iWidth, const _uint & iHeight, const _uint & iWeight)
{
	CMyFont*	pMyFont = Find_Font(pFontTag);

	if (nullptr != pMyFont)
		return E_FAIL;

	pMyFont = CMyFont::Create(pGraphicDev, pFontType, iWidth, iHeight, iWeight);
	NULL_CHECK_RETURN(pMyFont, E_FAIL);

	m_umapFont.insert({ pFontTag, pMyFont });

	return S_OK;
}

void CFontMgr::Render_Font(const _tchar * pFontTag, const _tchar * pString, const _vec2 * pPos, D3DXCOLOR Color)
{
	CMyFont*	pMyFont = Find_Font(pFontTag);
	NULL_CHECK(pMyFont);

	pMyFont->Render_Font(pString, pPos, Color);
}

CMyFont * CFontMgr::Find_Font(const _tchar * pFontTag)
{
	auto	iter = find_if(m_umapFont.begin(), m_umapFont.end(), CTag_Finder(pFontTag));

	if (iter == m_umapFont.end())
		return nullptr;

	return iter->second;
}

void CFontMgr::Free(void)
{
	for_each(m_umapFont.begin(), m_umapFont.end(), CDeleteMap());
	m_umapFont.clear();
}
