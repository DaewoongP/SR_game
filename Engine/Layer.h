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

public:
	HRESULT				Add_GameObject(const _tchar* pObjTag, CGameObject* pGameObject);

	HRESULT				Ready_Layer(void);
	_int				Update_Layer(const _float& fTimeDelta);
	void				LateUpdate_Layer(void);

private:
	unordered_map<const _tchar*, CGameObject*>			m_uMapObject;

public:
	static CLayer*		Create(void);

private:
	virtual void Free(void);

};

END