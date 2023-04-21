#pragma once
#include "VIBuffer.h"

struct VertexPoints
{
	_vec3 p1;
	_vec3 p2;
	_vec3 p3;
};

BEGIN(Engine)

class  CBossPatternTex : public CVIBuffer
{
private:
	explicit CBossPatternTex(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CBossPatternTex(const CBossPatternTex& rhs);
	virtual ~CBossPatternTex();

public:
	HRESULT		Ready_Buffer(
		const _tchar * pPath, 
		const _uint & iCntconst,
		const _ulong& dwCntX, 
		const _ulong& dwCntZ, 
		const _ulong& dwVtxItv );
	void		Render_Buffer(void);

	vector<D3DXPLANE>	m_PlaneVec;
	vector<VertexPoints> m_VertexPoints;
	vector<_tchar*>		m_vecFilePath;

private:
	HANDLE				m_hFile;
	BITMAPFILEHEADER	m_fH;
	BITMAPINFOHEADER	m_iH;
	

public:
	static CBossPatternTex*		Create(LPDIRECT3DDEVICE9 pGraphicDev, TEXTYPE eTextype, const _tchar * pPath, const _uint & iCnt, const _ulong& dwCntX = VTXCNTX, const _ulong& dwCntZ = VTXCNTZ, const _ulong& dwVtxItv = VTXITV);
	virtual CComponent*	Clone(void);
private:
	virtual void Free(void);

};

END