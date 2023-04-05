#include "stdafx.h"
#include "MoveBox.h"

#include "Export_Function.h"
CMoveBox::CMoveBox(LPDIRECT3DDEVICE9 pGraphicDev)
	:CCube(pGraphicDev)
{
}

CMoveBox::~CMoveBox()
{
}

HRESULT CMoveBox::Ready_GameObject(void)
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	m_pTransform->m_vScale = { 1.f, 1.f, 1.f };
	m_pTransform->m_vInfo[INFO_POS] = _vec3(20, 10, 10);
	m_pTransform->m_bIsStatic = false;
	m_pCollider->Set_Group(COL_OBJ);
	m_MovetoPos = m_pTransform->m_vInfo[INFO_POS];
	return S_OK;
}

_int CMoveBox::Update_GameObject(const _float & fTimeDelta)
{
	return 0;
}

_int CMoveBox::Update_Too(const _float & fTimeDelta)
{
	if (m_bDead)
		return OBJ_DEAD;
	if (m_pTag)
		int a = 1;

	__super::Update_GameObject(fTimeDelta);

	Engine::Add_RenderGroup(RENDER_NONALPHA, this);

	m_pTransform->m_bIsStatic = true;
	return 0;
}

_int CMoveBox::Update_Top(const _float & fTimeDelta)
{
	if (m_bDead)
		return OBJ_DEAD;
	if (m_pTag)
		int a = 1;
	Move(fTimeDelta);
	__super::Update_GameObject(fTimeDelta);

	Engine::Add_RenderGroup(RENDER_NONALPHA, this);
	m_pTransform->m_bIsStatic = false;
	return 0;
}

void CMoveBox::LateUpdate_GameObject(void)
{
	
}

void CMoveBox::LateUpdate_Too()
{
	CGameObject::LateUpdate_GameObject();
}

void CMoveBox::LateUpdate_Top()
{
	CGameObject::LateUpdate_GameObject();
}

void CMoveBox::Render_GameObject(void)
{
	
}

void CMoveBox::Render_Too()
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransform->Get_WorldMatrixPointer());

	m_pTextureCom->Set_Texture();

	m_pBufferCom->Render_Buffer();

	CGameObject::Render_GameObject();
}

void CMoveBox::Render_Top()
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransform->Get_WorldMatrixPointer());

	m_pTextureCom->Set_Texture();

	m_pBufferCom->Render_Buffer();

	CGameObject::Render_GameObject();
}

void CMoveBox::OnCollisionEnter(const Collision * collision)
{
	if (!lstrcmp(collision->otherObj->m_pTag, L"Player02"))
	{
		//������ ���ͼ� �о��ش�.
		switch (collision->_dir)
		{
		case DIR_UP:
			m_MoveVec = _vec3(0, -2, 0);
			break;
		case DIR_DOWN:
			m_MoveVec = _vec3(0, 2, 0);
			break;
		case DIR_LEFT:
			m_MoveVec = _vec3(2, 0, 0);
			break;
		case DIR_RIGHT:
			m_MoveVec = _vec3(-2, 0, 0);
			break;
		}
		m_MovetoPos = _vec3(m_pTransform->m_vInfo[INFO_POS].x + m_MoveVec.x, m_pTransform->m_vInfo[INFO_POS].y + m_MoveVec.y, m_pTransform->m_vInfo[INFO_POS].z);
		m_bIsMoving = true;
	}
	__super::OnCollisionEnter(collision);
}

void CMoveBox::OnCollisionStay(const Collision * collision)
{
	__super::OnCollisionStay(collision);
}

void CMoveBox::OnCollisionExit(const Collision * collision)
{
	__super::OnCollisionExit(collision);
}


HRESULT CMoveBox::Add_Component(void)
{
	CComponent*		pComponent = nullptr;

	pComponent = m_pBufferCom = dynamic_cast<CCubeTex*>(Engine::Clone_Proto(L"CubeTex", this));
	NULL_CHECK_RETURN(m_pBufferCom, E_FAIL);
	m_uMapComponent[ID_STATIC].insert({ L"CubeTex", pComponent });

	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Texture_Cube", this));
	NULL_CHECK_RETURN(m_pTextureCom, E_FAIL);
	m_uMapComponent[ID_STATIC].insert({ L"Texture_Cube", pComponent });

	pComponent = m_pCollider = dynamic_cast<CCollider*>(Engine::Clone_Proto(L"Collider", this));
	NULL_CHECK_RETURN(m_pCollider, E_FAIL);
	m_uMapComponent[ID_DYNAMIC].insert({ L"Collider", pComponent });
	m_pCollider->Set_BoundingBox({ 1.99f,1.99f,1.99f });
	m_pCollider->Set_Group(COL_ENV);

	return S_OK;
}

void CMoveBox::Move(const _float & fTimeDelta)
{
	if (m_bIsMoving)
		if (IsMoveDone(fTimeDelta))
			return;
		else
		{
			if (prePos == m_pTransform->m_vInfo[INFO_POS])
			{
				m_MovetoPos = prePos;
			}
		}
}

_bool CMoveBox::IsMoveDone(const _float & fTimeDelta)
{
	_vec3 dir;
	D3DXVec3Normalize(&dir, &_vec3(m_MovetoPos - m_pTransform->m_vInfo[INFO_POS]));
	m_pTransform->m_vInfo[INFO_POS] += dir*m_fSpeed*fTimeDelta;

	if (prePos == m_pTransform->m_vInfo[INFO_POS])
	{
		m_MovetoPos = prePos;
	}
	prePos = m_pTransform->m_vInfo[INFO_POS];
	//�Ÿ� �̿� �����ߴ��� �˷��ִ� �ڵ�
	if (D3DXVec3Length(&_vec3(m_pTransform->m_vInfo[INFO_POS] - m_MovetoPos)) < 0.15f)
	{
		m_pTransform->m_vInfo[INFO_POS] = m_MovetoPos;
		return false;
	}
	return true;
}

CMoveBox * CMoveBox::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CMoveBox*		pInstance = new CMoveBox(pGraphicDev);

	if (FAILED(pInstance->Ready_GameObject()))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CMoveBox::Free(void)
{
	__super::Free();
}
