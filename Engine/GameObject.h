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

public:
	virtual HRESULT		Ready_GameObject(void)PURE;
	virtual _int		Update_GameObject(const _float& fTimeDelta);
	virtual void		LateUpdate_GameObject(void);
	virtual void		Render_GameObject(void);

	virtual void OnCollisionEnter(const class Collision* collsion) {}
	virtual void OnCollisionStay(const class Collision* collision) {}
	virtual void OnCollisionExit(const class Collision* collision) {}

	virtual void OnTriggerEnter(const class CCollider* other) {}
	virtual void OnTriggerStay(const class CCollider* other) {}
	virtual void OnTirggerExit(const class CCollider* other) {}

private:
	CComponent*			Find_Component(const _tchar* pComponentTag, COMPONENTID eID);

protected:
	unordered_map<const _tchar*, CComponent*>			m_uMapComponent[ID_END];
	LPDIRECT3DDEVICE9		m_pGraphicDev;

public:
	class CTransform* m_pTransform;

protected:
	virtual void		Free(void);
};

END