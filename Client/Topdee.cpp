#include "stdafx.h"
#include "Topdee.h"
#include "MoveCube.h"
#include "Export_Function.h"
#include "PortalCube.h"

CTopdee::CTopdee(LPDIRECT3DDEVICE9 pGraphicDev)
	: CGameObject(pGraphicDev)
{
}

CTopdee::~CTopdee()
{
}

HRESULT CTopdee::Ready_GameObject(_vec3& vPos)
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	m_pTransform->m_vScale = { 1.f, 1.f, 1.f };
	m_pTransform->m_vInfo[INFO_POS] = vPos;
	m_MovetoPos = m_pTransform->m_vInfo[INFO_POS];
	m_pCollider->Set_BoundingBox({ 0.999f,1.999f,1.0f });
	m_pCollider->m_bIsTrigger = true;
	
	__super::Update_GameObject(0.01f);
	return S_OK;
}
_int CTopdee::Update_GameObject(const _float& fTimeDelta)
{
	Engine::Add_RenderGroup(RENDER_NONALPHA, this);
	return 0;
}
_int CTopdee::Update_Too(const _float & fTimeDelta)
{
	CComponent* otherTrans = Engine::Get_Component(L"Layer_GameLogic", L"Toodee", L"Transform", ID_DYNAMIC);
	m_pSlerpParticle->Set_Vectors(m_pTransform->m_vInfo[INFO_POS] + _vec3(0.f, 0.f, -1.f),
		dynamic_cast<CTransform*>(otherTrans)->m_vInfo[INFO_POS]);
	return 0;
}
_int CTopdee::Update_Top(const _float & fTimeDelta)
{
	Key_Input(fTimeDelta);
	RayDiskey();
	if (m_bIsMoving)
		Move(fTimeDelta);
	PlayerState(fTimeDelta);

	__super::Update_GameObject(fTimeDelta);
	return 0;
}
void CTopdee::LateUpdate_GameObject(void)
{
	__super::LateUpdate_GameObject();
}

void CTopdee::Render_GameObject(void)
{
	m_pSlerpParticle->Update_Particle();
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransform->Get_WorldMatrixPointer());

	m_pTextureCom->Set_Texture(0);

	m_pBufferCom->Render_Buffer();

	__super::Render_GameObject();
}

void CTopdee::OnCollisionEnter(const Collision * collision)
{
	if (!lstrcmp(collision->otherObj->m_pTag, L"Boss3Left") && collision->_dir==DIR_FRONT || 
		!lstrcmp(collision->otherObj->m_pTag, L"Boss3Right") && collision->_dir == DIR_FRONT||
		!lstrcmp(collision->otherObj->m_pTag, L"Boss3") && collision->_dir == DIR_FRONT)
	{
		//m_pTextureCom->Switch_Anim(L"Die");
		int i = 0;
	}


	__super::OnCollisionEnter(collision);
}

void CTopdee::OnCollisionStay(const Collision * collision)
{
	__super::OnCollisionStay(collision);
}

void CTopdee::SwapTrigger()
{
	if (g_Is2D)
	{
		Set_SlerpParticle();
	}
}

HRESULT CTopdee::Add_Component(void)
{
	CComponent*		pComponent = nullptr;

	m_pTransform->m_bIsStatic = false;

	pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(Engine::Clone_Proto(L"RcTex", this));
	NULL_CHECK_RETURN(m_pBufferCom, E_FAIL);
	m_vecComponent[ID_STATIC].push_back({ L"RcTex", pComponent });

	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Player_Texture", this));
	NULL_CHECK_RETURN(m_pTextureCom, E_FAIL);
	m_vecComponent[ID_STATIC].push_back({ L"Texture", pComponent });

	pComponent = m_pCollider = dynamic_cast<CCollider*>(Engine::Clone_Proto(L"Collider", this));
	NULL_CHECK_RETURN(m_pCollider, E_FAIL);
	m_vecComponent[ID_DYNAMIC].push_back({ L"Collider", pComponent });

	pComponent = m_pSlerpParticle = dynamic_cast<CSlerpParticle*>(Engine::Clone_Proto(L"SlerpParticle", this));
	NULL_CHECK_RETURN(m_pSlerpParticle, E_FAIL);
	m_vecComponent[ID_STATIC].push_back({ L"SlerpParticle", pComponent });
	return S_OK;
}

CTopdee* CTopdee::Create(LPDIRECT3DDEVICE9 pGraphicDev, _vec3& vPos)
{
	CTopdee*		pInstance = new CTopdee(pGraphicDev);

	if (FAILED(pInstance->Ready_GameObject(vPos)))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CTopdee::Free(void)
{
	__super::Free();
}

void CTopdee::Key_Input(const _float & fTimeDelta)
{
	if ((Engine::Get_DIKeyState(DIK_LEFT) == Engine::KEYPRESS ||
		Engine::Get_DIKeyState(DIK_RIGHT) == Engine::KEYPRESS ||
		Engine::Get_DIKeyState(DIK_UP) == Engine::KEYPRESS ||
		Engine::Get_DIKeyState(DIK_DOWN) == Engine::KEYPRESS) &&
		m_eState == TD_MOVE)
		m_bIsMoving = true;

	if (Engine::Get_DIKeyState(DIK_LEFT) == Engine::KEYDOWN)
		m_byPlayerInputDir |= 8;

	if (Engine::Get_DIKeyState(DIK_RIGHT) == Engine::KEYDOWN)
		m_byPlayerInputDir |= 4;

	if (Engine::Get_DIKeyState(DIK_UP) == Engine::KEYDOWN)
		m_byPlayerInputDir |= 2;

	if (Engine::Get_DIKeyState(DIK_DOWN) == Engine::KEYDOWN)
		m_byPlayerInputDir |= 1;

	if (Engine::Get_DIKeyState(DIK_LEFT) == Engine::KEYUP)
		m_byPlayerInputDir &= 7;

	if (Engine::Get_DIKeyState(DIK_RIGHT) == Engine::KEYUP)
		m_byPlayerInputDir &= 11;

	if (Engine::Get_DIKeyState(DIK_UP) == Engine::KEYUP)
		m_byPlayerInputDir &= 13;

	if (Engine::Get_DIKeyState(DIK_DOWN) == Engine::KEYUP)
		m_byPlayerInputDir &= 14;

	if (Engine::Get_DIKeyState(DIK_Z) == Engine::KEYDOWN&&!m_bIsMoving)
		m_eState = TD_FINDING;

	if (Engine::Get_DIKeyState(DIK_Z) == Engine::KEYUP&&m_eState != TD_SOMETHING)
		m_eState = TD_MOVE;

	if (m_byPlayerInputDir != 0)
		m_byLookDir = m_byPlayerInputDir;
}

void CTopdee::RayDiskey()
{
	_int fdir[MD_END] = { 2,1,8,4,6,10,5,9 };
	//플레이어가 이동하려는 방향으로만 검출합니다
	for (int i = 0; i < MD_END; i++)
	{
		if (m_byPlayerInputDir == fdir[i])
			RayDisKey_part((COL_MOVEDIR)i);
	}
}

void CTopdee::RayDisKey_part(COL_MOVEDIR dir)
{
	_vec3 vdir[MD_END] = { { 0,1,0 },{ 0,-1,0 },{ -1,0,0 },{ 1,0,0 },{1,1,0},{-1,1,0},{1,-1,0},{-1,-1,0} };
	_int fdir[MD_END] = {13,14,7,11,9,5,10,6};
	vector<RayCollision> _detectedCOL = Engine::Check_Collision_Ray(RAYCAST(m_pTransform->m_vInfo[INFO_POS], vdir[dir], 1.5f), m_pCollider);
	if (_detectedCOL.size() >= 1)
	{
		if (!lstrcmp(_detectedCOL[0].tag, L"MapCube") ||
			!lstrcmp(_detectedCOL[0].tag, L"InstallGrid") ||
			!lstrcmp(_detectedCOL[0].tag, L"CrackCube")||
			!lstrcmp(_detectedCOL[0].tag, L"InstallCube") ||
			!lstrcmp(_detectedCOL[0].tag, L"SwitchCube")||
			!lstrcmp(_detectedCOL[0].tag, L"Boss3")||
			!lstrcmp(_detectedCOL[0].tag, L"Boss3Left")||
			!lstrcmp(_detectedCOL[0].tag, L"Boss3Right")
			) m_byPlayerInputDir &= fdir[dir];
		if (!lstrcmp(_detectedCOL[0].tag, L"MoveCube")||
			!lstrcmp(_detectedCOL[0].tag, L"GravityCube"))
		{

			if (dir < 4)
			{
				COL_DIR destdir;
				if (dir % 2 == 0)
					destdir = (COL_DIR)(dir + 1);
				else
					destdir = (COL_DIR)(dir - 1);

				if (dynamic_cast<CMoveCube*>(_detectedCOL[0].col->m_pGameObject)->m_bIsCol[destdir])
					m_byPlayerInputDir &= fdir[dir];
			}
		}
		if (!lstrcmp(_detectedCOL[0].tag, L"PortalCube"))
		{
			COL_DIR destdir = (COL_DIR)dynamic_cast<CPortalCube*>(_detectedCOL[0].col->m_pGameObject)->Get_CubeDir();
			if (dir % 2 != 0)
				destdir = (COL_DIR)(destdir + 1);
			else
				destdir = (COL_DIR)(destdir - 1);
			_bool isBlocked = dynamic_cast<CMoveCube*>(_detectedCOL[0].col->m_pGameObject)->m_bIsCol[dir];
			
			//만약 플레이어가 이동하려는 방향이 입구가 아니고
			//그쪽방향으로 막혀있다면?
			if (destdir == dir&& !isBlocked )
			{

			}
			else 
			{
				//이동 막아주는 로직
				COL_DIR destdir;
				if (dir % 2 == 0)
					destdir = (COL_DIR)(dir + 1);
				else
					destdir = (COL_DIR)(dir - 1);

				if (dynamic_cast<CMoveCube*>(_detectedCOL[0].col->m_pGameObject)->m_bIsCol[destdir])
					m_byPlayerInputDir &= fdir[dir];
			}
		
		}
	}
}

void CTopdee::PlayerState(const _float& fTimeDelta)
{
	int x = 0; int y = 0;
	switch (m_eState)
	{
	case Engine::TD_MOVE:
		if (!m_bIsMoving)
		{
			DirApply(m_byPlayerInputDir, x, y);
			m_MovetoPos = _vec3((int)m_pTransform->m_vInfo[INFO_POS].x + x, (int)m_pTransform->m_vInfo[INFO_POS].y + y, (int)m_pTransform->m_vInfo[INFO_POS].z);
			m_bIsMoving = false;
		}
		break;
	case Engine::TD_FINDING:
	{
		DirApply(m_byLookDir,x,y);

		_vec3 maindir;
		D3DXVec3Normalize(&maindir, &_vec3(x, y, 0));
		if (m_pGrabObj == nullptr)
		{
			CCollider* col = nullptr;
			_vec3 dir[8] = { { 1,1,0 },{ 0,1,0 },{ -1,1,0 },{ 1,0,0 },{ -1,0,0 },{ 1,-1,0 },{ 0,-1,0 },{ -1,-1,0 } };

			//보는 방향 1순위
			if (CheckCubeExist(_vec3((_float)x, (_float)y, 0.f), &col))
			{
				if (dynamic_cast<CMoveCube*>(col->m_pGameObject)->GetHandleState())
				{
					m_pGrabObj = col->m_pGameObject;
					dynamic_cast<CMoveCube*>(m_pGrabObj)->SetTarget(_vec3(m_pTransform->m_vInfo[INFO_POS] + _vec3(0, 0, -4)), this);
					return;
				}
			}

			//나머지
			for (int i = 0; i < 8; i++)
			{
				if (maindir == dir[i])
					return;
				if (CheckCubeExist(dir[i], &col))
				{
					if (dynamic_cast<CMoveCube*>(col->m_pGameObject)->GetHandleState())
					{
						m_pGrabObj = col->m_pGameObject;
						dynamic_cast<CMoveCube*>(m_pGrabObj)->SetTarget(_vec3(m_pTransform->m_vInfo[INFO_POS] + _vec3(0, 0, -4)), this);
						return;
					}
				}
			}

		}
		else
		{
			CCollider* col = nullptr;
			_vec3 dir[8] = { { 1,1,0 },{ 0,1,0 },{ -1,1,0 },{ 1,0,0 },{ -1,0,0 },{ 1,-1,0 },{ 0,-1,0 },{ -1,-1,0 } };

			//보는 방향 1순위
			if (!CheckAnythingExist(_vec3((_float)x, (_float)y, 0), &col))
			{
				if (dynamic_cast<CMoveCube*>(m_pGrabObj)->GetHandleState())
				{
					dynamic_cast<CMoveCube*>(m_pGrabObj)->SetTarget(_vec3(m_pTransform->m_vInfo[INFO_POS] + _vec3((_float)x, (_float)y, -1.f)), this);
					m_pGrabObj = nullptr;
					return;
				}
			}

			//나머지
			for (int i = 0; i < 8; i++)
			{
				if (maindir == dir[i])
					return;
				if (!CheckAnythingExist(dir[i], &col))
				{
					if (dynamic_cast<CMoveCube*>(m_pGrabObj)->GetHandleState())
					{
						dynamic_cast<CMoveCube*>(m_pGrabObj)->SetTarget(_vec3(m_pTransform->m_vInfo[INFO_POS] + _vec3((_int)dir[i].x * 2.f, (_int)dir[i].y * 2.f, -1.f)), this);
						m_pGrabObj = nullptr;
						return;
					}
				}
			}
		}
		break;
	}
	}
}

//실제 움직이는 코드
void CTopdee::Move(const _float& fTimeDelta)
{
	_vec3 dir;
	D3DXVec3Normalize(&dir, &_vec3(m_MovetoPos - m_pTransform->m_vInfo[INFO_POS]));
	m_pTransform->m_vInfo[INFO_POS] += dir*m_fSpeed*fTimeDelta;

	//만약 도달했다면?
	if (D3DXVec3Length(&_vec3(m_pTransform->m_vInfo[INFO_POS] - m_MovetoPos)) < 0.3f)
	{
		m_pTransform->m_vInfo[INFO_POS] = m_MovetoPos;
		m_bIsMoving = false;
		return;
	}
}

_bool CTopdee::CheckCubeExist(_vec3 dir, CCollider** col)
{
	vector<RayCollision> _detectedCOL = Engine::Check_Collision_Ray(RAYCAST(m_pTransform->m_vInfo[INFO_POS], dir, 1.5f), m_pCollider);

	if (_detectedCOL.size() >= 1)
	{
		if (!lstrcmp(_detectedCOL[0].tag, L"MoveCube") ||
			!lstrcmp(_detectedCOL[0].tag, L"GravityCube")||
			!lstrcmp(_detectedCOL[0].tag, L"PortalCube"))
		{
			*col = _detectedCOL[0].col;
			return true;
		}
	}
	return false;
}

void CTopdee::SetMovePos_zero()
{
	m_bIsMoving = true;
	m_MovetoPos = m_pTransform->m_vInfo[INFO_POS];
}


void CTopdee::SetMovePos(COL_DIR dir)
{
	_vec3 m_MoveVec;
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

_bool CTopdee::CheckAnythingExist(_vec3 dir, CCollider ** col)
{
	_vec3 centerpos = m_pTransform->m_vInfo[INFO_POS];
	vector<RayCollision> _detectedCOL = Engine::Check_Collision_Ray(RAYCAST(centerpos, dir, 1.5f), m_pCollider);

	if (_detectedCOL.size() >= 1 )
	{
		if (!lstrcmp(_detectedCOL[0].tag, L"InstallGrid"))
			return false;
		return true;
	}
	return false;
}

void CTopdee::DirApply(_int dir, _int & x, _int & y)
{
	if (dir & 1)
		y -= 2;
	if (dir & 2)
		y += 2;
	if (dir & 4)
		x += 2;
	if (dir & 8)
		x -= 2;
}

void CTopdee::Set_SlerpParticle()
{
	m_pSlerpParticle->Reset();
	m_pSlerpParticle->Set_Size(1.5f);
	m_pSlerpParticle->Start_Particle();
}


