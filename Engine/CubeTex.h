#pragma once
#include "VIBuffer.h"

BEGIN(Engine)

class CCubeTex : public CVIBuffer
{
private:
	explicit CCubeTex(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CCubeTex(const CCubeTex& rhs);
	virtual ~CCubeTex();

public:
	HRESULT		Ready_Buffer(void);
	void		Render_Buffer(void);

public:
	static CCubeTex*		Create(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual CComponent*	Clone(void);
private:
	virtual void Free(void);

};

END