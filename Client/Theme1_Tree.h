#pragma once
#include "BackGroundBase.h"
class CTheme1_Tree :
	public CBackGroundBase
{
private:
	explicit
		CTheme1_Tree(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CTheme1_Tree();
public:
	virtual _int Update_GameObject(const _float& fTimeDelta) override;
	virtual _int Update_Too(const _float & fTimeDelta);
	virtual void Render_GameObject(void) override;
	static CTheme1_Tree*	Create(LPDIRECT3DDEVICE9 pGraphicDev, _vec3& vPos, _float fScale);
private:
	virtual HRESULT Ready_GameObject(_vec3& vPos, _float fScale);
	virtual HRESULT Add_Component(void);
private:
	virtual void Free(void);

};