#include "stdafx.h"
#include "TerrainTex.h"


CTerrainTex::CTerrainTex(LPDIRECT3DDEVICE9 pGraphicDev)
	: CVIBuffer(pGraphicDev)
{
}

CTerrainTex::CTerrainTex(const CTerrainTex & rhs)
	: CVIBuffer(rhs)
	, m_fH(rhs.m_fH)
	, m_iH(rhs.m_iH)
	, m_hFile(rhs.m_hFile)
	, m_PlaneVec(rhs.m_PlaneVec)
	, m_VertexPoints(rhs.m_VertexPoints)
{
}

CTerrainTex::~CTerrainTex()
{
}

HRESULT CTerrainTex::Ready_Buffer(const _ulong& dwCntX, 
	const _ulong& dwCntZ, 
	const _ulong& dwVtxItv)
{

	/*LPDIRECT3DTEXTURE9	pTexture = nullptr;

	D3DXCreateTexture(m_pGraphicDev, 129, 129, 1, 0, D3DFMT_A8R8G8B8, D3DPOOL_MANAGED, &pTexture);

	D3DLOCKED_RECT		LockRect;

	pTexture->LockRect(0, &LockRect, NULL, 0);

	for (_int i = 0; i < 129; ++i)
	{
		for (_int j = 0; j < 129; ++j)
		{
			_int		iIndex = i * 129 + j;

			*(((_ulong*)LockRect.pBits) + iIndex) = D3DCOLOR_ARGB(255, 255, 0, 0);
		}
	}
	
	pTexture->UnlockRect(0);
	
	D3DXSaveTextureToFile(L"../Bin/Resource/Texture/Terrain/MyHeight.bmp", D3DXIFF_BMP, pTexture, nullptr);*/
	
	m_dwFVF = FVF_TEX;
	m_dwVtxSize = sizeof(VTXTEX);
	m_dwVtxCnt = dwCntX * dwCntZ;
	m_dwTriCnt = (dwCntX - 1) * (dwCntZ - 1) * 2;

	m_PlaneVec.reserve(m_dwTriCnt);
	m_VertexPoints.reserve(m_dwTriCnt);

	m_dwIdxSize = sizeof(INDEX32);
	m_IdxFmt = D3DFMT_INDEX32;

	FAILED_CHECK_RETURN(__super::Ready_Buffer(), E_FAIL);


	m_hFile = CreateFile(L"../Resource/Texture/Terrain/Height1.bmp", GENERIC_READ, 0, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);

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


	return S_OK;
}

void CTerrainTex::Render_Buffer(void)
{
	
	__super::Render_Buffer();
}

CTerrainTex * CTerrainTex::Create(LPDIRECT3DDEVICE9 pGraphicDev, const _ulong& dwCntX, const _ulong& dwCntZ, const _ulong& dwVtxItv)
{
	CTerrainTex *	pInstance = new CTerrainTex(pGraphicDev);

	if (FAILED(pInstance->Ready_Buffer(dwCntX, dwCntZ, dwVtxItv)))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}
CComponent * CTerrainTex::Clone(void)
{
	return new CTerrainTex(*this);
}

void CTerrainTex::Free(void)
{
	__super::Free();
}

bool CTerrainTex::IsInPlane(const _vec3& playerPos, const int& iIndex)
{
	_vec3		vPoint[3] = {

		m_VertexPoints[iIndex].p1,
		m_VertexPoints[iIndex].p2,
		m_VertexPoints[iIndex].p3
	};

	_vec3		vDir[3] = {

		vPoint[1] - vPoint[0],
		vPoint[2] - vPoint[1],
		vPoint[0] - vPoint[2],
	};

	_vec3		vNormal[3] = {

		{ -vDir[0].z , 0.f, vDir[0].x },
		{ -vDir[1].z , 0.f, vDir[1].x },
		{ -vDir[2].z , 0.f, vDir[2].x },
	};

	_vec3		vMouseDir[3] = {
		playerPos - vPoint[0],
		playerPos - vPoint[1],
		playerPos - vPoint[2]
	};

	for (int i = 0; i < 3; ++i)
	{
		D3DXVec3Normalize(&vMouseDir[i], &vMouseDir[i]);
		D3DXVec3Normalize(&vNormal[i], &vNormal[i]);
	}

	for (int i = 0; i < 3; ++i)
	{
		// 예각인 경우 타일 외부를 피킹한 것
		if (0.f < D3DXVec3Dot(&vNormal[i], &vMouseDir[i]))
			return false;
	}

	return true;
}
