#pragma once
#include "Scene.h"

class CStage4 : public Engine::CScene
{
private:
	explicit CStage4(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CStage4();

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
	static CStage4*		Create(LPDIRECT3DDEVICE9 pGraphicDev);
	_int m_iHp;
	_float		m_fHittime;

private:
	virtual void		Free(void);
};

