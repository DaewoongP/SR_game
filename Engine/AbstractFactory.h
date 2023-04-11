#pragma once
#include "Include.h"
#include "GameObject.h"
#include "Export_Function.h"
BEGIN(Engine)
#define FACTORY		CAbstractFactory
template<typename T>
class CAbstractFactory 
{
private:
	explicit CAbstractFactory(){}
	~CAbstractFactory(){}

public:
	static HRESULT	Create(const _tchar * pObjTag, CLayer* pLayer)
	{
		// 초기화
		LPDIRECT3DDEVICE9 pGraphicDev = Get_GraphicDev();
		CScene*	pScene = Get_Scene();
		NULL_CHECK_RETURN(pScene, E_FAIL);
		// 생성
		CGameObject* pGameObject = T::Create(pGraphicDev);
		NULL_CHECK_RETURN(pGameObject, E_FAIL);
		pGameObject->Sort_Component();
		FAILED_CHECK_RETURN(pLayer->Add_GameObject(pObjTag, pGameObject), E_FAIL);
		return  S_OK;
	}

	static HRESULT	Create(const _tchar * pObjTag , CLayer* pLayer, _vec3& vPos)
	{
		// 초기화
		LPDIRECT3DDEVICE9 pGraphicDev = Get_GraphicDev();
		CScene*	pScene = Get_Scene();
		NULL_CHECK_RETURN(pScene, E_FAIL);
		// 생성
		CGameObject* pGameObject = T::Create(pGraphicDev, vPos);
		NULL_CHECK_RETURN(pGameObject, E_FAIL);
		pGameObject->Sort_Component();
		FAILED_CHECK_RETURN(pLayer->Add_GameObject(pObjTag, pGameObject), E_FAIL);
		return S_OK;
	}

	static HRESULT	Create(const _tchar * pObjTag, CLayer* pLayer, _vec3& vPos, int iIndex)
	{
		// 초기화
		LPDIRECT3DDEVICE9 pGraphicDev = Get_GraphicDev();
		CScene*	pScene = Get_Scene();
		NULL_CHECK_RETURN(pScene, E_FAIL);
		// 생성
		CGameObject* pGameObject = T::Create(pGraphicDev, vPos, iIndex);
		NULL_CHECK_RETURN(pGameObject, E_FAIL);
		pGameObject->Sort_Component();
		FAILED_CHECK_RETURN(pLayer->Add_GameObject(pObjTag, pGameObject), E_FAIL);
		return S_OK;
	}

	static HRESULT	CreateParent(const _tchar * pObjTag, CLayer* pLayer, _vec3& vPos)
	{
		// 초기화
		LPDIRECT3DDEVICE9 pGraphicDev = Get_GraphicDev();
		CScene*	pScene = Get_Scene();
		NULL_CHECK_RETURN(pScene, E_FAIL);
		// 생성
		CGameObject* pGameObject = T::Create(pGraphicDev, vPos, pLayer);
		NULL_CHECK_RETURN(pGameObject, E_FAIL);
		pGameObject->Sort_Component();
		FAILED_CHECK_RETURN(pLayer->Add_GameObject(pObjTag, pGameObject), E_FAIL);
		return S_OK;
	}
};

END