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
	void		Render_Shadow(CVIBuffer* VIBuffer,_float x,_float y,_float z);

	void		Off() 
	{
		m_bUseOutLine = false;
		m_bUseShadow = false;
	}
public:
	static CShadow*		Create(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual CComponent * Clone(void) override;


	_bool	m_bUseOutLine;
	_float	m_fOutLineHeight;
	_float	m_fOutLineScale;


	_bool	m_bUseShadow;
	_bool	m_fShadowScale;
	_float	m_fShadowHeight;


private:
	virtual void Free() override;
};

END