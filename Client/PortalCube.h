#pragma once
#include "MoveCube.h"
BEGIN(Engine)

class CJumpParticle;
END
class CPortalCube :
	public CMoveCube
{
public:
	explicit CPortalCube(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CPortalCube();

public:
	virtual HRESULT Ready_GameObject(_vec3& vPos) override;
	virtual _int Update_Too(const _float& fTimeDelta) override;
	virtual _int Update_Top(const _float& fTimeDelta) override;
	virtual _int Update_GameObject(const _float& fTimeDelta) override;
	virtual void LateUpdate_GameObject(void) override;
	virtual void Render_GameObject(void) override;

	virtual void OnCollisionEnter(const class Collision* collision);
	virtual void OnCollisionStay(const class Collision* collision);
	virtual void OnCollisionExit(const class Collision* collision);
public:
	void Set_CubeDir(CUBE_DIR dir);
	CUBE_DIR Get_CubeDir() { return m_eDir; }
	_vec3 Get_CubeHeadPos();
	void Set_Other(CGameObject* pOther) { m_pOtherCube = pOther; }
	void CoolReset() { m_dwCool = 0; }
	_vec3 Trans_Velocity(_vec3& velocity, CPortalCube* other);
	void ShootRay_Portal();
	_vec3 GetDirVec() { return m_DirVec; }

	void Start_PortalParticle();
private:
	HRESULT		Add_Component(void);

public:
	static CPortalCube*	Create(LPDIRECT3DDEVICE9 pGraphicDev, _vec3& vPos, int eDir);

private:
	_bool Check_BoundingBox(CCollider * pSrc, CCollider * pDest);
	virtual void Free(void) override;

private:
	_float					m_dwCool;
	CUBE_DIR				m_eDir;
	class CGameObject*		m_pOtherCube;
	_bool					m_bInit;
	_vec3					m_DirVec;
	Engine::CJumpParticle*	m_pPortalParticle;
};

