#pragma once

#include "Scene.h"
#include "Loading.h"
class CPreStage : public Engine::CScene
{
private:
	explicit CPreStage(LPDIRECT3DDEVICE9 pGraphicDev, LOADINGID eID);
	virtual ~CPreStage();

public:
	// CScene을(를) 통해 상속됨
	virtual HRESULT		Ready_Scene(void) override;
	virtual _int		Update_Scene(const _float& fTimeDelta)override;
	virtual void		LateUpdate_Scene(void)override;
	virtual void		Render_Scene(void) override;

	_bool		isLoadingFinished() { return m_pLoading->Get_Finish(); }
	void Set_Scene(CScene* pScene) { m_pScene = pScene; }
private:
	HRESULT				Ready_Layer_UI(const _tchar* pLayerTag);
private:
	CLoading*					m_pLoading;
	LOADINGID					m_eLoadingID;
	CScene*						m_pScene = nullptr;
public:
	static CPreStage*		Create(LPDIRECT3DDEVICE9 pGraphicDev, LOADINGID eID);

private:
	virtual void		Free(void);
};

