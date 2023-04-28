#pragma once
#include "VIBuffer.h"

BEGIN(Engine)

struct VertexPoints
{
	_vec3 p1;
	_vec3 p2;
	_vec3 p3;
};

class  CTerrainTex : public CVIBuffer
{
private:
	explicit CTerrainTex(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CTerrainTex(const CTerrainTex& rhs);
	virtual ~CTerrainTex();

public:
	HRESULT		Ready_Buffer(const _ulong& dwCntX, const _ulong& dwCntZ, const _ulong& dwVtxItv );
	void		Render_Buffer(void);
	bool IsInPlane(const _vec3& playerPos, const int& iIndex);

	vector<D3DXPLANE>	m_PlaneVec;
	vector<VertexPoints> m_VertexPoints;

private:
	HANDLE				m_hFile;
	BITMAPFILEHEADER	m_fH;
	BITMAPINFOHEADER	m_iH;

public:
	static CTerrainTex*		Create(LPDIRECT3DDEVICE9 pGraphicDev, const _ulong& dwCntX = VTXCNTX, const _ulong& dwCntZ = VTXCNTZ, const _ulong& dwVtxItv = VTXITV);
	virtual CComponent*	Clone(void);
private:
	virtual void Free(void);

};

END