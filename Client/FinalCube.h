#pragma once
#include "Include.h"
#include "GameObject.h"

BEGIN(Engine)

class CCubeTex;
class CTexture;
class CCollider;
class CTexParticle;
END

class CFinalCube :
	public CGameObject
{
private:
	explicit CFinalCube(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CFinalCube();

public:
	virtual HRESULT Ready_GameObject(_vec3& vPos, _int iIndex);
	virtual _int Update_GameObject(const _float& fTimeDelta) override;
	virtual void LateUpdate_GameObject(void) override;
	virtual void Render_GameObject(void) override;

	virtual void OnCollisionEnter(const class Collision* collision);

private:
	HRESULT		Add_Component(void);

private:
	Engine::CCubeTex*	m_pBufferCom;
	Engine::CTexture*	m_pTextureCom;
	Engine::CCollider*  m_pCollider;
	Engine::CTexParticle* m_pExpParticle;
	_int		m_iCubeIndex;

	_bool		m_bCreateItem;

public:
	static CFinalCube* Create(LPDIRECT3DDEVICE9 pGraphicDev, _vec3& vPos, _int iIndex);

private:
	virtual void Free() override;
};

