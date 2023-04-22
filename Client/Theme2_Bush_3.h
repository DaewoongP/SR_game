#pragma once
#include "BackGroundBase.h"
class CTheme2_Bush_3 :
	public CBackGroundBase
{
private:
	explicit
		CTheme2_Bush_3(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CTheme2_Bush_3();
public:
	virtual _int Update_GameObject(const _float& fTimeDelta) override;
	virtual _int Update_Too(const _float & fTimeDelta);
	virtual void Render_GameObject(void) override;
	static CTheme2_Bush_3*	Create(LPDIRECT3DDEVICE9 pGraphicDev, _vec3& vPos, _float fScale, _float fAngle);
private:
	virtual HRESULT Ready_GameObject(_vec3& vPos, _float fScale, _float fAngle);
	virtual HRESULT Add_Component(void);
private:
	virtual void Free(void);
private:
	_float m_fMinAngle;
	_float m_fMaxAngle;
	//-1~1->0~1
	//-1 + 1 * 0.5 = 0; 
	// 1 + 1 * 0.5 = 1;
};