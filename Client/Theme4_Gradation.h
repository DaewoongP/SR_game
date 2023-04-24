#pragma once
#include "GameObject.h"
class CTheme4_Gradation :
	public CGameObject
{
private:
	explicit
		CTheme4_Gradation(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CTheme4_Gradation();
public:
	virtual _int Update_GameObject(const _float& fTimeDelta) override;
	virtual void Render_GameObject(void) override;
	static CTheme4_Gradation*	Create(LPDIRECT3DDEVICE9 pGraphicDev, _vec3& vPos, _float fScale, _float fAngle);
private:
	virtual HRESULT Ready_GameObject(_vec3& vPos, _float fScale, _float fAngle);
	virtual HRESULT Add_Component(void);
private:
	virtual void Free(void);
	Engine::CRcGradation*			m_pBufferGRCom;
};