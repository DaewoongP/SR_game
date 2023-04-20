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
		// ï¿½Ê±ï¿½È­
		LPDIRECT3DDEVICE9 pGraphicDev = Get_GraphicDev();
		// ï¿½ï¿½ï¿½ï¿½
		CGameObject* pGameObject = T::Create(pGraphicDev);
		NULL_CHECK_RETURN(pGameObject, E_FAIL);
		pGameObject->Sort_Component();
		FAILED_CHECK_RETURN(pLayer->Add_GameObject(pObjTag, pGameObject), E_FAIL);
		return  S_OK;
	}

	static HRESULT	Create(const _tchar * pObjTag , CLayer* pLayer, _vec3& vPos)
	{
		// ï¿½Ê±ï¿½È­
		LPDIRECT3DDEVICE9 pGraphicDev = Get_GraphicDev();
		// ï¿½ï¿½ï¿½ï¿½
		CGameObject* pGameObject = T::Create(pGraphicDev, vPos);
		NULL_CHECK_RETURN(pGameObject, E_FAIL);
		pGameObject->Sort_Component();
		FAILED_CHECK_RETURN(pLayer->Add_GameObject(pObjTag, pGameObject), E_FAIL);
		return S_OK;
	}

	static HRESULT	Create(const _tchar * pObjTag, CLayer* pLayer, _vec3& vPos, int iIndex)
	{
		// ï¿½Ê±ï¿½È­
		LPDIRECT3DDEVICE9 pGraphicDev = Get_GraphicDev();
		// ï¿½ï¿½ï¿½ï¿½
		CGameObject* pGameObject = T::Create(pGraphicDev, vPos, iIndex);
		NULL_CHECK_RETURN(pGameObject, E_FAIL);
		pGameObject->Sort_Component();
		FAILED_CHECK_RETURN(pLayer->Add_GameObject(pObjTag, pGameObject), E_FAIL);
		return S_OK;
	}

	static HRESULT	CreateParent(const _tchar * pObjTag, CLayer* pLayer, _vec3& vPos)
	{
		// ï¿½Ê±ï¿½È­
		LPDIRECT3DDEVICE9 pGraphicDev = Get_GraphicDev();
		// ï¿½ï¿½ï¿½ï¿½
		CGameObject* pGameObject = T::Create(pGraphicDev, vPos, pLayer);
		NULL_CHECK_RETURN(pGameObject, E_FAIL);
		pGameObject->Sort_Component();
		FAILED_CHECK_RETURN(pLayer->Add_GameObject(pObjTag, pGameObject), E_FAIL);
		return S_OK;
	}

	static HRESULT	Create(const _tchar * pObjTag, CLayer* pLayer, _vec3& vPos, CTransform* trans)
	{
		// ÃÊ±âÈ­
		LPDIRECT3DDEVICE9 pGraphicDev = Get_GraphicDev();
		CScene*	pScene = Get_Scene();
		// »ý¼º
		CGameObject* pGameObject = T::Create(pGraphicDev, vPos);
		NULL_CHECK_RETURN(pGameObject, E_FAIL);
		pGameObject->Sort_Component();
		pGameObject->m_pTransform->SetParent(trans);
		FAILED_CHECK_RETURN(pLayer->Add_GameObject(pObjTag, pGameObject), E_FAIL);
		return S_OK;
	}
	//Å¾µð ÆÄÃ÷¿ëÀÓ
	static HRESULT	Create(const _tchar * pObjTag, CLayer* pLayer, _vec3& vPos, CTransform* trans, _tchar* texturename, _int idx, _bool setAnim)
	{
		// ÃÊ±âÈ­
		LPDIRECT3DDEVICE9 pGraphicDev = Get_GraphicDev();
		CScene*	pScene = Get_Scene();
		// »ý¼º
		CGameObject* pGameObject = T::Create(pGraphicDev, vPos, trans, texturename, idx, setAnim);
		NULL_CHECK_RETURN(pGameObject, E_FAIL);
		pGameObject->Sort_Component();
		pGameObject->m_pTransform->SetParent(trans);
		FAILED_CHECK_RETURN(pLayer->Add_GameObject(pObjTag, pGameObject), E_FAIL);
		return S_OK;
	}
	//¹é±×¶ó¿îµå ¿ÀºêÁ§Æ® »ý¼º¿ë
	static HRESULT	Create(const _tchar * pObjTag, CLayer* pLayer, _vec3& vPos, _float fScale)
	{
		LPDIRECT3DDEVICE9 pGraphicDev = Get_GraphicDev();
		CGameObject* pGameObject = T::Create(pGraphicDev, vPos, fScale);
		NULL_CHECK_RETURN(pGameObject, E_FAIL);
		pGameObject->Sort_Component();
		FAILED_CHECK_RETURN(pLayer->Add_GameObject(pObjTag, pGameObject), E_FAIL);
		return S_OK;
	}
};

END