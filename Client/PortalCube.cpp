#include "stdafx.h"
#include "PortalCube.h"
#include "Export_Function.h"
#include "Toodee.h"
#include "Topdee.h"
#include "Tookee.h"

static _float m_fSound=1.f;
CPortalCube::CPortalCube(LPDIRECT3DDEVICE9 pGraphicDev) :CMoveCube(pGraphicDev)
{
}

CPortalCube::~CPortalCube()
{
}

HRESULT CPortalCube::Ready_GameObject(_vec3 & vPos)
{
	__super::Ready_GameObject(vPos);
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);
	m_pTransform->m_bIsStatic = true;
	m_pCollider->Set_Options();
	_matrix matInfo;
	D3DXMatrixIdentity(&matInfo);
	m_dwCool = 0; 
	switch (m_eDir)
	{
	case Engine::CD_UP:
		break;
	case Engine::CD_LEFT:
		m_pTransform->Rotation(ROT_Z, D3DXToRadian(90));
		break;
	case Engine::CD_DOWN:
		m_pTransform->Rotation(ROT_Z, D3DXToRadian(180));
		break;
	case Engine::CD_RIGHT:
		m_pTransform->Rotation(ROT_Z, D3DXToRadian(270));
		break;
	}
	m_bInit = true;
	return S_OK;
}

_int CPortalCube::Update_Too(const _float & fTimeDelta)
{
	//첫 업데이트때 다른 큐브를 가져오는 로직인거 같음.
	// 맞음
	if (m_bInit)
	{
		m_pOtherCube = Get_GameObject(L"Layer_GameLogic", L"PortalCube");
		if (m_pOtherCube != this)
			static_cast<CPortalCube*>(m_pOtherCube)->Set_Other(this);
		m_bInit = false;
	}
	if (m_pOtherCube == nullptr)
	{
		m_bInit = true;
		return 0;
	}
	m_pThird = Get_GameObject(L"Layer_GameLogic", L"Thirddee");
	if (m_pThird == nullptr)
		return 0;
	
	_vec3 DisP =m_pThird->m_pTransform->m_vInfo[INFO_POS];
	_float DistoP = D3DXVec3Length(&(m_pTransform->m_vInfo[INFO_POS] - DisP));
	if (DistoP> 70.f)
		m_fSound = 0.f;
	else if(DistoP>50.f)
		m_fSound = 0.3f;
	else if (DistoP > 30.f)
		m_fSound = 0.7f;
	else if (DistoP > 10.f)
		m_fSound = 1.f;

	return 0;
}

_int CPortalCube::Update_Top(const _float & fTimeDelta)
{
	__super::Update_Top(fTimeDelta);
	return 0;
}

_int CPortalCube::Update_GameObject(const _float & fTimeDelta)
{
	if (m_bDead)
		return OBJ_DEAD;
	__super::Update_GameObject(fTimeDelta);
	m_dwCool += fTimeDelta;
	Engine::Add_RenderGroup(RENDER_NONALPHA, this);
	return 0;
}

void CPortalCube::LateUpdate_GameObject(void)
{
	__super::LateUpdate_GameObject();
}

void CPortalCube::Render_GameObject(void)
{
	//레이 諛 위치 = 친구의 원점
	_vec3 start = m_pTransform->m_vInfo[INFO_POS];
	//레이 방향 = 친구의 방향 *-1
	_vec3 dir = m_pTransform->m_vInfo[INFO_POS] + GetDirVec()*5.f;
	
	D3DXMATRIX mat,view,proj;
	D3DXMatrixIdentity(&mat);
	m_pGraphicDev->GetTransform(D3DTS_VIEW,&view);
	m_pGraphicDev->GetTransform(D3DTS_PROJECTION, &proj);
	__super::Render_GameObject();
	m_pPortalParticle->Update_Particle();
}

_vec3 CPortalCube::Trans_Velocity(_vec3 & velocity, CPortalCube* other)
{
	_vec3 myUp, vNormal, vRref, outVelocity, outUp;
	_matrix matRot;
	_float fSpeed, fRad;
	myUp = m_DirVec;
	fSpeed = D3DXVec3Length(&velocity);
	D3DXVec3Normalize(&vNormal, &velocity);
	fRad = acosf(D3DXVec3Dot(&(-myUp), &vNormal));
	D3DXMatrixIdentity(&matRot);
	D3DXMatrixRotationZ(&matRot, fRad);
	outUp = other->m_DirVec;

	D3DXVec3TransformNormal(&vRref, &outUp, &matRot);
	outVelocity = vRref * fSpeed;
	return outVelocity;
}

void CPortalCube::ShootRay_Portal()
{
	vector<_tchar*> name;
	name.push_back(L"MoveCube");
	name.push_back(L"GravityCube");
	_vec3 start = m_pTransform->m_vInfo[INFO_POS];
	vector<RayCollision> _detectedCOL = Engine::Check_Collision_Ray(RAYCAST(start, m_DirVec, 4.f),m_pCollider, name);
	if (_detectedCOL.size() >= 1)
	{
		COL_DIR __dir = (COL_DIR)m_eDir;
		if (__dir % 2 == 0)
			__dir = (COL_DIR)(__dir + 1);
		else
			__dir = (COL_DIR)(__dir - 1);
		dynamic_cast<CMoveCube*>(_detectedCOL[0].col->m_pGameObject)->DoRayToDir(__dir);
	}
}

_bool CPortalCube::Check_BoundingBox(CCollider * pSrc, CCollider * pDest)
{
	float	fX = fabs(pDest->Get_BoundCenter().x - pSrc->Get_BoundCenter().x);
	float	fY = fabs(pDest->Get_BoundCenter().y - pSrc->Get_BoundCenter().y);
	float	fZ = fabs(pDest->Get_BoundCenter().z - pSrc->Get_BoundCenter().z);

	float	fRadiusX = (pDest->Get_BoundSize().x + pSrc->Get_BoundSize().x) * 0.5f;
	float	fRadiusY = (pDest->Get_BoundSize().y + pSrc->Get_BoundSize().y) * 0.5f;
	float	fRadiusZ = (pDest->Get_BoundSize().z + pSrc->Get_BoundSize().z) * 0.5f;

	if ((fRadiusX > fX) && (fRadiusY > fY) && (fRadiusZ > fZ))
		return true;

	return false;
}

void CPortalCube::OnCollisionEnter(const Collision * collision)
{
	__super::OnCollisionEnter(collision);
}

void CPortalCube::OnCollisionStay(const Collision * collision)
{
	if (m_dwCool > 0.2f)
	{
		_bool IsIntersect = Check_BoundingBox(m_pCollider, collision->otherCol);
		//그냥 레이를 하나 쏩시다.
		if (g_Is2D)
		{
			if ((_int)collision->_dir == (_int)m_eDir &&  //들어온 방향이 입구고
				lstrcmp(collision->otherObj->m_pTag, L"MapCube") && //애 제외
				lstrcmp(collision->otherObj->m_pTag, L"PortalCube") && // 애 제외
				lstrcmp(collision->otherObj->m_pTag, L"Topdee") &&//애 제외면?
				IsIntersect)
			{
				StopSound(SOUND_EFFECT_GIMMICK);
				PlaySound_Effect(L"80.wav", SOUND_EFFECT_GIMMICK, m_fSound);
				collision->otherObj->m_pTransform->m_vInfo[INFO_POS] = static_cast<CPortalCube*>(m_pOtherCube)->Get_CubeHeadPos(); //들어온 물체의 위치를 다른 큐브의 헤드로 바꿔주고
				if (collision->otherObj->Get_Component(L"Rigidbody", ID_DYNAMIC) != NULL)
				{
					CRigidbody* rigid = dynamic_cast<CRigidbody*>(collision->otherObj->Get_Component(L"Rigidbody", ID_DYNAMIC));
					_vec3 velocity = rigid->m_Velocity;
					if (D3DXVec3Length(&velocity) < 1)
						rigid->AddForce(velocity, 300, IMPULSE, 0.02f);
						
					if (nullptr == m_pOtherCube)
					{
						rigid->m_Velocity = -velocity;
						return;
					}
					rigid->m_Velocity = Trans_Velocity(velocity, static_cast<CPortalCube*>(m_pOtherCube));
				}
				if (!lstrcmp(collision->otherObj->m_pTag, L"Toodee"))
					dynamic_cast<CToodee*>(collision->otherObj)->SetMovePos_zero();
				static_cast<CPortalCube*>(m_pOtherCube)->CoolReset();
				Start_PortalParticle();
				static_cast<CPortalCube*>(m_pOtherCube)->Start_PortalParticle();
				CoolReset();
			}
		}
		else
		{
			if ((_int)collision->_dir == (_int)m_eDir &&//입구방향
				m_bIsCol[m_eDir] &&
				IsIntersect)//들어온 각도로 막혀있음.
			{
				StopSound(SOUND_EFFECT_GIMMICK);
				PlaySound_Effect(L"80.wav", SOUND_EFFECT_GIMMICK, m_fSound);
				collision->otherObj->m_pTransform->m_vInfo[INFO_POS] = static_cast<CPortalCube*>(m_pOtherCube)->Get_CubeHeadPos();
				static_cast<CPortalCube*>(m_pOtherCube)->ShootRay_Portal();
				//들어온 물체의 위치를 다른 큐브의 헤드로 바꿔주고
				COL_DIR destdir;
				if (static_cast<CPortalCube*>(m_pOtherCube)->Get_CubeDir() % 2 == 0)
					destdir = (COL_DIR)(static_cast<CPortalCube*>(m_pOtherCube)->Get_CubeDir() + 1);
				else
					destdir = (COL_DIR)(static_cast<CPortalCube*>(m_pOtherCube)->Get_CubeDir() - 1);

				if (!lstrcmp(collision->otherObj->m_pTag, L"Topdee"))
					dynamic_cast<CTopdee*>(collision->otherObj)->SetMovePos_zero();

				if (!lstrcmp(collision->otherObj->m_pTag, L"Tookee"))
					dynamic_cast<CTookee*>(collision->otherObj)->SetMovePos_zero();

				else if (dynamic_cast<CMoveCube*>(collision->otherObj))
					dynamic_cast<CMoveCube*>(collision->otherObj)->SetMovePos(destdir);
				Start_PortalParticle();
				static_cast<CPortalCube*>(m_pOtherCube)->Start_PortalParticle();
				static_cast<CPortalCube*>(m_pOtherCube)->CoolReset();
				CoolReset();
			}
		}
	}
	if (collision->_dir != m_eDir)
		__super::OnCollisionStay(collision);
}

void CPortalCube::OnCollisionExit(const Collision * collision)
{
	__super::OnCollisionExit(collision);
}

void CPortalCube::Set_CubeDir(CUBE_DIR dir)
{
	m_eDir = dir;
	_vec3 dirvec[CD_END] = { {0,1,0},{0,-1,0},{-1,0,0},{1,0,0} };
	m_DirVec = dirvec[dir];
}

_vec3 CPortalCube::Get_CubeHeadPos()
{
	//방향에 따라 헤드 위치를 반환시켜주는 친구임.
	if (nullptr == m_pTransform)
		return _vec3{ 0, 0, 0 };
	return _vec3(m_pTransform->m_vInfo[INFO_POS] + (m_DirVec*2));
}

void CPortalCube::Start_PortalParticle()
{
	BoundingBox box;
	if (nullptr == m_pTransform)
		return;
	box.Offset(m_pTransform->m_vInfo[INFO_POS] + m_DirVec * 2);
	if (nullptr == m_pPortalParticle)
		return;
	m_pPortalParticle->Set_Color(D3DXCOLOR(0.4f, 0.8f, 0.7f, 0.8f));
	m_pPortalParticle->Set_SizeLifeTime(0.997f);
	m_pPortalParticle->Set_BoundingBox(box);
	m_pPortalParticle->Set_Size(0.6f);
	m_pPortalParticle->Start_Particle();
}

HRESULT CPortalCube::Add_Component(void)
{
	CComponent*		pComponent = nullptr;

	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Portal_Cube", this));
	NULL_CHECK_RETURN(m_pTextureCom, E_FAIL);
	m_vecComponent[ID_STATIC].push_back({ L"Portal_Cube", pComponent });

	pComponent = m_pPortalParticle = dynamic_cast<CJumpParticle*>(Engine::Clone_Proto(L"JumpParticle", this));
	NULL_CHECK_RETURN(m_pPortalParticle, E_FAIL);
	m_vecComponent[ID_STATIC].push_back({ L"PortalParticle", pComponent });

	return S_OK;
}


CPortalCube * CPortalCube::Create(LPDIRECT3DDEVICE9 pGraphicDev, _vec3 & vPos, int eDir)
{
	CPortalCube*		pInstance = new CPortalCube(pGraphicDev);

	pInstance->Set_CubeDir((CUBE_DIR)eDir);
	if (FAILED(pInstance->Ready_GameObject(vPos)))
	{
		Safe_Release(pInstance);
		return nullptr;
	}
	return pInstance;
}

void CPortalCube::Free(void)
{
	__super::Free();
}

