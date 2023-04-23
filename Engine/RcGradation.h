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
	HRESULT		Ready_Buffer(D3DXCOLOR upColer, D3DXCOLOR downColer);
	void		Render_Buffer(void);

public:
	static CRcGradation*		Create(LPDIRECT3DDEVICE9 pGraphicDev, D3DXCOLOR upColer, D3DXCOLOR downColer);
	virtual CComponent*	Clone(void);
private:
	virtual void Free(void);
};

END