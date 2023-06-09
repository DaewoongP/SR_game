#pragma once

#include "Base.h"
#include "Scene.h"

BEGIN(Engine)

class  CManagement : public CBase
{
	DECLARE_SINGLETON(CManagement)

private:
	explicit CManagement();
	virtual ~CManagement();

public:
	CComponent*			Get_Component(const _tchar* pLayerTag, const _tchar* pObjTag, const _tchar* pComponentTag, COMPONENTID eID);
	CLayer*				Get_Layer(const _tchar* pLayerTag);
	CGameObject*		Get_GameObject(const _tchar* pLayerTag, const _tchar* pObjTag);

public:
	HRESULT			Set_Scene(CScene* pScene);
	CScene*			Get_Scene() { return m_pScene; }

	_int			Update_Management(const _float& fTimeDelta);
	void			LateUpdate_Management();
	void			Render_Management(LPDIRECT3DDEVICE9 pGraphicDev);
private:
	CScene*			m_pScene = nullptr;
private:
	virtual void	Free(void);
};

END