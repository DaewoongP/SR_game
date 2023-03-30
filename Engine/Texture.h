#pragma once

#include "Base.h"
#include "Component.h"

BEGIN(Engine)

class  CTexture : public CComponent
{
private:
	explicit CTexture(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CTexture(const CTexture& rhs);
	virtual ~CTexture();

public:
	HRESULT			Ready_Texture(TEXTYPE eTextype, const _tchar* pPath, const _uint& iCnt = 1);
	void			Set_Texture(const _uint& iIndex = 0);

private:
	vector<IDirect3DBaseTexture9*>		m_vecTexture;

public:
	static CTexture*		Create(LPDIRECT3DDEVICE9 pGraphicDev, TEXTYPE eTextype, const _tchar* pPath, const _uint& iCnt = 1);
	virtual CComponent*		Clone(void);

private:
	virtual void Free(void);
};

END