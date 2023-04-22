#pragma once
#include "BackGroundBase.h"
class CTheme2_Bush_1 :
	public CBackGroundBase
{
private:
	explicit
		CTheme2_Bush_1(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CTheme2_Bush_1();
public:
	virtual _int Update_GameObject(const _float& fTimeDelta) override;
	virtual _int Update_Too(const _float & fTimeDelta);
	virtual void Render_GameObject(void) override;
	static CTheme2_Bush_1*	Create(LPDIRECT3DDEVICE9 pGraphicDev, _vec3& vPos, _float fScale);
private:
	virtual HRESULT Ready_GameObject(_vec3& vPos, _float fScale);
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