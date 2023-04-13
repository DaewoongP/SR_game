#pragma once
#include "Cube.h"

BEGIN(Engine)
class CBlockExp;
END
class CKeyCube : public CCube
{
private:
	explicit CKeyCube(LPDIRECT3DDEVICE9 pGraphicDev);
    virtual ~CKeyCube();

public:
	virtual HRESULT Ready_GameObject(_vec3& vPos) override;
	virtual _int Update_GameObject(const _float& fTimeDelta) override;
	virtual void LateUpdate_GameObject(void) override;
	virtual void Render_GameObject(void) override;
private:
	HRESULT		Add_Component(void);

public:
	static CKeyCube* Create(LPDIRECT3DDEVICE9 pGraphicDev, _vec3& vPos);
	
private:
	virtual void Free() override;
private:
	Engine::CBlockExp* m_pExpParticle;
};


