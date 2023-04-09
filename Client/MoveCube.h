#pragma once
#include "Include.h"
#include "GameObject.h"
#include "Cube.h"

BEGIN(Engine)
class CCubeTex;
class CTexture;
class CCollider;
class CLine;
END

class CMoveCube :
	public CCube
{
protected:
	explicit CMoveCube(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CMoveCube();

public:
	virtual HRESULT Ready_GameObject(_vec3& vPos) override;
	virtual _int Update_GameObject(const _float& fTimeDelta) override;

	virtual _int Update_Top(const _float& fTimeDelta) override;
	virtual void LateUpdate_GameObject(void) override;
	virtual void Render_GameObject(void) override;

	virtual void OnCollisionEnter(const class Collision* collision);
	virtual void OnCollisionStay(const class Collision* collision);
	virtual void OnCollisionExit(const class Collision* collision);

protected:
	HRESULT		Add_Component(void);
	void		Move(const _float& fTimeDelta);
	_bool		IsMoveDone(const _float& fTimeDelta);
	
	//벽충돌감지용
	void		ShootRay();
	void		CheckColAble(_vec3 vdir,float len, COL_DIR edir);

	//이동용
	_bool		DoRayToDir(COL_DIR dir);
	void		SetMovePos(COL_DIR dir);

	//들기용
	void		MoveToPos(const _float& fTimeDelta);

public:
	void		SetTarget(_vec3 pos, CGameObject* obj);
	_bool		GetHandleState() {return (((m_handleState == CH_ING) || (m_handleState == CH_NONE))&& !m_bIsMoving); }
	void		DoFallingStart();

protected:
	Engine::CLine*			m_pLine;

	_float					m_fSpeed = 16.f;
	_vec3					m_MovetoPos;
	_bool					m_bIsMoving = false;
	_vec3					prePos;
	_vec3					m_MoveVec;

	_vec3					m_TargetPos; //들때 올 위치
	CGameObject*			m_Target;
	CUBE_HANDING			m_handleState = CH_NONE;

public:
	_bool					m_bIsCol[DIR_END] = { 0 };
	_bool					m_bIsStone=false;
	_bool					m_bIsFall = false;

public:
	static CMoveCube*	Create(LPDIRECT3DDEVICE9 pGraphicDev, _vec3& vPos);

protected:
	virtual void Free(void) override;
};

