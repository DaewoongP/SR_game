#pragma once
#include "ProduceObject.h"
class CPDErroe :
	public CProduceObject
{
private:
	explicit CPDErroe(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CPDErroe();

public:
	virtual HRESULT		Ready_GameObject(void);
	virtual _int		Update_GameObject(const _float& fTimeDelta);
	virtual void		Render_GameObject(void);
private:
	_int iTextureIndex;
	_bool m_bPlaySound;
public:
	static CPDErroe* Create(LPDIRECT3DDEVICE9 pGraphicDev);
	HRESULT Add_Component();
private:
	virtual void Free();
};

