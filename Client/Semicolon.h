#pragma once
#include "GameObject.h"
BEGIN(Engine)

class CRcTex;
class CTexture;
class CCollider;
END

class CSemicolon : public CGameObject
{
private:
	explicit CSemicolon(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CSemicolon();

public:
	virtual HRESULT Ready_GameObject(_vec3& vPos) override;
	virtual _int Update_GameObject(const _float& fTimeDelta) override;
	virtual void Render_GameObject(void) override;

private:
	HRESULT		Add_Component(void);

	void		PreRender();
	void		PostRender();

private:
	Engine::CRcTex*		m_pBufferCom;
	Engine::CTexture*	m_pSemicolon;
	Engine::CCollider*		m_pCollider;

public:
	static CSemicolon*		Create(LPDIRECT3DDEVICE9 pGraphicDev, _vec3 vPos);

private:
	virtual void Free(void) override;
};
