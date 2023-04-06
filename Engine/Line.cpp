#include "stdafx.h"
#include "Line.h"

#include "Export_Function.h"

CLine::CLine(LPDIRECT3DDEVICE9 pGraphicDev)
	:CComponent(pGraphicDev)
	, m_pLine(nullptr)
	, m_fWidth(0.f)
	, m_iCnt(0)
{
	ZeroMemory(&m_vecLine, sizeof(_vec3) * 2);
}

CLine::CLine(const CLine & rhs)
	:CComponent(rhs)
	, m_pLine(rhs.m_pLine)
	,m_fWidth(rhs.m_fWidth)
	, m_iCnt(rhs.m_iCnt)
{
	memcpy(&m_vecLine, &rhs.m_vecLine, sizeof(_vec3) * 2);
}


CLine::~CLine()
{
}

HRESULT CLine::Ready_Line(_float width)
{
	D3DXCreateLine(m_pGraphicDev, &m_pLine);
	m_fWidth = width;
	m_Color = D3DXCOLOR(0.f, 0.f, 0.f, 1.f);
	return S_OK;
}

void CLine::Set_Line(const _vec3 & first, const _vec3 & second, D3DXCOLOR color)
{
	m_vecLine[0] = first;
	m_vecLine[1] = second;
	m_Color = color;
}

void CLine::Draw_Line(const _matrix* matLine)
{
	m_pLine->SetWidth(m_fWidth);
	m_pLine->Begin();
	m_pLine->DrawTransform(m_vecLine, 2, matLine, m_Color);

	m_pLine->End();
}

CLine * CLine::Create(LPDIRECT3DDEVICE9 pGraphicDev, _float width)
{
	CLine *	pInstance = new CLine(pGraphicDev);

	if (FAILED(pInstance->Ready_Line(width)))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

CComponent * CLine::Clone(void)
{
	return new CLine(*this);
}

void CLine::Free()
{
	__super::Free();
}
