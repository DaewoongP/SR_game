#include "stdafx.h"
#include "RcAlpha.h"

CRcAlpha::CRcAlpha(LPDIRECT3DDEVICE9 pGraphicDev)
	: CVIBuffer(pGraphicDev)
{
}

CRcAlpha::CRcAlpha(const CRcAlpha & rhs)
	: CVIBuffer(rhs)
{
}

CRcAlpha::~CRcAlpha()
{
}

HRESULT CRcAlpha::Ready_Buffer(void)
{

	m_dwFVF = FVF_TEX;
	m_dwVtxSize = sizeof(VTXTEX);
	m_dwVtxCnt = 4;
	m_dwTriCnt = 2;

	m_dwIdxSize = sizeof(INDEX32);
	m_IdxFmt = D3DFMT_INDEX32;

	FAILED_CHECK_RETURN(__super::Ready_Buffer(), E_FAIL);

	VTXTEX*		pVertex = nullptr;

	m_pVB->Lock(0, 0, (void**)&pVertex, 0);
	// 버텍스 버퍼 메모리 공간의 접근을 막는 행위, 3번 인자는 버텍스 버퍼에 저장된 버텍스들 중 첫 번째 버텍스의 주소를 반환한다.

	pVertex[0].vPos = { -1.f, 1.f, 0.f };
	pVertex[0].dwColor = D3DXCOLOR(0.5f, 0.5f, 0.5f, 0.1f);
	pVertex[0].vTexUV = { 0.f, 0.f };

	pVertex[1].vPos = { 1.f, 1.f, 0.f };
	pVertex[1].dwColor = D3DXCOLOR(0.5f, 0.5f, 0.5f, 0.1f);
	pVertex[1].vTexUV = { 1.f, 0.f };

	pVertex[2].vPos = { 1.f, -1.f, 0.f };
	pVertex[2].dwColor = D3DXCOLOR(0.5f, 0.5f, 0.5f, 0.1f);
	pVertex[2].vTexUV = { 1.f, 1.f };

	pVertex[3].vPos = { -1.f, -1.f, 0.f };
	pVertex[3].dwColor = D3DXCOLOR(0.5f, 0.5f, 0.5f, 0.1f);
	pVertex[3].vTexUV = { 0.f, 1.f };

	m_pVB->Unlock();

	INDEX32*		pIndex = nullptr;

	m_pIB->Lock(0, 0, (void**)&pIndex, 0);

	pIndex[0]._0 = 0;
	pIndex[0]._1 = 1;
	pIndex[0]._2 = 2;

	pIndex[1]._0 = 0;
	pIndex[1]._1 = 2;
	pIndex[1]._2 = 3;

	m_pIB->Unlock();

	return S_OK;
}

void CRcAlpha::Render_Buffer(void)
{
	__super::Render_Buffer();
}

CRcAlpha * CRcAlpha::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CRcAlpha *	pInstance = new CRcAlpha(pGraphicDev);

	if (FAILED(pInstance->Ready_Buffer()))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}
CComponent * CRcAlpha::Clone(void)
{
	return new CRcAlpha(*this);
}

void CRcAlpha::Free(void)
{
	__super::Free();
}
