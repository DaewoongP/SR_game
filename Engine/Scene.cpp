#include "stdafx.h"
#include "Scene.h"

CScene::CScene(LPDIRECT3DDEVICE9 pGraphicDev)
	: m_pGraphicDev(pGraphicDev)
{
	m_pGraphicDev->AddRef();
}

CScene::~CScene()
{
}

CComponent * CScene::Get_Component(const _tchar * pLayerTag, const _tchar * pObjTag, const _tchar * pComponentTag, COMPONENTID eID)
{
	auto	iter = find_if(m_uMapLayer.begin(), m_uMapLayer.end(), CTag_Finder(pLayerTag));

	if (iter == m_uMapLayer.end())
		return nullptr;

	return iter->second->Get_Component(pObjTag, pComponentTag, eID);
}

CLayer * CScene::Get_Layer(const _tchar * pLayerTag)
{
	auto	iter = find_if(m_uMapLayer.begin(), m_uMapLayer.end(), CTag_Finder(pLayerTag));

	if (iter == m_uMapLayer.end())
		return nullptr;

	return iter->second;
}

CGameObject * CScene::Get_GameObject(const _tchar * pLayerTag, const _tchar * pObjTag)
{
	auto	iter = find_if(m_uMapLayer.begin(), m_uMapLayer.end(), CTag_Finder(pLayerTag));

	if (iter == m_uMapLayer.end())
		return nullptr;

	return iter->second->Get_GameObject(pObjTag);
}
size_t CScene::Find_ClassName(const _tchar* pName)
{
	size_t cnt = 0;
	// 앞의 클래스이름만 비교해서 찾아야함
	auto iter = find_if(m_mapClassName.begin(), m_mapClassName.end(), [&](auto& pair) {
		_tchar temp[MAX_STR] = L"";
		wcsncpy_s(temp, wcslen(pair.first) - 1, pair.first, _TRUNCATE);
		return !lstrcmpW(pName, temp);
	});

	if (iter == m_mapClassName.end())
	{
		m_mapClassName.insert({ pName, cnt });
		return cnt;
	}
	else
	{
		cnt = iter->second;
		m_mapClassName.erase(iter);
		m_mapClassName.insert({ pName, ++cnt });
		return cnt;
	}
}

HRESULT CScene::Ready_Scene(void)
{
	return S_OK;
}

_int CScene::Update_Scene(const _float & fTimeDelta)
{
	_int iResult = 0;

	for (auto& iter : m_uMapLayer)
	{
		iResult = iter.second->Update_Layer(fTimeDelta);

		if (iResult & 0x80000000)
			return iResult;
	}

	return iResult;
}

void CScene::LateUpdate_Scene(void)
{
	for (auto& iter : m_uMapLayer)
		iter.second->LateUpdate_Layer();
}

void Engine::CScene::Free(void)
{
	for_each(m_uMapLayer.begin(), m_uMapLayer.end(), CDeleteMap());
	m_uMapLayer.clear();
	/*for_each(m_mapClassName.begin(), m_mapClassName.end(), [](auto& pair) {
		delete pair.first;
	});*/
	Safe_Release(m_pGraphicDev);
}
