#include "stdafx.h"
#include "RcGradation.h"


CRcGradation::CRcGradation(LPDIRECT3DDEVICE9 pGraphicDev)
	: CVIBuffer(pGraphicDev)
{
}

CRcGradation::CRcGradation(const CRcGradation & rhs)
	: CVIBuffer(rhs)
{
}

CRcGradation::~CRcGradation()
{
}

HRESULT CRcGradation::Ready_Buffer(void)
{
	m_dwFVF = FVF_COL;
	m_dwVtxSize = sizeof(VTXCOL);
	m_dwVtxCnt = 4;
	m_dwTriCnt = 2;

	m_dwIdxSize = sizeof(INDEX32);
	m_IdxFmt = D3DFMT_INDEX32;

	FAILED_CHECK_RETURN(__super::Ready_Buffer(), E_FAIL);

	VTXCOL*		pVertex = nullptr;

	m_pVB->Lock(0, 0, (void**)&pVertex, 0);
	// 버텍스 버퍼 메모리 공간의 접근을 막는 행위, 3번 인자는 버텍스 버퍼에 저장된 버텍스들 중 첫 번째 버텍스의 주소를 반환한다.

	pVertex[0].vPos = { -1.f, 1.f, 0.f };
	pVertex[0].dwColor = D3DXCOLOR(0.f, 12.0f / 255.0f, 50.0f / 255.0f, 1.f);

	pVertex[1].vPos = { 1.f, 1.f, 0.f };
	pVertex[1].dwColor = D3DXCOLOR(0.f, 12.0f / 255.0f, 50.0f / 255.0f, 1.f);

	pVertex[2].vPos = { 1.f, -1.f, 0.f };
	pVertex[2].dwColor = D3DXCOLOR(0.f, 12.0f / 255.0f, 150.0f / 255.0f, 1.f);

	pVertex[3].vPos = { -1.f, -1.f, 0.f };
	pVertex[3].dwColor = D3DXCOLOR(0.f, 12.0f / 255.0f, 150.0f / 255.0f, 1.f);

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

void CRcGradation::Render_Buffer(void)
{
	__super::Render_Buffer();
}

CRcGradation * CRcGradation::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CRcGradation *	pInstance = new CRcGradation(pGraphicDev);

	if (FAILED(pInstance->Ready_Buffer()))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}
CComponent * CRcGradation::Clone(void)
{
	return new CRcGradation(*this);
}

void CRcGradation::Free(void)
{
	__super::Free();
}

