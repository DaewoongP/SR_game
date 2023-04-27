#pragma once
#include "ProduceObject.h"
class CPDBackCode :
	public CProduceObject
{
private:
	explicit CPDBackCode(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CPDBackCode();

public:
	virtual HRESULT		Ready_GameObject(void);
	virtual _int		Update_GameObject(const _float& fTimeDelta);
	virtual void		Render_GameObject(void);
private:
	_int iTextureIndex;
	_bool m_bPlaySound;
public:
	static CPDBackCode* Create(LPDIRECT3DDEVICE9 pGraphicDev);
	HRESULT Add_Component();
private:
	virtual void Free();
};

