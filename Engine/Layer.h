#pragma once

#include "Base.h"
#include "GameObject.h"

BEGIN(Engine)

class  CLayer : public CBase
{
private:
	explicit CLayer();
	virtual ~CLayer();

public:
	CComponent*			Get_Component(const _tchar* pObjTag, const _tchar* pComponentTag, COMPONENTID eID);
	CGameObject*		Get_GameObject(const _tchar* pObjTag);

public:
	HRESULT				Add_GameObject(const _tchar* pObjTag, CGameObject* pGameObject);

	HRESULT				Ready_Layer(void);
	_int				Update_Layer(const _float& fTimeDelta);
	void				LateUpdate_Layer(void);
	list<pair<const _tchar*, CGameObject*>>* GetMapObject() { return &m_uMapObject; }

public:
	void				Delete_In_Layer();
	void				Delete_Tag(const _tchar* pObjTag);
	HRESULT				Delete_LastObject(CGameObject* pGameObject);
private:
	list<pair<const _tchar*, CGameObject*>>	m_uMapObject;

	_bool		bPreState;

public:
	static CLayer*		Create(void);

private:
	virtual void Free(void);

};

END