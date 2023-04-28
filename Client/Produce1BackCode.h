#pragma once
#include "ProduceObject.h"
class CProduce1BackCode :
	public CProduceObject
{
private:
	explicit CProduce1BackCode(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CProduce1BackCode();

public:
	virtual HRESULT		Ready_GameObject(void);
	virtual _int		Update_GameObject(const _float& fTimeDelta);
	virtual void		Render_GameObject(void);
private:
	_int iTextureIndex;
public:
	static CProduce1BackCode* Create(LPDIRECT3DDEVICE9 pGraphicDev);
	HRESULT Add_Component();
private:
	virtual void Free();
};

