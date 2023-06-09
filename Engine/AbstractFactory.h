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
		// ����
		CGameObject* pGameObject = T::Create(pGraphicDev);
		NULL_CHECK_RETURN(pGameObject, E_FAIL);
		pGameObject->Sort_Component();
		FAILED_CHECK_RETURN(pLayer->Add_GameObject(pObjTag, pGameObject), E_FAIL);
		return  S_OK;
	}

	static HRESULT	Create(const _tchar * pObjTag, CLayer* pLayer, _vec3& vPos)
	{
		// �ʱ�ȭ
		LPDIRECT3DDEVICE9 pGraphicDev = Get_GraphicDev();
		// ����
		CGameObject* pGameObject = T::Create(pGraphicDev, vPos);
		NULL_CHECK_RETURN(pGameObject, E_FAIL);
		pGameObject->Sort_Component();
		FAILED_CHECK_RETURN(pLayer->Add_GameObject(pObjTag, pGameObject), E_FAIL);
		return S_OK;
	}

	static HRESULT	Create(const _tchar * pObjTag, CLayer* pLayer, _vec3& vPos,_vec3& vtoward)
	{
		// �ʱ�ȭ
		LPDIRECT3DDEVICE9 pGraphicDev = Get_GraphicDev();
		// ����
		CGameObject* pGameObject = T::Create(pGraphicDev, vPos, vtoward);
		NULL_CHECK_RETURN(pGameObject, E_FAIL);
		pGameObject->Sort_Component();
		FAILED_CHECK_RETURN(pLayer->Add_GameObject(pObjTag, pGameObject), E_FAIL);
		return S_OK;
	}

	static HRESULT	Create(const _tchar * pObjTag, CLayer* pLayer, _vec3& vPos, int iIndex)
	{
		// �ʱ�ȭ
		LPDIRECT3DDEVICE9 pGraphicDev = Get_GraphicDev();
		// ����
		CGameObject* pGameObject = T::Create(pGraphicDev, vPos, iIndex);
		NULL_CHECK_RETURN(pGameObject, E_FAIL);
		pGameObject->Sort_Component();
		FAILED_CHECK_RETURN(pLayer->Add_GameObject(pObjTag, pGameObject), E_FAIL);
		return S_OK;
	}

	static HRESULT	Create(const _tchar * pObjTag, _vec3& vPos, vector<CGameObject*>& vec,int iIndex)
	{
		// �ʱ�ȭ
		LPDIRECT3DDEVICE9 pGraphicDev = Get_GraphicDev();
		// ����
		CGameObject* pGameObject = T::Create(pGraphicDev, vPos, vec,iIndex );
		NULL_CHECK_RETURN(pGameObject, E_FAIL);
		lstrcpy(pGameObject->m_pTag, pObjTag);
		pGameObject->Sort_Component();
		vec.push_back(pGameObject);
		return S_OK;
	}

	static HRESULT	CreateParent(const _tchar * pObjTag, CLayer* pLayer, _vec3& vPos)
	{
		// �ʱ�ȭ
		LPDIRECT3DDEVICE9 pGraphicDev = Get_GraphicDev();
		// ����
		CGameObject* pGameObject = T::Create(pGraphicDev, vPos, pLayer);
		NULL_CHECK_RETURN(pGameObject, E_FAIL);
		pGameObject->Sort_Component();
		FAILED_CHECK_RETURN(pLayer->Add_GameObject(pObjTag, pGameObject), E_FAIL);
		return S_OK;
	}

	static HRESULT	Create(const _tchar * pObjTag, CLayer* pLayer, _vec3& vPos, CTransform* trans)
	{
		// �ʱ�ȭ
		LPDIRECT3DDEVICE9 pGraphicDev = Get_GraphicDev();
		CScene*	pScene = Get_Scene();
		// ����
		CGameObject* pGameObject = T::Create(pGraphicDev, vPos);
		NULL_CHECK_RETURN(pGameObject, E_FAIL);
		pGameObject->Sort_Component();
		pGameObject->m_pTransform->SetParent(trans);
		FAILED_CHECK_RETURN(pLayer->Add_GameObject(pObjTag, pGameObject), E_FAIL);
		return S_OK;
	}
	//ž�� ��������
	static HRESULT	Create(const _tchar * pObjTag, CLayer* pLayer, _vec3& vPos, CTransform* trans, _tchar* texturename, _int idx, _bool setAnim)
	{
		// �ʱ�ȭ
		LPDIRECT3DDEVICE9 pGraphicDev = Get_GraphicDev();
		CScene*	pScene = Get_Scene();
		// ����
		CGameObject* pGameObject = T::Create(pGraphicDev, vPos, trans, texturename, idx, setAnim);
		NULL_CHECK_RETURN(pGameObject, E_FAIL);
		pGameObject->Sort_Component();
		pGameObject->m_pTransform->SetParent(trans);
		FAILED_CHECK_RETURN(pLayer->Add_GameObject(pObjTag, pGameObject), E_FAIL);
		return S_OK;
	}
	//��׶��� ������Ʈ ������
	static HRESULT	Create(const _tchar * pObjTag, CLayer* pLayer, _vec3& vPos, _float fScale)
	{
		LPDIRECT3DDEVICE9 pGraphicDev = Get_GraphicDev();
		CGameObject* pGameObject = T::Create(pGraphicDev, vPos, fScale);
		NULL_CHECK_RETURN(pGameObject, E_FAIL);
		pGameObject->Sort_Component();
		FAILED_CHECK_RETURN(pLayer->Add_GameObject(pObjTag, pGameObject), E_FAIL);
		return S_OK;
	}
	static HRESULT	Create(const _tchar * pObjTag, CLayer* pLayer, _vec3& vPos, _float fScale, _float fAngle)
	{
		LPDIRECT3DDEVICE9 pGraphicDev = Get_GraphicDev();
		CGameObject* pGameObject = T::Create(pGraphicDev, vPos, fScale, fAngle);
		NULL_CHECK_RETURN(pGameObject, E_FAIL);
		pGameObject->Sort_Component();
		FAILED_CHECK_RETURN(pLayer->Add_GameObject(pObjTag, pGameObject), E_FAIL);
		return S_OK;
	}
	static HRESULT	Create(const _tchar* pObjTag, CLayer* pLayer, _vec3& vPos, _float fScale,_int iTexnum)
	{
		LPDIRECT3DDEVICE9 pGraphicDev = Get_GraphicDev();
		CGameObject* pGameObject = T::Create(pGraphicDev, vPos, fScale,iTexnum);
		NULL_CHECK_RETURN(pGameObject, E_FAIL);
		pGameObject->Sort_Component();
		FAILED_CHECK_RETURN(pLayer->Add_GameObject(pObjTag, pGameObject), E_FAIL);
		return S_OK;
	}
	static HRESULT	Create(const _tchar* pObjTag, CLayer* pLayer, _int iHP)
	{
		LPDIRECT3DDEVICE9 pGraphicDev = Get_GraphicDev();
		CGameObject* pGameObject = T::Create(pGraphicDev,iHP);
		NULL_CHECK_RETURN(pGameObject, E_FAIL);
		pGameObject->Sort_Component();
		FAILED_CHECK_RETURN(pLayer->Add_GameObject(pObjTag, pGameObject), E_FAIL);
		return S_OK;
	}
};

END