#pragma once
#include "BackGroundBase.h"
class CTheme1_Floor :
	public CBackGroundBase
{
private:
	explicit
		CTheme1_Floor(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CTheme1_Floor();
public:
	virtual _int Update_GameObject(const _float& fTimeDelta) override;
	virtual void Render_GameObject(void) override;
	static CTheme1_Floor* Create(LPDIRECT3DDEVICE9 pGraphicDev, _vec3& vPos, _float fScale, _int TexNum);
private:
	virtual HRESULT Ready_GameObject(_vec3& vPos, _float fScale, _int TexNum);
	virtual HRESULT Add_Component(void);
private:
	virtual void Free(void);
private:
	_int m_iTexNum;
};

