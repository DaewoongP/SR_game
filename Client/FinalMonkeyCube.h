#pragma once
#include "Include.h"
#include "GameObject.h"
#include "Cube.h"
class CFinalMonkeyCube :
	public CCube
{
protected:
	explicit CFinalMonkeyCube(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CFinalMonkeyCube();

public:
	virtual HRESULT Ready_GameObject(_vec3& vPos,vector<CGameObject*>& vec);
	virtual _int Update_GameObject(const _float& fTimeDelta) override;
	virtual void LateUpdate_GameObject(void) override;
	virtual void Render_GameObject(void) override;
	virtual void OnCollisionEnter(const class Collision* collision);
	virtual void OnCollisionStay(const class Collision* collision);
	virtual void OnCollisionExit(const class Collision* collision);

public:
	_bool Get_MoveDone() {return m_isMoveDone;}
	void	SetLerpPos(_float value, _float time, _bool revese) { m_fLerpPos_z = value; m_SleepTime = time; m_Reverse = revese; }
private:
	_bool	m_isMoveDone;
	_float	m_fLerpPos_z;
	_bool	m_Reverse;
protected:
	HRESULT Add_Component(void);

protected:
	_float m_SleepTime;
public:
	static CFinalMonkeyCube*	Create(LPDIRECT3DDEVICE9 pGraphicDev, _vec3& vPos, vector<CGameObject*>& vec, _int iIndex = 0);

protected:
	virtual void Free(void) override;
};

