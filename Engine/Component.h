#pragma once

#include "Base.h"
#include "Include.h"

BEGIN(Engine)

class CComponent : public CBase
{
	friend class CProtoMgr;

protected:
	explicit CComponent();
	explicit CComponent(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CComponent(const CComponent& rhs);
	virtual ~CComponent();

public:
	virtual			_int	Update_Component(const _float& fTimeDelta) { return 0; }
	virtual			void	LateUpdate_Component() {}
	virtual			void	Render_Component() {}

	CComponent*			Get_Component(const _tchar* pComponentTag, COMPONENTID eID);
	CGameObject*		Get_Parent(void) { return m_pGameObject; }

public:
	LPDIRECT3DDEVICE9			m_pGraphicDev;
	_bool						m_bClone;
	class CGameObject*			m_pGameObject;	
	// 현재 2D, Topview 확인 변수 기본값 2D
	_bool						Is2D;

public:
	virtual	CComponent*	Clone(void) PURE;
	virtual void		Free(void)override;

private:
	void SetOwner(class CGameObject* gameObject);
};

END