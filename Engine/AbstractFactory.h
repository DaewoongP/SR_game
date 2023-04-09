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
		// �ʱ�ȭ
		LPDIRECT3DDEVICE9 pGraphicDev = Get_GraphicDev();
		CScene*	pScene = Get_Scene();
		NULL_CHECK_RETURN(pScene, E_FAIL);
		// ����
		CGameObject* pGameObject = T::Create(pGraphicDev);
		NULL_CHECK_RETURN(pGameObject, E_FAIL);
		FAILED_CHECK_RETURN(pLayer->Add_GameObject(pObjTag, pGameObject), E_FAIL);
		return  S_OK;
	}

	static HRESULT	Create(const _tchar * pObjTag , CLayer* pLayer, _vec3& vPos)
	{
		// �ʱ�ȭ
		LPDIRECT3DDEVICE9 pGraphicDev = Get_GraphicDev();
		CScene*	pScene = Get_Scene();
		NULL_CHECK_RETURN(pScene, E_FAIL);
		// ����
		CGameObject* pGameObject = T::Create(pGraphicDev, vPos);
		NULL_CHECK_RETURN(pGameObject, E_FAIL);
		FAILED_CHECK_RETURN(pLayer->Add_GameObject(pObjTag, pGameObject), E_FAIL);
		return S_OK;
	}
};

END