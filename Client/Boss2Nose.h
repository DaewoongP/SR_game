#pragma once
#include "GameObject.h"
#include "Include.h"

BEGIN(Engine)
class CRcTex;
class CTexture;
END

class CBoss2Nose :
	public CGameObject
{
public:
	explicit CBoss2Nose(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CBoss2Nose();

	virtual HRESULT Ready_GameObject(_vec3& vPos) override;
	virtual _int Update_GameObject(const _float& fTimeDelta) override;
	virtual _int Update_Too(const _float& fTimeDelta) override;
	virtual _int Update_Top(const _float& fTimeDelta) override;
	virtual void LateUpdate_GameObject(void) override;
	virtual void Render_GameObject(void) override;
	virtual void Render_Too(void) override;
	virtual void Render_Top(void) override;

private:
	Engine::CRcTex*			m_pBufferCom;
	Engine::CTexture*		m_pTextureCom;

private:
	HRESULT		Add_Component(void);

public:
	static CBoss2Nose*		Create(LPDIRECT3DDEVICE9 pGraphicDev, _vec3& vPos);

private:
	virtual void Free(void) override;
};

