#pragma once
#include"Cube.h"
#define CRACKTIME	2.f

BEGIN(Engine)
class CCubeTex;
class CTexture;
class CCollider;
class CTexParticle;
END	

class CCrackCube :
    public CCube
{
private:
	explicit CCrackCube(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CCrackCube();

public:
	virtual HRESULT Ready_GameObject(_vec3& vPos) override;
	virtual _int Update_GameObject(const _float& fTimeDelta) override;
	virtual _int Update_Too(const _float& fTimeDelta) override;
	virtual _int Update_Top(const _float& fTimeDelta) override;
	virtual void LateUpdate_GameObject(void) override;
	virtual void Render_GameObject(void) override;

	virtual void OnCollisionEnter(const class Collision* collision);

public:
	_bool GetCrackDead() { return m_bCrackDead; }
	void DoShootRay(COL_DIR exceptDir);
private:
	HRESULT Add_Component(void);
	void ShootRay(COL_DIR exceptDir);
private:
	Engine::CCubeTex* m_pBufferCom;
	Engine::CTexture* m_pTextureCom;
	Engine::CCollider* m_pCollider;
	Engine::CTexParticle*	m_pExpParticle;
private:
	//블럭이 사라지는 시간 *.*초
	_float m_fBlockTime;
	void Shaking(_vec3& vPos,const _float& fTimeDelta);
	_bool m_bRealDead;
	_bool m_bCrackDead;

	_bool m_bShakeDir;
	
public:
	static CCrackCube* Create(LPDIRECT3DDEVICE9 pGraphicDev, _vec3& vPos);

private:
	virtual void Free(void) override;
};

