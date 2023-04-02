#include "stdafx.h"
#include "Component.h"
#include "GameObject.h"


CComponent::CComponent() 
	: m_pGraphicDev(nullptr), m_bClone(false)
{
}

CComponent::CComponent(LPDIRECT3DDEVICE9 pGraphicDev)
	: m_pGraphicDev(pGraphicDev)
	, m_bClone(false)
{
	m_pGraphicDev->AddRef();
}

CComponent::CComponent(const CComponent & rhs)
	: m_pGraphicDev(rhs.m_pGraphicDev),
	  m_bClone(true)
{
	m_pGraphicDev->AddRef();
}


CComponent::~CComponent()
{
}

Engine::CComponent* Engine::CComponent::Get_Component(const _tchar* pComponentTag, COMPONENTID eID)
{
	CComponent*		pComponent = m_pGameObject->Get_Component(pComponentTag, eID);
	NULL_CHECK_RETURN(pComponent, nullptr);

	return pComponent;
}

void CComponent::SetOwner(CGameObject * gameObject)
{
	m_pGameObject = gameObject;
}

void Engine::CComponent::Free(void)
{
	Safe_Release(m_pGraphicDev);
}
