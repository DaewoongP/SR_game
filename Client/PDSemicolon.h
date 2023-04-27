#pragma once
#include "ProduceObject.h"
class CPDSemicolon :
	public CProduceObject
{
private:
	explicit CPDSemicolon(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CPDSemicolon();

public:
	virtual HRESULT		Ready_GameObject(void);
	virtual _int		Update_GameObject(const _float& fTimeDelta);
	virtual void		Render_GameObject(void);
private:
	_int iTextureIndex;
	_bool m_bPlaySound;
public:
	static CPDSemicolon* Create(LPDIRECT3DDEVICE9 pGraphicDev);
	HRESULT Add_Component();
private:
	virtual void Free();
};

