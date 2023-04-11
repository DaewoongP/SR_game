#pragma once
#include "MoveCube.h"
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
	void Set_CubeDir(CUBE_DIR dir) { m_eDir = dir; }
	_vec3 Get_CubeHeadPos();
	void Set_Other(CGameObject* pOther) { m_pOtherCube = pOther; }

	_vec3 Trans_Velocity(_vec3& velocity, CPortalCube* other);
private:
	HRESULT		Add_Component(void);
public:
	static CPortalCube*	Create(LPDIRECT3DDEVICE9 pGraphicDev, _vec3& vPos, int eDir);

private:
	virtual void Free(void) override;

private:
	CUBE_DIR			m_eDir;
	class CGameObject*		m_pOtherCube;
	_bool					m_bInit;
};

