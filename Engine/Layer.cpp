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
	m_uMapObject.push_back({ pObjTag, pGameObject });

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

		if (STAGE_END == iResult)
		{
			// 콜리전 삭제 필수
			Engine::Clear_Collision();
			return STAGE_END;
		}

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
		// Æ®¸®°Å È£Ãâ
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

HRESULT CLayer::Delete_LastObject(CGameObject* pGameObject)
{
	if (!lstrcmp(pGameObject->m_pTag, L"LaserTurret"))
	{
		for (auto& iter = m_uMapObject.rbegin(); iter != m_uMapObject.rend(); ++iter)
		{
			if (!lstrcmp(iter->second->m_pTag, L"LaserTurret"))
			{
				Engine::Delete_Collider(iter->second);
				Safe_Release(iter->second);
				// reverse_iterator 삭제법.
				std::advance(iter, 1);
				m_uMapObject.erase(iter.base());
				return S_OK;
			}
		}
	}
	else
	{
		for (auto& iter = m_uMapObject.begin(); iter != m_uMapObject.end();)
		{
			if (!lstrcmp(iter->second->m_pTag, L"Laser"))
			{
				Engine::Delete_Collider(iter->second);
				Safe_Release(iter->second);
				iter = m_uMapObject.erase(iter);
			}
			else
				++iter;
		}
		if (pGameObject == m_uMapObject.back().second)
		{
			Engine::Delete_Collider(m_uMapObject.back().second);
			Safe_Release(m_uMapObject.back().second);
			m_uMapObject.pop_back();

			return S_OK;
		}
		else
			return E_FAIL;
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
