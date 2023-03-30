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

	Safe_Release(m_pGraphicDev);
}
