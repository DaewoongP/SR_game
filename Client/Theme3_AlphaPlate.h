#pragma once
#include "BackGroundBase.h"
class CTheme3_AlphaPlate :
	public CBackGroundBase
{
private:
	explicit
		CTheme3_AlphaPlate(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CTheme3_AlphaPlate();
public:
	virtual _int Update_GameObject(const _float& fTimeDelta) override;
	virtual void Render_GameObject(void) override;
	static CTheme3_AlphaPlate*	Create(LPDIRECT3DDEVICE9 pGraphicDev, _vec3& vPos, _float fScale, _float fAngle);
private:
	virtual HRESULT Ready_GameObject(_vec3& vPos, _float fScale, _float fAngle);
	virtual HRESULT Add_Component(void);
private:
	virtual void Free(void);
};