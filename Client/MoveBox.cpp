#include "stdafx.h"
#include "MoveBox.h"

#include "Player02.h"
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

	m_handleState = CH_NONE;
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
	MoveToPos(fTimeDelta);
	if(m_handleState == CH_START || m_handleState == CH_END)
	{
		
	}
	else
	{
		ShootRay();
		Move(fTimeDelta);
	}
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
	//_vec3 v1 = m_pTransform->m_vInfo[INFO_POS] + _vec3(0, 0, 0); // origin
	//_vec3 v2 = v1+ _vec3(2.5,0,0); // dir

	//_vec3 v3 = v1 + _vec3(-2.5, 0, 0);
	//_vec3 v4 = v1 + _vec3(0, 2.5, 0);
	//_vec3 v5 = v1 + _vec3(0, -2.5, 0);

	//_matrix matView, matProj;
	//m_pGraphicDev->GetTransform(D3DTS_VIEW, &matView);
	//m_pGraphicDev->GetTransform(D3DTS_PROJECTION, &matProj);
	//_matrix matworld;
	//D3DXMatrixIdentity(&matworld);
	//m_pLine->Set_Line(v1, v2, D3DXCOLOR(1.f, 1.f, 0.f, 1.f));
	//m_pLine->Draw_Line(matworld, matView, matProj);
	//m_pLine->Set_Line(v1, v3, D3DXCOLOR(1.f, 1.f, 0.f, 1.f));
	//m_pLine->Draw_Line(matworld, matView, matProj);
	//m_pLine->Set_Line(v1, v4, D3DXCOLOR(1.f, 1.f, 0.f, 1.f));
	//m_pLine->Draw_Line(matworld, matView, matProj);
	//m_pLine->Set_Line(v1, v5, D3DXCOLOR(1.f, 1.f, 0.f, 1.f));
	//m_pLine->Draw_Line(matworld, matView, matProj);
	
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
		DoRayToDir(collision->_dir);
	}

	//충돌시 레이 발생! 
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
	m_pCollider->Set_BoundingBox({ 2.0f,2.0f,2.0f });
	m_pCollider->Set_Group(COL_ENV);

	pComponent = m_pLine = dynamic_cast<CLine*>(Engine::Clone_Proto(L"Line", this));
	NULL_CHECK_RETURN(m_pLine, E_FAIL);
	m_uMapComponent[ID_STATIC].insert({ L"Line", pComponent });

	return S_OK;
}

void CMoveBox::Move(const _float & fTimeDelta)
{
	if (m_bIsMoving)
		if (IsMoveDone(fTimeDelta))
			return;
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
	//거리 이용 도달했는지 알려주는 코드
	if (D3DXVec3Length(&_vec3(m_pTransform->m_vInfo[INFO_POS] - m_MovetoPos)) < 0.15f)
	{
		m_pTransform->m_vInfo[INFO_POS] = m_MovetoPos;
		m_bIsMoving = false;

		return false;
	}
	return true;
}

void CMoveBox::ShootRay()
{
	CheckColAble(_vec3(1, 0, 0), 2.5f, DIR_LEFT);
	CheckColAble(_vec3(-1, 0, 0), 2.5f, DIR_RIGHT);
	CheckColAble(_vec3(0, 1, 0), 2.5f, DIR_UP);
	CheckColAble(_vec3(0, -1, 0), 2.5f, DIR_DOWN);
}

void CMoveBox::CheckColAble(_vec3 vdir, float len, COL_DIR edir)
{
	_vec3 centerpos = m_pTransform->m_vInfo[INFO_POS];
	vector<RayCollision> _detectedCOL = Engine::Check_Collision_Ray(RAYCAST(centerpos, vdir, len), m_pCollider);
	if (_detectedCOL.size() == 1)
	{
		if (!lstrcmp(_detectedCOL[0].tag, L"MapCube"))
		{
			m_bIsCol[edir] = true;
		}
		else
			m_bIsCol[edir] = false;

		if (!lstrcmp(_detectedCOL[0].tag, L"MoveCube"))
			m_bIsCol[edir] = dynamic_cast<CMoveBox*>(_detectedCOL[0].col->m_pGameObject)->m_bIsCol[edir];
	}
	else
		m_bIsCol[edir] = false;
}

_bool CMoveBox::DoRayToDir(COL_DIR  dir)
{
	//들어온 방향으로 레이를 쏩니다.
	_vec3 centerpos = m_pTransform->m_vInfo[INFO_POS];
	vector<RayCollision> _detectedCOL;
	switch (dir)
	{
	case DIR_UP:
		_detectedCOL = Engine::Check_Collision_Ray(RAYCAST(centerpos, _vec3(0, -1, 0), 2.5f), m_pCollider);
		break;
	case DIR_DOWN:
		_detectedCOL = Engine::Check_Collision_Ray(RAYCAST(centerpos, _vec3(0, 1, 0), 2.5f), m_pCollider);
		break;
	case DIR_LEFT:
		_detectedCOL = Engine::Check_Collision_Ray(RAYCAST(centerpos, _vec3(1.f, 0, 0), 2.5f), m_pCollider);
		break;
	case DIR_RIGHT:
		_detectedCOL = Engine::Check_Collision_Ray(RAYCAST(centerpos, _vec3(-1.f, 0, 0), 2.5f), m_pCollider);
		break;
	}
	//거기에 movecube 검출되면 그 친구에게 드로우 레이를 쏩니다.
	if (_detectedCOL.size() == 1)
	{
		if (!lstrcmp(_detectedCOL[0].tag, L"MoveCube"))
		{
			m_bIsCol[dir] = dynamic_cast<CMoveBox*>(_detectedCOL[0].col->m_pGameObject)->m_bIsCol[dir];
				
			if (dynamic_cast<CMoveBox*>(_detectedCOL[0].col->m_pGameObject)->DoRayToDir(dir))
			{
				SetMovePos(dir);
				return true;
			}
			//거짓이라면 암것도 안합니다.
			return false;
		}
	}

	SetMovePos(dir);	
	return true;
}

void CMoveBox::SetMovePos(COL_DIR dir)
{
	switch (dir)
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

void CMoveBox::MoveToPos(const _float& fTimeDelta)
{
	switch (m_handleState)
	{
	case Engine::CH_NONE:
		//암것도 없음
		break;
	case Engine::CH_START:
		//머리위로
	{
		dynamic_cast<CPlayer02*>(m_Target)->Player02StateChange(TD_SOMETHING);
		_vec3 vec = m_TargetPos - m_pTransform->m_vInfo[INFO_POS];
		if (D3DXVec3Length(&vec)<0.3f)
		{
			m_pTransform->m_vInfo[INFO_POS] = m_TargetPos;
			dynamic_cast<CPlayer02*>(m_Target)->Player02StateChange(TD_MOVE);
			m_handleState = CH_ING;
			return;
		}
		D3DXVec3Normalize(&vec, &vec);
		m_pTransform->m_vInfo[INFO_POS] += vec*m_fSpeed*fTimeDelta;
		break;
	}
		
	case Engine::CH_ING:
	{//플레이어 머리위 위치로 고정
		m_pTransform->m_vInfo[INFO_POS] = m_Target->m_pTransform->m_vInfo[INFO_POS] + _vec3(0,0,-4);
		break;
	}
	case Engine::CH_END:
	{
		dynamic_cast<CPlayer02*>(m_Target)->Player02StateChange(TD_SOMETHING);
		_vec3 vec = m_TargetPos - m_pTransform->m_vInfo[INFO_POS];
		if (D3DXVec3Length(&vec)<0.3f)
		{
			m_pTransform->m_vInfo[INFO_POS] = m_TargetPos;
			dynamic_cast<CPlayer02*>(m_Target)->Player02StateChange(TD_MOVE);
			m_handleState = CH_NONE;
			return;
		}
		D3DXVec3Normalize(&vec, &vec);
		m_pTransform->m_vInfo[INFO_POS] += vec*m_fSpeed*fTimeDelta;
		break;
		}
	}
}

void CMoveBox::SetTarget(_vec3 pos, CGameObject * obj)
{
	m_TargetPos = pos;
	m_Target = obj;
	m_handleState = static_cast<CUBE_HANDING>((int)(m_handleState)+1);
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
