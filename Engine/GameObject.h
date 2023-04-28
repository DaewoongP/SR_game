#pragma once

#include "Component.h"
#include "Base.h"

BEGIN(Engine)

class  CGameObject : public CBase
{
protected:
	explicit CGameObject(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CGameObject();

public:
	CComponent*			Get_Component(const _tchar* pComponentTag, COMPONENTID eID);
	void				Set_Tag(const _tchar* pTag);
public:
	virtual HRESULT		Ready_GameObject(void) { return S_OK; }
	virtual HRESULT		Ready_GameObject(_vec3& vPos) { return S_OK; }
	virtual _int		Update_GameObject(const _float& fTimeDelta);
	virtual _int		Update_Too(const _float& fTimeDelta) { return 0; }
	virtual _int		Update_Top(const _float& fTimeDelta) { return 0; }
	virtual void		LateUpdate_GameObject(void);
	virtual void		LateUpdate_Too() {}
	virtual void		LateUpdate_Top() {}
	virtual void		Render_GameObject(void);
	virtual void		Render_Too() {}
	virtual void		Render_Top() {}
	// 2D -> 3D 전환 or 3D -> 2D 전환시 호출
	virtual void		SwapTrigger() { }

	virtual void OnCollisionEnter(const class Collision* collsion);
	virtual void OnCollisionStay(const class Collision* collision);
	virtual void OnCollisionExit(const class Collision* collision);

	void				Sort_Component();
public:
	void				Set_Dead() { m_bDead = true; }
	_int				Get_Hp() { return m_iHp; }
	_bool		Get_Damage() { return m_bDamage; }
	_bool		Set_D_T() { return m_bDamage = true; }
	_bool		Set_Damage() { return m_bDamage = false; }
	_bool		m_bDamage = false;
	void				Set_Render(_bool value) { m_bUseRender = value; }
	void				Set_Update(_bool value) { m_bUseUpdate = value; }
	_bool				Get_Render() { return m_bUseRender; }
	_bool				Get_Update() { return m_bUseUpdate; }
	_bool				Get_Dead() { return m_bDead; }
private:
	CComponent*			Find_Component(const _tchar* pComponentTag, COMPONENTID eID);

protected:
	vector<pair<const _tchar*, CComponent*>> m_vecComponent[ID_END];
	LPDIRECT3DDEVICE9		m_pGraphicDev;

public:
	_tchar					m_pTag[MAX_STR];
	class CTransform*	m_pTransform;
	_bool					m_bDead;
	_float					m_iHp;
	_bool					m_bUseRender = true;
	_bool					m_bUseUpdate = true;
protected:
	virtual void		Free(void);
};

END