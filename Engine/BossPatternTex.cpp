#include "stdafx.h"
#include "BossPatternTex.h"


CBossPatternTex::CBossPatternTex(LPDIRECT3DDEVICE9 pGraphicDev)
	: CVIBuffer(pGraphicDev)
{
}

CBossPatternTex::CBossPatternTex(const CBossPatternTex & rhs)
	: CVIBuffer(rhs)
	, m_fH(rhs.m_fH)
	, m_iH(rhs.m_iH)
	, m_hFile(rhs.m_hFile)
	, m_PlaneVec(rhs.m_PlaneVec)
	, m_VertexPoints(rhs.m_VertexPoints)
{
}

CBossPatternTex::~CBossPatternTex()
{
}

HRESULT CBossPatternTex::Ready_Buffer(
	const _tchar * pPath, 
	const _uint & iCnt,
	const _ulong& dwCntX,
	const _ulong& dwCntZ, 
	const _ulong& dwVtxItv)
{
	//텍스쳐 벡터에 요청된 텍스쳐수만큼 할당한다.
	m_vecFilePath.reserve(iCnt);

	for (_uint i = 0; i < iCnt; ++i)
	{
		TCHAR	szFileName[256] = L"";
		wsprintf(szFileName, pPath, i);
		m_vecFilePath.push_back(szFileName);
	}

	//할당된 텍스쳐 0번부터 heightmap을 생성해줄거임.

	m_dwFVF = FVF_TEX;
	m_dwVtxSize = sizeof(VTXTEX);
	m_dwVtxCnt = dwCntX * dwCntZ;
	m_dwTriCnt = (dwCntX - 1) * (dwCntZ - 1) * 2;

	//요청된 텍스쳐 수만큼의 할당
	m_PlaneVec.reserve(m_dwTriCnt);
	m_VertexPoints.reserve(m_dwTriCnt);

	m_dwIdxSize = sizeof(INDEX32);
	m_IdxFmt = D3DFMT_INDEX32;

	FAILED_CHECK_RETURN(__super::Ready_Buffer(), E_FAIL);

	m_hFile = CreateFile(m_vecFilePath[0], GENERIC_READ, 0, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);


	_ulong dwByte = 0;

	ReadFile(m_hFile, &m_fH, sizeof(BITMAPFILEHEADER), &dwByte, nullptr);
	ReadFile(m_hFile, &m_iH, sizeof(BITMAPINFOHEADER), &dwByte, nullptr);

	_ulong*		pPixel = new _ulong[m_iH.biHeight * m_iH.biWidth];

	ReadFile(m_hFile, pPixel, sizeof(_ulong) * m_iH.biHeight * m_iH.biWidth, &dwByte, nullptr);

	CloseHandle(m_hFile);

	VTXTEX*		pVertex = nullptr;

	_ulong	dwIndex = 0;

	m_pVB->Lock(0, 0, (void**)&pVertex, 0);
	// 버텍스 버퍼 메모리 공간의 접근을 막는 행위, 3번 인자는 버텍스 버퍼에 저장된 버텍스들 중 첫 번째 버텍스의 주소를 반환한다.

	for (_ulong i = 0; i < dwCntZ; ++i)
	{
		for (_ulong j = 0; j < dwCntX; ++j)
		{
			dwIndex = i * dwCntX + j;
			//검정색이라면 알고있는 가장 근처의 흰색의 pos으로 붙여줄거임.

			pVertex[dwIndex].vPos = {
				_float(j * dwVtxItv),
				_float(pPixel[dwIndex] & 0x000000ff) / 20.f,
				_float(i * dwVtxItv) };

			pVertex[dwIndex].vTexUV = { _float(j) / (dwCntX - 1) * 20.f,
				_float(i) / (dwCntZ - 1) * 20.f };
		}
	}

	m_pVB->Unlock();



	Safe_Delete_Array(pPixel);

	INDEX32*		pIndex = nullptr;

	_ulong	dwTriCnt = 0;

	m_pIB->Lock(0, 0, (void**)&pIndex, 0);

	for (_ulong i = 0; i < dwCntZ - 1; ++i)
	{
		for (_ulong j = 0; j < dwCntX - 1; ++j)
		{
			dwIndex = i * dwCntX + j;

			pIndex[dwTriCnt]._0 = dwIndex + dwCntX;
			pIndex[dwTriCnt]._1 = dwIndex + dwCntX + 1;
			pIndex[dwTriCnt]._2 = dwIndex + 1;


			D3DXPLANE plane;
			D3DXPlaneFromPoints(&plane, &pVertex[pIndex[dwTriCnt]._0].vPos, &pVertex[pIndex[dwTriCnt]._1].vPos, &pVertex[pIndex[dwTriCnt]._2].vPos);

			m_PlaneVec.push_back(plane);
			m_VertexPoints.push_back({ pVertex[pIndex[dwTriCnt]._0].vPos, pVertex[pIndex[dwTriCnt]._1].vPos, pVertex[pIndex[dwTriCnt]._2].vPos });

			dwTriCnt++;

			pIndex[dwTriCnt]._0 = dwIndex + dwCntX;
			pIndex[dwTriCnt]._1 = dwIndex + 1;
			pIndex[dwTriCnt]._2 = dwIndex;

			D3DXPlaneFromPoints(&plane, &pVertex[pIndex[dwTriCnt]._0].vPos, &pVertex[pIndex[dwTriCnt]._1].vPos, &pVertex[pIndex[dwTriCnt]._2].vPos);

			m_PlaneVec.push_back(plane);
			m_VertexPoints.push_back({ pVertex[pIndex[dwTriCnt]._0].vPos, pVertex[pIndex[dwTriCnt]._1].vPos, pVertex[pIndex[dwTriCnt]._2].vPos });

			dwTriCnt++;
		}
	}

	m_pIB->Unlock();
}
	
	


void CBossPatternTex::Render_Buffer(void)
{
	__super::Render_Buffer();
}

CBossPatternTex * CBossPatternTex::Create(LPDIRECT3DDEVICE9 pGraphicDev, TEXTYPE eTextype, const _tchar * pPath, const _uint & iCnt,const _ulong& dwCntX, const _ulong& dwCntZ, const _ulong& dwVtxItv)
{
	CBossPatternTex *	pInstance = new CBossPatternTex(pGraphicDev);

	if (FAILED(pInstance->Ready_Buffer(pPath,iCnt,dwCntX, dwCntZ, dwVtxItv)))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}
CComponent * CBossPatternTex::Clone(void)
{
	return new CBossPatternTex(*this);
}

void CBossPatternTex::Free(void)
{
	__super::Free();
}