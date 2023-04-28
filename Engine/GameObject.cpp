#include "stdafx.h"
#include "GameObject.h"
#include "Transform.h"
#include "Export_Function.h"

CGameObject::CGameObject(LPDIRECT3DDEVICE9 pGraphicDev)
	: m_pGraphicDev(pGraphicDev), m_bDead(false), m_pTag(L"")
{	
	m_pGraphicDev->AddRef();
	m_bUseRender = true;
	m_bUseUpdate = true;
	m_pTransform = dynamic_cast<CTransform*>(Engine::Clone_Proto(L"Transform", this));
	m_vecComponent[ID_DYNAMIC].push_back({ L"Transform", m_pTransform });
}

CGameObject::~CGameObject()
{
}

CComponent * CGameObject::Get_Component(const _tchar * pComponentTag, COMPONENTID eID)
{
    CComponent*        pComponent = Find_Component(pComponentTag, eID);
    if (pComponent == nullptr)return nullptr;

    return pComponent;
}

void CGameObject::Set_Tag(const _tchar * pTag)
{
	const _tchar* pCutTag = nullptr;
	pCutTag = wcschr(pTag, '_');

	if (nullptr == pCutTag)
	{
		wcscpy_s(m_pTag, pTag);
		return;
	}
	// 입력한 사이즈만큼 문자열 복사
	wcsncpy_s(m_pTag, wcslen(pTag) - wcslen(pCutTag) + 1, pTag, _TRUNCATE );
	NULL_CHECK(m_pTag);
}

_int CGameObject::Update_GameObject(const _float & fTimeDelta)
{
	if (!m_bUseUpdate)
		return 0;

	for (auto& iter : m_vecComponent[ID_DYNAMIC])
		iter.second->Update_Component(fTimeDelta);

	return 0;
}

void CGameObject::LateUpdate_GameObject(void)
{
	for (auto& iter : m_vecComponent[ID_DYNAMIC])
		iter.second->LateUpdate_Component();
}

void CGameObject::Render_GameObject(void)
{
	if (!m_bUseRender)
		return;
	for (auto& iter : m_vecComponent[ID_DYNAMIC])
	{
		m_pGraphicDev->SetTexture(0, nullptr);
		iter.second->Render_Component();
	}
}

void CGameObject::OnCollisionEnter(const Collision * collision)
{
}

void CGameObject::OnCollisionStay(const Collision * collision)
{
	if (!m_pTransform->m_bIsStatic)
		return;
	//나랑 충돌한 물체가 리짓바디를 가지고있지 않다면 실행 X
	CRigidbody* _rigid;

	if ((collision->otherObj->Get_Component(L"Rigidbody", ID_DYNAMIC)) == nullptr) return;
	_rigid = dynamic_cast<CRigidbody*>(collision->otherObj->Get_Component(L"Rigidbody", ID_DYNAMIC));

	//velocity / trans 둘다 밀어내기를 진행해줘야함.
	_vec3 reaction = _vec3(0, 0, 0);

	switch (collision->_dir)
	{
	case DIR_UP:
		if (_rigid->m_Velocity.y < 0)
			reaction = _vec3(0, _rigid->m_Velocity.y, 0);
		_rigid->m_Velocity.x *= 0.8f;
		break;
	case DIR_DOWN:
		if (_rigid->m_Velocity.y > 0)
			reaction = _vec3(0, _rigid->m_Velocity.y, 0);
		_rigid->m_Velocity.x *= 0.8f;
		break;
	case DIR_LEFT:
		if (_rigid->m_Velocity.x > 0)
			reaction = _vec3(_rigid->m_Velocity.x, 0, 0);
		break;
	case DIR_RIGHT:
		if (_rigid->m_Velocity.x < 0)
			reaction = _vec3(_rigid->m_Velocity.x, 0, 0);
		break;
	}
	_rigid->m_Velocity -= reaction;
 	CTransform* trans_other = collision->otherObj->m_pTransform;
	CCollider* collider_other = collision->otherCol;
	if (collider_other->m_bIsTrigger)
		return;
	//현재 게임 오브젝트의 콜라이더를 가져옵니다.
  	CCollider* collider_this = dynamic_cast<CCollider*>(this->Get_Component(L"Collider", ID_DYNAMIC));

	if (trans_other->m_bIsStatic)
		return;

	//각 콜라이더의 충돌 영역을 가져옴.
	_vec3 center_this = collider_this->Get_BoundCenter();
	_vec3 size_this = collider_this->Get_BoundSize();
	_vec3 center_other = collider_other->Get_BoundCenter();
	_vec3 size_other = collider_other->Get_BoundSize();

	// 콜라이더 자체 오프셋
	_vec3 offset_other = collider_other->Get_BoundOffset();

	//충돌 영역을 이용한 위치 보정값
	_float min_x = center_this.x - (size_this.x*0.5f + size_other.x*0.5f);
	_float min_y = center_this.y - (size_this.y*0.5f + size_other.y*0.5f);
	_float max_y = center_this.y + (size_this.y*0.5f + size_other.y*0.5f);
	_float max_x = center_this.x + (size_this.x*0.5f + size_other.x*0.5f);


	if ((collision->_dir == DIR_LEFT&&trans_other->m_vInfo[INFO_POS].x + offset_other.x <= min_x) ||
		(collision->_dir == DIR_RIGHT&&trans_other->m_vInfo[INFO_POS].x + offset_other.x >= max_x) ||
		(collision->_dir == DIR_DOWN&&trans_other->m_vInfo[INFO_POS].y + offset_other.y <= min_y) ||
		(collision->_dir == DIR_UP&&trans_other->m_vInfo[INFO_POS].y + offset_other.y >= max_y)
		)
		return;

	if (collision->_dir == DIR_UP &&center_other.y > min_y)
		trans_other->m_vInfo[INFO_POS].y = max_y - offset_other.y;
	else if (collision->_dir == DIR_DOWN&&center_other.y < max_y)
		trans_other->m_vInfo[INFO_POS].y = min_y - offset_other.y;
	else if (collision->_dir == DIR_LEFT&&center_other.x < max_x)
		trans_other->m_vInfo[INFO_POS].x = min_x - offset_other.x;
	else if (collision->_dir == DIR_RIGHT&&center_other.x > min_x)
		trans_other->m_vInfo[INFO_POS].x = max_x - offset_other.x;
		
	
}

void CGameObject::OnCollisionExit(const Collision * collision)
{
}

void CGameObject::Sort_Component()
{
	if (!m_vecComponent->empty())
	{
		CComponent* pTransform = nullptr;
		CComponent* pCollider = nullptr;
		CComponent* pRigidbody = nullptr;

		for (auto& pair = m_vecComponent[ID_DYNAMIC].begin(); pair != m_vecComponent[ID_DYNAMIC].end();)
		{
			if (!lstrcmp(pair->first, L"Transform"))
			{
				pTransform = pair->second;
				pair = m_vecComponent[ID_DYNAMIC].erase(pair);
			}
			else if (!lstrcmp(pair->first, L"Collider"))
			{
				pCollider = pair->second;
				pair = m_vecComponent[ID_DYNAMIC].erase(pair);
			}
			else if (!lstrcmp(pair->first, L"Rigidbody"))
			{
				pRigidbody = pair->second;
				pair = m_vecComponent[ID_DYNAMIC].erase(pair);
			}
			else
				++pair;
		}
		if (nullptr != pTransform)
			m_vecComponent[ID_DYNAMIC].push_back({ L"Transform", pTransform });
		if (nullptr != pCollider)
			m_vecComponent[ID_DYNAMIC].push_back({ L"Collider", pCollider });
		if (nullptr != pRigidbody)
			m_vecComponent[ID_DYNAMIC].push_back({ L"Rigidbody", pRigidbody });
	}
}


CComponent * CGameObject::Find_Component(const _tchar * pComponentTag, COMPONENTID eID)
{
	auto	iter = find_if(m_vecComponent[eID].begin(), m_vecComponent[eID].end(), CTag_Finder(pComponentTag));

	if (iter == m_vecComponent[eID].end())
		return nullptr;

	return iter->second;
}

void CGameObject::Free(void)
{
	for (size_t i = 0; i < ID_END; ++i)
	{
		for_each(m_vecComponent[i].begin(), m_vecComponent[i].end(), CDeleteMap());
		m_vecComponent[i].clear();
	}
	Safe_Release(m_pGraphicDev);
}
	