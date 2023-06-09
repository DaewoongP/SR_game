#pragma once

#include "Base.h"
#include "Layer.h"

BEGIN(Engine)

class  CScene : public CBase
{
protected:
	explicit CScene(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CScene();

public:
	CComponent*			Get_Component(const _tchar* pLayerTag, const _tchar* pObjTag, const _tchar* pComponentTag, COMPONENTID eID);
	CLayer*				Get_Layer(const _tchar* pLayerTag);
	CGameObject*		Get_GameObject(const _tchar* pLayerTag, const _tchar* pObjTag);
public:
	virtual HRESULT		Ready_Scene(void) PURE;
	virtual _int		Update_Scene(const _float& fTimeDelta);
	virtual void		LateUpdate_Scene(void);
	virtual void		Render_Scene(void)PURE;

	void Set_LoadingID(LOADINGID eID) { m_eLoadingID = eID; };
	LOADINGID	Get_LoadingID() { return m_eLoadingID; }

	void Release_Fade()
	{ 
		if (m_pFade != nullptr)
			m_pFade->Release();
	}
protected:
	LPDIRECT3DDEVICE9			m_pGraphicDev;
	unordered_map<const _tchar*, CLayer*>		m_uMapLayer;
	LOADINGID					m_eLoadingID;

	CGameObject*				m_pFade;
	CScene*						m_pPreScene;
protected:
	virtual void	Free(void);
};

END