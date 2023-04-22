#pragma once
#include "BackGroundBase.h"
class CTheme1_House :
	public CBackGroundBase
{
private:
	explicit
		CTheme1_House(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CTheme1_House();
public:
	virtual _int Update_GameObject(const _float& fTimeDelta) override;
	virtual _int Update_Too(const _float & fTimeDelta);
	virtual void Render_GameObject(void) override;
	static CTheme1_House*	Create(LPDIRECT3DDEVICE9 pGraphicDev, _vec3& vPos, _float fScale, _float fAngle);
private:
	virtual HRESULT Ready_GameObject(_vec3& vPos, _float fScale, _float fAngle);
	virtual HRESULT Add_Component(void);
private:
	virtual void Free(void);

};

