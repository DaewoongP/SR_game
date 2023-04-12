#include "stdafx.h"
#include "Layer.h"

#include "Export_Function.h"

CLayer::CLayer()
	: bPreState(g_Is2D)
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

CGameObject * CLayer::Get_GameObject(const _tchar * pObjTag)
{
	auto iter = find_if(m_uMapObject.begin(), m_uMapObject.end(), CTag_Finder(pObjTag));

	if (iter == m_uMapObject.end())
		return nullptr;

	return iter->second;
}

HRESULT CLayer::Add_GameObject(const _tchar * pObjTag, CGameObject * pGameObject)
{
	if (nullptr == pGameObject)
		return E_FAIL;

	pGameObject->Set_Tag(pObjTag);
	m_uMapObject.insert({ pObjTag, pGameObject });

	return S_OK;
}

HRESULT CLayer::Ready_Layer(void)
{
	return S_OK;
}

_int CLayer::Update_Layer(const _float & fTimeDelta)
{
	for (auto iter = m_uMapObject.begin(); iter != m_uMapObject.end();)
	{
		_int iResult = OBJ_NOEVENT;
		if (g_Is2D)
			iter->second->Update_Too(fTimeDelta);
		else
			iter->second->Update_Top(fTimeDelta);

		iResult = iter->second->Update_GameObject(fTimeDelta);

		if (OBJ_DEAD == iResult)
		{
			Engine::Delete_Collider(iter->second);
			Safe_Release(iter->second);
			iter = m_uMapObject.erase(iter);
		}
		else
			++iter;
	}

	return 0;
}

void CLayer::LateUpdate_Layer(void)
{
	for (auto& iter : m_uMapObject)
	{
		if (g_Is2D)
			iter.second->LateUpdate_Too();
		else
			iter.second->LateUpdate_Top();
		iter.second->LateUpdate_GameObject();
		// 트리거 호출
		if (bPreState != g_Is2D)
		{
			iter.second->SwapTrigger();
		}
	}
	bPreState = g_Is2D;
}

void CLayer::Delete_In_Layer()
{
	for (auto& iter = m_uMapObject.begin(); iter != m_uMapObject.end(); ++iter)
	{
		if (!lstrcmp(L"MapCube", iter->first) ||
			!lstrcmp(L"Toodee", iter->first) ||
			!lstrcmp(L"Topdee", iter->first))
			continue;
		iter->second->m_bDead = true;
	}	
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
