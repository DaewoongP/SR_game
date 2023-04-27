#pragma once
#include "ProduceObject.h"
class CProduceToodoo :
	public CProduceObject
{
private:
	explicit CProduceToodoo(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CProduceToodoo();

public:
	virtual HRESULT		Ready_GameObject(void);
	virtual _int		Update_GameObject(const _float& fTimeDelta);
	virtual void		Render_GameObject(void);
private:
public:
	static CProduceToodoo* Create(LPDIRECT3DDEVICE9 pGraphicDev);
	HRESULT Add_Component();

	_bool m_bPlaySound;

private:
	virtual void Free();
};

