#pragma once
#include "Component.h"

BEGIN(Engine)

class CVIBuffer;

class CShadow :
	public CComponent
{
private:
	explicit CShadow(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CShadow(const CShadow& rhs);
	virtual ~CShadow();

public:
	HRESULT		Ready_CComponent();
	//월드 행렬과 텍스처 결정 이후 사용
	void		Render_Shadow(CVIBuffer* VIBuffer);

public:
	static CShadow*		Create(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual CComponent * Clone(void) override;


private:
	virtual void Free() override;
};

END