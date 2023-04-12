#pragma once
#include "VIBuffer.h"
BEGIN(Engine)

class CRcAlpha :
	public CVIBuffer
{
private:
	explicit CRcAlpha(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CRcAlpha(const CRcAlpha& rhs);
	virtual ~CRcAlpha();

public:
	HRESULT		Ready_Buffer(void);
	void		Render_Buffer(void);

public:
	static CRcAlpha*		Create(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual CComponent*	Clone(void);
private:
	virtual void Free(void);
};

END
