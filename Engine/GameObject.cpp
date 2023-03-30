#include "stdafx.h"
#include "GameObject.h"
#include "Transform.h"
#include "Export_Function.h"

// 테스트용 주석

CGameObject::CGameObject(LPDIRECT3DDEVICE9 pGraphicDev)
	: m_pGraphicDev(pGraphicDev)
{
	m_pGraphicDev->AddRef();

	m_pTransform = dynamic_cast<CTransform*>(Engine::Clone_Proto(L"Transform", this));
	m_uMapComponent[ID_DYNAMIC].insert({ L"Transform", m_pTransform });

}

CGameObject::~CGameObject()
{
}

CComponent * CGameObject::Get_Component(const _tchar * pComponentTag, COMPONENTID eID)
{
	CComponent*		pComponent = Find_Component(pComponentTag, eID);
	NULL_CHECK_RETURN(pComponent, nullptr);

	return pComponent;
}

_int CGameObject::Update_GameObject(const _float & fTimeDelta)
{
	for (auto& iter : m_uMapComponent[ID_DYNAMIC])
		iter.second->Update_Component(fTimeDelta);

	return 0;
}

void CGameObject::LateUpdate_GameObject(void)
{
	for (auto& iter : m_uMapComponent[ID_DYNAMIC])
		iter.second->LateUpdate_Component();
}

void CGameObject::Render_GameObject(void)
{
	for (auto& iter : m_uMapComponent[ID_DYNAMIC])
		iter.second->Render_Component();
}

CComponent * CGameObject::Find_Component(const _tchar * pComponentTag, COMPONENTID eID)
{
	auto	iter = find_if(m_uMapComponent[eID].begin(), m_uMapComponent[eID].end(), CTag_Finder(pComponentTag));

	if (iter == m_uMapComponent[eID].end())
		return nullptr;

	return iter->second;
}

void CGameObject::Free(void)
{
	for (size_t i = 0; i < ID_END; ++i)
	{
		for_each(m_uMapComponent[i].begin(), m_uMapComponent[i].end(), CDeleteMap());
		m_uMapComponent[i].clear();
	}
	Safe_Release(m_pGraphicDev);
}
