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

void CGameObject::OnCollisionEnter(const Collision * collsion)
{
	int a = 0;
}

void CGameObject::OnCollisionStay(const Collision * collision)
{	
}

void CGameObject::OnCollisionExit(const Collision * collision)
{
}

void CGameObject::OnTriggerEnter(const CCollider * other)
{
}

void CGameObject::OnTriggerStay(const CCollider * other)
{
	//나랑 충돌한 물체가 리짓바디를 가지고있지 않다면 실행 X
	CRigidbody* _rigid;
	NULL_CHECK(other->m_pGameObject->Get_Component(L"Rigidbody", ID_DYNAMIC));
	_rigid = dynamic_cast<CRigidbody*>(other->m_pGameObject->Get_Component(L"Rigidbody", ID_DYNAMIC));
	

	//충돌의 법선을 확인합니다.
	//지금은 그냥 무식하게 y만 고정시켜주겠음.

	//애는 나랑 충돌한 물체의 벨로시티임.
	_vec3 velo = _rigid->m_Velocity;
	//나랑 충돌한 물체의 벨로시티에서 특정 축을 향한 힘만 남기고 날려주겠음.
	if (_rigid->m_Velocity.y < 0)
	{
		_vec3 reaction = _vec3(0, _rigid->m_Velocity.y, 0);

		//나랑 충돌한 물체에 적용중인 velocirt의 y를 알아냄. 그걸 넣어주겠음
		_rigid->m_Velocity -= reaction;
	}
	//충돌시 마찰계수 적용
	_rigid->m_Velocity.x *= 0.95f;
}

void CGameObject::OnTirggerExit(const CCollider * other)
{
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
