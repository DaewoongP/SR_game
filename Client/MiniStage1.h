#pragma once
#include "Scene.h"

class CMiniStage1 : public Engine::CScene
{
private:
	explicit CMiniStage1(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CMiniStage1();

public:
	// CScene을(를) 통해 상속됨
	virtual HRESULT		Ready_Scene(void) override;
	virtual _int		Update_Scene(const _float& fTimeDelta)override;
	virtual void		LateUpdate_Scene(void)override;
	virtual void		Render_Scene(void) override;

private:
	HRESULT				Ready_Layer_Environment(const _tchar* pLayerTag);
	HRESULT				Ready_Layer_GameLogic(const _tchar* pLayerTag);
	HRESULT				Ready_Layer_UI(const _tchar* pLayerTag);
public:
	static CMiniStage1*		Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:

	_float				m_dwSpawnTimer;
	virtual void		Free(void);
};

