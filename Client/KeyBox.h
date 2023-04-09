#pragma once
#include "Cube.h"

BEGIN(Engine)
class CCubeTex;
class CTexture;
class CCollider;
END

class CKeyBox : public CCube
{
private:
	explicit CKeyBox(LPDIRECT3DDEVICE9 pGraphicDev);
    virtual ~CKeyBox();

public:
	virtual HRESULT Ready_GameObject(_vec3& vPos) override;
	virtual _int Update_GameObject(const _float& fTimeDelta) override;
	virtual void LateUpdate_GameObject(void) override;
	virtual void Render_GameObject(void) override;
private:
	HRESULT		Add_Component(void);

public:
	static CKeyBox* Create(LPDIRECT3DDEVICE9 pGraphicDev, _vec3& vPos);
	
private:
	virtual void Free() override;
};


