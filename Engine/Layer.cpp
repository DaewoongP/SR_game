#include "stdafx.h"
#include "Layer.h"


CLayer::CLayer()
{
}

CLayer::~CLayer()
{
}

CComponent * CLayer::Get_Component(const _tchar* pObjTag, const _tchar * pComponentTag, COMPONENTID eID)
{
	auto iter = find_if(m_uMapObject.begin(), m_uMapObject.end(), CTag_Finder(pObjTag));

	if (iter == m_uMapObject.end())
		return nullptr;
	
	return iter->second->Get_Component(pComponentTag, eID);
}

HRESULT CLayer::Add_GameObject(const _tchar * pObjTag, CGameObject * pGameObject)
{
	if (nullptr == pGameObject)
		return E_FAIL;

	m_uMapObject.insert({ pObjTag, pGameObject });

	return S_OK;
}

HRESULT CLayer::Ready_Layer(void)
{
	return S_OK;
}

_int CLayer::Update_Layer(const _float & fTimeDelta)
{
	for (auto& iter : m_uMapObject)
		iter.second->Update_GameObject(fTimeDelta);

	return 0;
}

void CLayer::LateUpdate_Layer(void)
{
	for (auto& iter : m_uMapObject)
		iter.second->LateUpdate_GameObject();
}


CLayer * CLayer::Create(void)
{
	CLayer *	pInstance = new CLayer;

	if (FAILED(pInstance->Ready_Layer()))
	{
		Safe_Release(pInstance);
		MSG_BOX("Layer Create Failed");
	}

	return pInstance;
}

void CLayer::Free(void)
{
	for_each(m_uMapObject.begin(), m_uMapObject.end(), CDeleteMap());
	m_uMapObject.clear();
}
