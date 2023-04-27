#include "stdafx.h"
#include "ProduceScene.h"

#include "AbstractFactory.h"
#include "Export_Function.h"

#include "UICamera.h"


CProduceScene::CProduceScene(LPDIRECT3DDEVICE9 pGraphicDev)
	:
	CScene(pGraphicDev)
{
}

CProduceScene::~CProduceScene()
{
}

HRESULT CProduceScene::Ready_Scene(void)
{





	return S_OK;
}

HRESULT CProduceScene::Ready_Layer_Environment(const _tchar * pLayerTag)
{
	return E_NOTIMPL;
}

HRESULT CProduceScene::Ready_Layer_GameLogic(const _tchar * pLayerTag)
{
	return E_NOTIMPL;
}

HRESULT CProduceScene::Ready_Layer_UI(const _tchar * pLayerTag)
{
	return E_NOTIMPL;
}

CProduceScene * CProduceScene::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	return nullptr;
}

void CProduceScene::Free(void)
{
}
