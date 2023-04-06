#pragma once
#include "Cube.h"

BEGIN(Engine)
class CCubeTex;
class CTexture;
class CCollider;

END

class CKeyBox :
    public CCube
{
private:
	explicit CKeyBox(LPDIRECT3DDEVICE9 pGraphicDev);
    virtual ~CKeyBox();

public:
	virtual HRESULT Ready_GameObject(void) override;
	virtual _int Update_GameObject(const _float& fTimeDelta) override;
	virtual _int Update_Too(const _float& fTimeDelta) override;
	virtual _int Update_Top(const _float& fTimeDelta) override;
	virtual void LateUpdate_GameObject(void) override;
	virtual void LateUpdate_Too() override;
	virtual void LateUpdate_Top() override;
	virtual void Render_GameObject(void) override;
	virtual void Render_Too() override;
	virtual void Render_Top() override;
	

private:
	HRESULT		Add_Component(void);
private:
	Engine::CCubeTex* m_pBufferCom;
	Engine::CTexture* m_pTextureCom;
	Engine::CCollider* m_pCollider;
	
	
public:
	static CKeyBox* Create(LPDIRECT3DDEVICE9 pGraphicDev);
	

	
private:
	virtual void Free() override ;



};


