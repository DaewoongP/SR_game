#include "stdafx.h"
#include "MoveCube.h"

#include "Topdee.h"
#include "Tookee.h"
#include "PortalCube.h"
#include "Export_Function.h"
CMoveCube::CMoveCube(LPDIRECT3DDEVICE9 pGraphicDev)
	:CCube(pGraphicDev)
{
}

CMoveCube::~CMoveCube()
{
}

HRESULT CMoveCube::Ready_GameObject(_vec3& vPos)
{
	__super::Ready_GameObject(vPos);
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);
	
	m_handleState = CH_NONE;
	m_pTransform->m_vScale = { 1.f, 1.f, 1.f };
	m_pTransform->m_bIsStatic = true;
	m_pCollider->Set_Group(COL_OBJ);
	m_MovetoPos = m_pTransform->m_vInfo[INFO_POS] = vPos;
	m_pCollider->Set_BoundingBox();

	m_bIsCol[DIR_END] = { 0 };
	m_bIsStone = false;
	m_bIsFall = false;

	return S_OK;
}

_int CMoveCube::Update_GameObject(const _float & fTimeDelta)
{
	if (m_bDead)
		return OBJ_DEAD;
	Engine::Add_RenderGroup(RENDER_NONALPHA, this);
	__super::Update_GameObject(fTimeDelta);
	return 0;
}


_int CMoveCube::Update_Top(const _float & fTimeDelta)
{
	MoveToPos(fTimeDelta);

	if (m_handleState == CH_START || m_handleState == CH_END)
		return 0;

	if(!m_bIsFall)
		ShootRay();
	Move(fTimeDelta);
	return 0;
}

void CMoveCube::LateUpdate_GameObject(void)
{
	__super::LateUpdate_GameObject();
}

void CMoveCube::Render_GameObject(void)
{
	__super::Render_GameObject();
}

void CMoveCube::OnCollisionEnter(const Collision * collision)
{
	if (!g_Is2D&&
		(!lstrcmp(collision->otherObj->m_pTag, L"Topdee")||
		 !lstrcmp(collision->otherObj->m_pTag, L"Tookee")||
		!lstrcmp(collision->otherObj->m_pTag, L"Thirddee")
			)&&m_handleState==CH_NONE&&!m_bIsCol[collision->_dir])
		DoRayToDir(collision->_dir);		

	__super::OnCollisionEnter(collision);
}

void CMoveCube::OnCollisionStay(const Collision * collision)
{
	__super::OnCollisionStay(collision);
}

void CMoveCube::OnCollisionExit(const Collision * collision)
{
	__super::OnCollisionExit(collision);
}


HRESULT CMoveCube::Add_Component(void)
{
	CComponent*		pComponent = nullptr;

	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Move_Cube", this));
	NULL_CHECK_RETURN(m_pTextureCom, E_FAIL);
	m_vecComponent[ID_STATIC].push_back({ L"Move_Cube", pComponent });

	pComponent = m_pLine = dynamic_cast<CLine*>(Engine::Clone_Proto(L"Line", this));
	NULL_CHECK_RETURN(m_pLine, E_FAIL);
	m_vecComponent[ID_STATIC].push_back({ L"Line", pComponent });
	
	return S_OK;
}

void CMoveCube::Move(const _float & fTimeDelta)
{
	if (m_bIsMoving)
		if (IsMoveDone(fTimeDelta))
			return;
}

_bool CMoveCube::IsMoveDone(const _float & fTimeDelta)
{
	_vec3 dir;
	D3DXVec3Normalize(&dir, &_vec3(m_MovetoPos - m_pTransform->m_vInfo[INFO_POS]));
	m_pTransform->m_vInfo[INFO_POS] += dir*m_fSpeed*fTimeDelta;

	//거리 이용 도달했는지 알려주는 코드
	if (D3DXVec3Length(&_vec3(m_pTransform->m_vInfo[INFO_POS] - m_MovetoPos)) < 0.15f)
	{
		if (m_bIsStone&&!m_bIsFall)
		{
			m_bIsFall = true;
			m_MovetoPos = m_MovetoPos + _vec3(0, 0, 1);
			m_pCollider->Set_BoundingBox(_vec3(0, 0, 0));
			return false;
		}
		m_pTransform->m_vInfo[INFO_POS] = m_MovetoPos;
		m_bIsMoving = false;

		return false;
	}
	return true;
}

void CMoveCube::ShootRay()
{
	CheckColAble(_vec3(1, 0, 0), 2.5f, DIR_LEFT);
	CheckColAble(_vec3(-1, 0, 0), 2.5f, DIR_RIGHT);
	CheckColAble(_vec3(0, 1, 0), 2.5f, DIR_DOWN);
	CheckColAble(_vec3(0, -1, 0), 2.5f, DIR_UP);
}

void CMoveCube::CheckColAble(_vec3 vdir, float len, COL_DIR edir)
{
	_vec3 centerpos = m_pTransform->m_vInfo[INFO_POS];
	vector<_tchar*> tagName;
	tagName.push_back(L"MapCube");
	tagName.push_back(L"MoveCube");
	tagName.push_back(L"GravityCube");
	tagName.push_back(L"InstallCube");
	tagName.push_back(L"SwitchCube");
	tagName.push_back(L"CrackCube");
	tagName.push_back(L"PortalCube");
	tagName.push_back(L"FoodCube");
	tagName.push_back(L"KeyCube");

	vector<RayCollision> _detectedCOL = Engine::Check_Collision_Ray(RAYCAST(centerpos, vdir, len), m_pCollider, tagName);
	if (_detectedCOL.size() >= 1)
	{
		if (!lstrcmp(_detectedCOL[0].tag, L"MapCube")||
			!lstrcmp(_detectedCOL[0].tag, L"InstallCube")||
			!lstrcmp(_detectedCOL[0].tag, L"SwitchCube")||
			!lstrcmp(_detectedCOL[0].tag, L"CrackCube") ||
			!lstrcmp(_detectedCOL[0].tag, L"KeyCube")
			)
			m_bIsCol[edir] = true;
		else
			m_bIsCol[edir] = false;

		if (!lstrcmp(_detectedCOL[0].tag, L"MoveCube") ||
			!lstrcmp(_detectedCOL[0].tag, L"GravityCube")||
			!lstrcmp(_detectedCOL[0].tag, L"FoodCube"))
			m_bIsCol[edir] = dynamic_cast<CMoveCube*>(_detectedCOL[0].col->m_pGameObject)->m_bIsCol[edir];

		//포탈 큐브고									//검출 레이의 방향이 포탈큐브 입구와 다르다면?
		if (!lstrcmp(_detectedCOL[0].tag, L"PortalCube")&&(edir != static_cast<CPortalCube*>(_detectedCOL[0].col->m_pGameObject)->Get_CubeDir()))
			m_bIsCol[edir] = dynamic_cast<CMoveCube*>(_detectedCOL[0].col->m_pGameObject)->m_bIsCol[edir]; //동기화
	}
	else
		m_bIsCol[edir] = false;
}

_bool CMoveCube::DoRayToDir(COL_DIR  dir)
{
	//들어온 방향으로 레이를 쏩니다.
	_vec3 centerpos = m_pTransform->m_vInfo[INFO_POS];
	vector<_tchar*> tagName;
	tagName.push_back(L"MoveCube");
	tagName.push_back(L"GravityCube");
	tagName.push_back(L"PortalCube");
	tagName.push_back(L"FoodCube");
	vector<RayCollision> _detectedCOL;
	switch (dir)
	{
	case DIR_UP:
		_detectedCOL = Engine::Check_Collision_Ray(RAYCAST(centerpos, _vec3(0, -1, 0), 2.5f), m_pCollider, tagName);
		break;
	case DIR_DOWN:
		_detectedCOL = Engine::Check_Collision_Ray(RAYCAST(centerpos, _vec3(0, 1, 0), 2.5f), m_pCollider, tagName);
		break;
	case DIR_LEFT:
		_detectedCOL = Engine::Check_Collision_Ray(RAYCAST(centerpos, _vec3(1.f, 0, 0), 2.5f), m_pCollider, tagName);
		break;
	case DIR_RIGHT:
		_detectedCOL = Engine::Check_Collision_Ray(RAYCAST(centerpos, _vec3(-1.f, 0, 0), 2.5f), m_pCollider, tagName);
		break;
	}
	//거기에 movecube 검출되면 그 친구에게 드로우 레이를 쏩니다.
	if (_detectedCOL.size() == 1)
	{
		if (!lstrcmp(_detectedCOL[0].tag, L"MoveCube")||
			!lstrcmp(_detectedCOL[0].tag, L"GravityCube")||
			!lstrcmp(_detectedCOL[0].tag, L"FoodCube"))
		{
			m_bIsCol[dir] = dynamic_cast<CMoveCube*>(_detectedCOL[0].col->m_pGameObject)->m_bIsCol[dir];
			if (dynamic_cast<CMoveCube*>(_detectedCOL[0].col->m_pGameObject)->DoRayToDir(dir)&&!m_bIsStone)
			{
				SetMovePos(dir);
				return true;
			}
			//거짓이라면 암것도 안합니다.
			return false;
		}
		else
		{
			if (!lstrcmp(_detectedCOL[0].tag, L"PortalCube"))
			{
				//들어오는 방향이 입구와 같은지? && 벽이랑 닿아있는지?
				if (dynamic_cast<CPortalCube*>(_detectedCOL[0].col->m_pGameObject)->Get_CubeDir() == dir&&
					dynamic_cast<CMoveCube*>(_detectedCOL[0].col->m_pGameObject)->m_bIsCol[dir])
				{
					//포탈타게해줘.
				}
				else //아니다
				{
					m_bIsCol[dir] = dynamic_cast<CMoveCube*>(_detectedCOL[0].col->m_pGameObject)->m_bIsCol[dir];
					if (dynamic_cast<CMoveCube*>(_detectedCOL[0].col->m_pGameObject)->DoRayToDir(dir) && !m_bIsStone)
					{
						SetMovePos(dir);
						return true;
					}
					//거짓이라면 암것도 안합니다.
					return false;
				}
			}
		}
			
	}
	if(!m_bIsStone)
		SetMovePos(dir);	
	return true;
}

void CMoveCube::SetMovePos_zero()
{
	m_bIsMoving = true;
	m_MovetoPos = m_pTransform->m_vInfo[INFO_POS];
}

void CMoveCube::SetMovePos(COL_DIR dir)
{
	switch (dir)
	{
	case DIR_UP:
		StopSound(SOUND_EFFECT_GIMMICK);
		PlaySound_Effect(L"11.wav", SOUND_EFFECT_GIMMICK, 1.f);
		m_MoveVec = _vec3(0, -2, 0);
		break;
	case DIR_DOWN:
		StopSound(SOUND_EFFECT_GIMMICK);
		PlaySound_Effect(L"11.wav", SOUND_EFFECT_GIMMICK, 1.f);
		m_MoveVec = _vec3(0, 2, 0);
		break;
	case DIR_LEFT:
		StopSound(SOUND_EFFECT_GIMMICK);
		PlaySound_Effect(L"11.wav", SOUND_EFFECT_GIMMICK, 1.f);
		m_MoveVec = _vec3(2, 0, 0);
		break;
	case DIR_RIGHT:
		StopSound(SOUND_EFFECT_GIMMICK);
		PlaySound_Effect(L"11.wav", SOUND_EFFECT_GIMMICK, 1.f);
		m_MoveVec = _vec3(-2, 0, 0);
		break;
	}
	m_bIsMoving = true;
	m_MovetoPos = _vec3(m_pTransform->m_vInfo[INFO_POS].x + m_MoveVec.x, m_pTransform->m_vInfo[INFO_POS].y + m_MoveVec.y, m_pTransform->m_vInfo[INFO_POS].z);

}

void CMoveCube::MoveToPos(const _float& fTimeDelta)
{
	switch (m_handleState)
	{
	case Engine::CH_NONE:
		//암것도 없음
		break;
	case Engine::CH_START:
		//머리위로
	{	
    Engine::PlaySound_Effect(L"65.wav", SOUND_EFFECT_GIMMICK, 1.f);
	m_iCount = 0;
		if(dynamic_cast<CTopdee*>(m_Target))
			dynamic_cast<CTopdee*>(m_Target)->TopdeeStateChange(TD_SOMETHING);
		if (dynamic_cast<CTookee*>(m_Target))
			dynamic_cast<CTookee*>(m_Target)->TopdeeStateChange(TD_SOMETHING);
		_vec3 vec = m_TargetPos - m_pTransform->m_vInfo[INFO_POS];
		if (D3DXVec3Length(&vec)<0.3f)
		{
			m_pTransform->m_vInfo[INFO_POS] = m_TargetPos;
			if (dynamic_cast<CTopdee*>(m_Target))
			dynamic_cast<CTopdee*>(m_Target)->TopdeeStateChange(TD_MOVE);
			if (dynamic_cast<CTookee*>(m_Target))
				dynamic_cast<CTookee*>(m_Target)->TopdeeStateChange(TD_MOVE);
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
	{	if (m_iCount==0)
		{
		StopSound(SOUND_EFFECT_GIMMICK);
		PlaySound_Effect(L"10.wav", SOUND_EFFECT_GIMMICK, 1.f);
		++m_iCount;
		}
		if (dynamic_cast<CTopdee*>(m_Target))
		dynamic_cast<CTopdee*>(m_Target)->TopdeeStateChange(TD_SOMETHING);
		if (dynamic_cast<CTookee*>(m_Target))
			dynamic_cast<CTookee*>(m_Target)->TopdeeStateChange(TD_SOMETHING);
		_vec3 vec = m_TargetPos - m_pTransform->m_vInfo[INFO_POS];
		if (D3DXVec3Length(&vec)<0.3f)
		{
			
			m_pTransform->m_vInfo[INFO_POS] = m_TargetPos;
			if (dynamic_cast<CTopdee*>(m_Target))
			dynamic_cast<CTopdee*>(m_Target)->TopdeeStateChange(TD_MOVE);
			if (dynamic_cast<CTookee*>(m_Target))
				dynamic_cast<CTookee*>(m_Target)->TopdeeStateChange(TD_MOVE);
			m_handleState = CH_NONE;
			return;
		}
		D3DXVec3Normalize(&vec, &vec);
		m_pTransform->m_vInfo[INFO_POS] += vec*m_fSpeed*fTimeDelta;
		break;
		}
	}
}

void CMoveCube::SetTarget(_vec3 pos, CGameObject * obj)
{
	m_TargetPos = pos;
	m_Target = obj;
	m_handleState = (CUBE_HANDING)((int)(m_handleState)+1);
}

void CMoveCube::DoFallingStart(_vec3 pos)
{
	m_bIsStone = true;
	m_MovetoPos = pos;
	m_bIsMoving = true; 
}

CMoveCube * CMoveCube::Create(LPDIRECT3DDEVICE9 pGraphicDev, _vec3& vPos)
{
	CMoveCube*		pInstance = new CMoveCube(pGraphicDev);

	if (FAILED(pInstance->Ready_GameObject(vPos)))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CMoveCube::Free(void)
{
	__super::Free();
}
