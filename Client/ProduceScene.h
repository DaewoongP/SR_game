#pragma once
#include "Scene.h"
class CProduceScene :
	public CScene
{
private:
	explicit CProduceScene(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CProduceScene();

public:
	// CScene을(를) 통해 상속됨
	virtual HRESULT		Ready_Scene(void) override;

private:
	HRESULT				Ready_Layer_Environment(const _tchar* pLayerTag);
	HRESULT				Ready_Layer_GameLogic(const _tchar* pLayerTag);
	HRESULT				Ready_Layer_UI(const _tchar* pLayerTag);
public:
	static CProduceScene*		Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	virtual void		Free(void);
};

