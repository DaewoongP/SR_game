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
	m_pTransform->m_bIsStatic = true;
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
	
	return 0;
}

_int CMoveBox::Update_Top(const _float & fTimeDelta)
{
	if (m_bDead)
		return OBJ_DEAD;
	ShootRay();
	Move(fTimeDelta);

	__super::Update_GameObject(fTimeDelta);

	Engine::Add_RenderGroup(RENDER_NONALPHA, this);
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
		m_bIsMoving = true;
		m_MovetoPos = _vec3(m_pTransform->m_vInfo[INFO_POS].x + m_MoveVec.x, m_pTransform->m_vInfo[INFO_POS].y + m_MoveVec.y, m_pTransform->m_vInfo[INFO_POS].z);
	}

	//�浹�� ���� �߻�! 
	__super::OnCollisionEnter(collision);
}

void CMoveBox::OnCollisionStay(const Collision * collision)
{
	//�ڽŰ� �浹�� ť�꿡�� �ڽ��� ������ ���¸� ������Ŵ.
	if (!lstrcmp(collision->otherObj->m_pTag, L"MoveCube"))
	{
		//���� �浹�Ѿְ� �������� �ʰ��־�?
		if (!dynamic_cast<CMoveBox*>(collision->otherObj)->m_bIsMoving)
		{
			CMoveBox* otherBox = dynamic_cast<CMoveBox*>(collision->otherObj);
			//���� ��������
			switch (collision->_dir)
			{
			case DIR_UP:
				otherBox->m_MoveVec = _vec3(0, 2, 0);
				break;
			case DIR_DOWN:
				otherBox->m_MoveVec = _vec3(0, -2, 0);
				break;
			case DIR_LEFT:
				otherBox->m_MoveVec = _vec3(-2, 0, 0);
				break;
			case DIR_RIGHT:
				otherBox->m_MoveVec = _vec3(2, 0, 0);
				break;
			}
			otherBox->m_bIsMoving = true;
			otherBox->m_MovetoPos =
				_vec3(otherBox->m_pTransform->m_vInfo[INFO_POS].x + otherBox->m_MoveVec.x,
					otherBox->m_pTransform->m_vInfo[INFO_POS].y + otherBox->m_MoveVec.y,
					otherBox->m_pTransform->m_vInfo[INFO_POS].z);
		}
	}

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
	m_pCollider->Set_BoundingBox({ 2.0f,2.0f,2.0f });
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

_bool CMoveBox::ShootRay()
{
	_vec3 centerpos = m_pTransform->m_vInfo[INFO_POS] + _vec3(-1, 0, 0);
	vector<RayCollision> _detectedCOL01 = Engine::Check_Collision_Ray(RAYCAST(centerpos, _vec3(1.f, 0, 0), 3.5f));
	if (_detectedCOL01.size() >= 2)
	{
		int a = 0;
		if (!lstrcmp(_detectedCOL01[1].tag, L"MapCube"))
		{
			m_bIsCol[DIR_LEFT] = true;
			m_MoveVec.x = 0; //return true;
		}
	}

	centerpos = m_pTransform->m_vInfo[INFO_POS] + _vec3(1, 0, 0);
	vector<RayCollision> _detectedCOL02 = Engine::Check_Collision_Ray(RAYCAST(centerpos, _vec3(-1.f, 0, 0), 3.5f));
	if (_detectedCOL02.size() >= 2)
	{
		int a = 0;
		if (!lstrcmp(_detectedCOL02[1].tag, L"MapCube"))
		{
			m_bIsCol[DIR_RIGHT] = true;
			m_MoveVec.x = 0; //return true;
		}
	}

	centerpos = m_pTransform->m_vInfo[INFO_POS] + _vec3(0, -1, 0);
	vector<RayCollision> _detectedCOL03 = Engine::Check_Collision_Ray(RAYCAST(centerpos, _vec3(0, 1, 0), 3.5f));
	if (_detectedCOL03.size() >= 2)
	{
		int a = 0;
		if (!lstrcmp(_detectedCOL03[1].tag, L"MapCube"))
		{
			m_bIsCol[DIR_UP] = true;
			m_MoveVec.y = 0; //return true;
		}
	}

	centerpos = m_pTransform->m_vInfo[INFO_POS] + _vec3(0, 1, 0);
	vector<RayCollision> _detectedCOL04 = Engine::Check_Collision_Ray(RAYCAST(centerpos, _vec3(0, -1, 0), 3.5f));
	if (_detectedCOL04.size() >= 2)
	{
		int a = 0;
		if (!lstrcmp(_detectedCOL04[1].tag, L"MapCube"))
		{
			m_bIsCol[DIR_DOWN] = true;
			m_MoveVec.y = 0; //return true;
		}
	}
	return false;
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
