#pragma once
#include "ProduceObject.h"
class CPDGoal :
	public CProduceObject
{
private:
	explicit CPDGoal(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CPDGoal();

public:
	virtual HRESULT		Ready_GameObject(void);
	virtual _int		Update_GameObject(const _float& fTimeDelta);
	virtual void		Render_GameObject(void);
private:
	_int iTextureIndex;
	_bool m_bPlaySound;
public:
	static CPDGoal* Create(LPDIRECT3DDEVICE9 pGraphicDev);
	HRESULT Add_Component();
private:
	virtual void Free();
};

