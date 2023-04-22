#pragma once
#include "BackGroundBase.h"
BEGIN(Engine)

class CSmokeParticle;
END
class CTheme4_Chimney :
	public CBackGroundBase
{
private:
	explicit
		CTheme4_Chimney(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CTheme4_Chimney();
public:
	virtual _int Update_GameObject(const _float& fTimeDelta) override;
	virtual void Render_GameObject(void) override;
	static CTheme4_Chimney* Create(LPDIRECT3DDEVICE9 pGraphicDev, _vec3& vPos, _float fScale, _float fAngle);
private:
	virtual HRESULT Ready_GameObject(_vec3& vPos, _float fScale, _float fAngle);
	virtual HRESULT Add_Component(void);
private:
	virtual void Free(void);

private:
	Engine::CSmokeParticle*		m_pSmokeParticle;
};