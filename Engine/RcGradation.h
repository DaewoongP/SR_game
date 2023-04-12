#pragma once
#include "VIBuffer.h"

BEGIN(Engine)

class CRcGradation :
	public CVIBuffer
{
private:
	explicit CRcGradation(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CRcGradation(const CRcGradation& rhs);
	virtual ~CRcGradation();

public:
	HRESULT		Ready_Buffer(void);
	void		Render_Buffer(void);

public:
	static CRcGradation*		Create(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual CComponent*	Clone(void);
private:
	virtual void Free(void);
};

END