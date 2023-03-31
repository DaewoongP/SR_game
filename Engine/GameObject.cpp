#include "stdafx.h"
#include "GameObject.h"
#include "Transform.h"
#include "Export_Function.h"

// �׽�Ʈ�� �ּ�

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
	//���� �浹�� ��ü�� �����ٵ� ���������� �ʴٸ� ���� X
	CRigidbody* _rigid;
	NULL_CHECK(other->m_pGameObject->Get_Component(L"Rigidbody", ID_DYNAMIC));
	_rigid = dynamic_cast<CRigidbody*>(other->m_pGameObject->Get_Component(L"Rigidbody", ID_DYNAMIC));
	

	//�浹�� ������ Ȯ���մϴ�.
	//������ �׳� �����ϰ� y�� ���������ְ���.

	//�ִ� ���� �浹�� ��ü�� ���ν�Ƽ��.
	_vec3 velo = _rigid->m_Velocity;
	//���� �浹�� ��ü�� ���ν�Ƽ���� Ư�� ���� ���� ���� ����� �����ְ���.
	if (_rigid->m_Velocity.y < 0)
	{
		_vec3 reaction = _vec3(0, _rigid->m_Velocity.y, 0);

		//���� �浹�� ��ü�� �������� velocirt�� y�� �˾Ƴ�. �װ� �־��ְ���
		_rigid->m_Velocity -= reaction;
	}
	//�浹�� ������� ����
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
