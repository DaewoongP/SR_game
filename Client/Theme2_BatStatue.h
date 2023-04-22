#pragma once
#include "BackGroundBase.h"
class CTheme2_BatStatue :
	public CBackGroundBase
{
private:
	explicit
		CTheme2_BatStatue(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CTheme2_BatStatue();
public:
	virtual _int Update_GameObject(const _float& fTimeDelta) override;
	virtual void Render_GameObject(void) override;
	static CTheme2_BatStatue*	Create(LPDIRECT3DDEVICE9 pGraphicDev, _vec3& vPos, _float fScale);
private:
	virtual HRESULT Ready_GameObject(_vec3& vPos, _float fScale);
	virtual HRESULT Add_Component(void);
private:
	virtual void Free(void);
};