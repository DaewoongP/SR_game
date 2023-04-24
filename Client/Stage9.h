#pragma once
#include "Scene.h"

class CStage9 : public Engine::CScene
{
private:
	explicit CStage9(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CStage9();

public:
	// CScene��(��) ���� ��ӵ�
	virtual HRESULT		Ready_Scene(void) override;
	virtual _int		Update_Scene(const _float& fTimeDelta)override;
	virtual void		LateUpdate_Scene(void)override;
	virtual void		Render_Scene(void) override;

private:
	HRESULT				Ready_Layer_Environment(const _tchar* pLayerTag);
	HRESULT				Ready_Layer_GameLogic(const _tchar* pLayerTag);
	HRESULT				Ready_Layer_UI(const _tchar* pLayerTag);

public:
	static CStage9* Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	virtual void		Free(void);
};
