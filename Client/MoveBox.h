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

class CMoveBox :
	public CCube
{
private:
	explicit CMoveBox(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CMoveBox();

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

	virtual void OnCollisionEnter(const class Collision* collision);
	virtual void OnCollisionStay(const class Collision* collision);
	virtual void OnCollisionExit(const class Collision* collision);

private:
	HRESULT		Add_Component(void);
	void		Move(const _float& fTimeDelta);
	_bool		IsMoveDone(const _float& fTimeDelta);
	_bool		ShootRay();
	_bool		DoRayToDir(COL_DIR dir);
	void		SetMovePos(COL_DIR dir);

private:
	Engine::CCubeTex*	m_pBufferCom;
	Engine::CTexture*	m_pTextureCom;
	Engine::CCollider * m_pCollider;
	Engine::CLine*			m_pLine;

	_float					m_fSpeed = 16.f;
	_vec3					m_MovetoPos;
	_bool					m_bIsMoving = false;
	_vec3					prePos;
	_vec3					m_MoveVec;
	
public:
	_bool					m_bIsCol[DIR_END] = { 0 };

public:
	static CMoveBox*	Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	virtual void Free(void) override;
};

