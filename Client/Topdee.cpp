#include "stdafx.h"
#include "Topdee.h"
#include "MoveCube.h"
#include "Export_Function.h"
#include "PortalCube.h"
#include "AbstractFactory.h"

CTopdee::CTopdee(LPDIRECT3DDEVICE9 pGraphicDev)
	: CGameObject(pGraphicDev)
{
	m_bInit = true;
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
	m_bRender = true;
	m_LookVec = _vec3(0, 0, 0);

	__super::Update_GameObject(Engine::Get_Timer(L"Timer_FPS60"));
	return S_OK;
}
_int CTopdee::Update_GameObject(const _float& fTimeDelta)
{
	if (m_bInit)
	{
		//실행해주는 코드
		CLayer* pStageLayer = dynamic_cast<CLayer*>(Engine::Get_Layer(L"Layer_GameLogic"));
		NULL_CHECK_RETURN(pStageLayer, E_FAIL);

		//머리
		FAILED_CHECK_RETURN(FACTORY<CTopdeeParts>::Create(L"TopdeeHead", pStageLayer, _vec3(-1.f, 0.f, -0.2f), m_pTransform, L"Topdee_Head", 0, false), E_FAIL);
		//몸통
		FAILED_CHECK_RETURN(FACTORY<CTopdeeParts>::Create(L"TopdeeBody", pStageLayer, _vec3(-1.f, 0.f, -0.2f), m_pTransform, L"Topdee_Body", 0, false), E_FAIL);
		
		//조인트는 몸통꺼
		FAILED_CHECK_RETURN(FACTORY<CTopdeeJoint>::Create(L"TopdeeBody", pStageLayer, _vec3(-1.f, 0.f, -0.2f), m_pTransform->GetChild(1)), E_FAIL);
		//조인트
		FAILED_CHECK_RETURN(FACTORY<CTopdeeJoint>::Create(L"TopdeeBody", pStageLayer, _vec3(-1.f, 0.f, -0.2f), m_pTransform->GetChild(1)), E_FAIL);
		//조인트
		FAILED_CHECK_RETURN(FACTORY<CTopdeeJoint>::Create(L"TopdeeBody", pStageLayer, _vec3(-1.f, 0.f, -0.2f), m_pTransform->GetChild(1)), E_FAIL);
		//조인트
		FAILED_CHECK_RETURN(FACTORY<CTopdeeJoint>::Create(L"TopdeeBody", pStageLayer, _vec3(-1.f, 0.f, -0.2f), m_pTransform->GetChild(1)), E_FAIL);

		//팔은 조인트 꺼
		FAILED_CHECK_RETURN(FACTORY<CTopdeeParts>::Create(L"TopdeeArm", pStageLayer, _vec3(-1.f, 0.f, -0.2f), m_pTransform->GetChild(1)->GetChild(0), L"Topdee_Arm", 0, false), E_FAIL);
		//팔
		FAILED_CHECK_RETURN(FACTORY<CTopdeeParts>::Create(L"TopdeeArm", pStageLayer, _vec3(-1.f, 0.f, -0.2f), m_pTransform->GetChild(1)->GetChild(1), L"Topdee_Arm", 0, false), E_FAIL);
		//다리
		FAILED_CHECK_RETURN(FACTORY<CTopdeeParts>::Create(L"TopdeeLeg", pStageLayer, _vec3(-1.f, 0.f, -0.2f), m_pTransform->GetChild(1)->GetChild(2), L"Topdee_Arm", 0, false), E_FAIL);
		//다리
		FAILED_CHECK_RETURN(FACTORY<CTopdeeParts>::Create(L"TopdeeLeg", pStageLayer, _vec3(-1.f, 0.f, -0.2f), m_pTransform->GetChild(1)->GetChild(3), L"Topdee_Arm", 0, false), E_FAIL);

		m_partVec.push_back(dynamic_cast<CTopdeeParts*>(m_pTransform->GetChild(0)->m_pGameObject));
		m_partVec.push_back(dynamic_cast<CTopdeeParts*>(m_pTransform->GetChild(1)->m_pGameObject));
		for (int i = 0; i < m_pTransform->GetChild(1)->GetChildCount(); i++)
			if (dynamic_cast<CTopdeeParts*>(m_pTransform->GetChild(1)->GetChild(i)->GetChild(0)->m_pGameObject))
				 m_partVec.push_back(dynamic_cast<CTopdeeParts*>(m_pTransform->GetChild(1)->GetChild(i)->GetChild(0)->m_pGameObject));

		//머리
		m_partVec[0]->m_pTransform->m_vScale = _vec3(2.0f, 2.f, 2.f);
		m_partVec[0]->m_pTransform->m_vInfo[INFO_POS] = _vec3(0,0,-1);

		//몸통
		m_partVec[1]->m_pTransform->m_vScale = _vec3(1,1.5f,1);
		m_partVec[1]->m_pTransform->m_vInfo[INFO_POS] = _vec3(0,-0.6f,-0.8f);

		//조이느
		m_partVec[2]->m_pTransform->GetParent()->m_vScale = _vec3(1,1,1);
		m_partVec[2]->m_pTransform->GetParent()->m_vInfo[INFO_POS] = _vec3(-0.8f, -0.1f, -0.4f);
		m_partVec[3]->m_pTransform->GetParent()->m_vScale = _vec3(1, 1, 1);
		m_partVec[3]->m_pTransform->GetParent()->m_vInfo[INFO_POS] = _vec3(0.8f, -0.1f, -0.4f);
		m_partVec[4]->m_pTransform->GetParent()->m_vScale = _vec3(1, 1, 1);
		m_partVec[4]->m_pTransform->GetParent()->m_vInfo[INFO_POS] = _vec3(-0.5f, 0.1f, 0.2f);
		m_partVec[5]->m_pTransform->GetParent()->m_vScale = _vec3(1, 1, 1);
		m_partVec[5]->m_pTransform->GetParent()->m_vInfo[INFO_POS] = _vec3(0.5f, 0.1f, 0.2f);

		//팔다리
		m_partVec[2]->m_pTransform->m_vScale = _vec3(0.3f,0.8f,1);
		m_partVec[2]->m_pTransform->m_vInfo[INFO_POS] = _vec3(0,0,0.3f);
		m_partVec[2]->m_pTransform->m_vAngle = _vec3(D3DXToRadian(90), D3DXToRadian(90), 0);
		m_partVec[3]->m_pTransform->m_vScale = _vec3(0.3f, 0.8f, 1);
		m_partVec[3]->m_pTransform->m_vInfo[INFO_POS] = _vec3(0, 0, 0.3f);
		m_partVec[3]->m_pTransform->m_vAngle = _vec3(D3DXToRadian(-90), D3DXToRadian(90), 0);
		m_partVec[4]->m_pTransform->m_vScale = _vec3(0.3f, 0.8f, 1);
		m_partVec[4]->m_pTransform->m_vInfo[INFO_POS] = _vec3(0, 0, 0.6f);
		m_partVec[4]->m_pTransform->m_vAngle = _vec3(D3DXToRadian(90), D3DXToRadian(90), 0);
		m_partVec[5]->m_pTransform->m_vScale = _vec3(0.3f, 0.8f, 1);
		m_partVec[5]->m_pTransform->m_vInfo[INFO_POS] = _vec3(0, 0, 0.6f);
		m_partVec[5]->m_pTransform->m_vAngle = _vec3(D3DXToRadian(-90), D3DXToRadian(90), 0);
		m_bInit = false;

		AnimClip* clip = nullptr;
		clip = new AnimClip();
		{
			//Face
			clip->parts.push_back(m_partVec[4]->m_pTransform->GetParent());
			clip->parts.push_back(m_partVec[5]->m_pTransform->GetParent());
			clip->source.resize(2);

			{
				clip->source[0].push_back(
					ANIMINFO{
					_vec3(0,0,0),
					_vec3(D3DXToRadian(0.1f),0,0),//rotation
					_vec3(0,0,0),//scale
					0.2f,//tilltime
					0.f//actionTime
				});

				clip->source[0].push_back(
					ANIMINFO{
					_vec3(0,0,0),
					_vec3(D3DXToRadian(0.1f),0,0),//rotation
					_vec3(0,0,0),
					0.2f,//tilltime
					0.5f//actionTime
				});
			}
			{
				clip->source[1].push_back(
					ANIMINFO{
					_vec3(0,0,0),
					_vec3(D3DXToRadian(0.1f),0,0),//rotation
					_vec3(0,0,0),
					0.2f,//tilltime
					0.f//actionTime
				});
				clip->source[1].push_back(
					ANIMINFO{
					_vec3(0,0,0),
					_vec3(D3DXToRadian(0.1f),0,0),//rotation
					_vec3(0,0,0),
					0.2f,//tilltime
					0.5f//actionTime
				});
			}
			clip->TotalTime = 0.4f;
			clip->Useloop = true;
		}
		m_pAnimation_Leg->AddClip(L"Idle", clip);
		clip = new AnimClip();
		{
			//Face
			clip->parts.push_back(m_partVec[4]->m_pTransform->GetParent());
			clip->parts.push_back(m_partVec[5]->m_pTransform->GetParent());
			clip->source.resize(2);

			{
				clip->source[0].push_back(
					ANIMINFO{
					_vec3(0,0,0),
					_vec3(D3DXToRadian(60),0,0),//rotation
					_vec3(0,0,0),//scale
					0.3f,//tilltime
					0.f//actionTime
				});

				clip->source[0].push_back(
					ANIMINFO{
					_vec3(0,0,0),
					_vec3(D3DXToRadian(-60),0,0),//rotation
					_vec3(0,0,0),
					0.3f,//tilltime
					0.3f//actionTime
				});
			}
			{
				clip->source[1].push_back(
					ANIMINFO{
					_vec3(0,0,0),
					_vec3(D3DXToRadian(-60),0,0),//rotation
					_vec3(0,0,0),
					0.3f,//tilltime
					0.f//actionTime
				});
				clip->source[1].push_back(
					ANIMINFO{
					_vec3(0,0,0),
					_vec3(D3DXToRadian (60),0,0),//rotation
					_vec3(0,0,0),
					0.3f,//tilltime
					0.3f//actionTime
				});
			}
			clip->TotalTime = 0.6f;
			clip->Useloop = true;
		}
		m_pAnimation_Leg->AddClip(L"Walk", clip);
		m_pAnimation_Leg->SetAnimation(L"Idle");
		clip = new AnimClip();
		{
			//Face
			clip->parts.push_back(m_partVec[2]->m_pTransform->GetParent());
			clip->parts.push_back(m_partVec[3]->m_pTransform->GetParent());
			clip->source.resize(2);

			{
				clip->source[0].push_back(
					ANIMINFO{
					_vec3(0,0,0),
					_vec3(D3DXToRadian(0.1f),0,0),//rotation
					_vec3(0,0,0),//scale
					0.3f,//tilltime
					0.f//actionTime
				});

				clip->source[0].push_back(
					ANIMINFO{
					_vec3(0,0,0),
					_vec3(D3DXToRadian(0.1f),0,0),//rotation
					_vec3(0,0,0),
					0.3f,//tilltime
					0.3f//actionTime
				});
			}
			{
				clip->source[1].push_back(
					ANIMINFO{
					_vec3(0,0,0),
					_vec3(D3DXToRadian(0.1f),0,0),//rotation
					_vec3(0,0,0),
					0.3f,//tilltime
					0.f//actionTime
				});
				clip->source[1].push_back(
					ANIMINFO{
					_vec3(0,0,0),
					_vec3(D3DXToRadian(0.1f),0,0),//rotation
					_vec3(0,0,0),
					0.3f,//tilltime
					0.3f//actionTime
				});
			}
			clip->TotalTime = 0.6f;
			clip->Useloop = true;
		}
		m_pAnimation_Arm->AddClip(L"Idle", clip);
		clip = new AnimClip();
		{
			//Face
			clip->parts.push_back(m_partVec[2]->m_pTransform->GetParent());
			clip->parts.push_back(m_partVec[3]->m_pTransform->GetParent());
			clip->source.resize(2);

			{
				clip->source[0].push_back(
					ANIMINFO{
					_vec3(0,0,0),
					_vec3(D3DXToRadian(60),0,0),//rotation
					_vec3(0,0,0),//scale
					0.3f,//tilltime
					0.f//actionTime
				});

				clip->source[0].push_back(
					ANIMINFO{
					_vec3(0,0,0),
					_vec3(D3DXToRadian(-60),0,0),//rotation
					_vec3(0,0,0),
					0.3f,//tilltime
					0.3f//actionTime
				});
			}
			{
				clip->source[1].push_back(
					ANIMINFO{
					_vec3(0,0,0),
					_vec3(D3DXToRadian(-60),0,0),//rotation
					_vec3(0,0,0),
					0.3f,//tilltime
					0.f//actionTime
				});
				clip->source[1].push_back(
					ANIMINFO{
					_vec3(0,0,0),
					_vec3(D3DXToRadian(60),0,0),//rotation
					_vec3(0,0,0),
					0.3f,//tilltime
					0.3f//actionTime
				});
			}
			clip->TotalTime = 0.6f;
			clip->Useloop = true;
		}
		m_pAnimation_Arm->AddClip(L"Walk", clip);
		clip = new AnimClip();
		{
			//Face
			clip->parts.push_back(m_partVec[2]->m_pTransform->GetParent());
			clip->parts.push_back(m_partVec[3]->m_pTransform->GetParent());
			clip->source.resize(2);

			{
				clip->source[0].push_back(
					ANIMINFO{
					_vec3(0,0,0),
					_vec3(D3DXToRadian(210),0,0),//rotation
					_vec3(0,0,0),//scale
					0.2f,//tilltime
					0.f//actionTime
				});

				clip->source[0].push_back(
					ANIMINFO{
					_vec3(0,0,0),
					_vec3(D3DXToRadian(210),0,0),//rotation
					_vec3(0,0,0),
					0.2f,//tilltime
					0.2f//actionTime
				});
			}
			{
				clip->source[1].push_back(
					ANIMINFO{
					_vec3(0,0,0),
					_vec3(D3DXToRadian(210),0,0),//rotation
					_vec3(0,0,0),
					0.2f,//tilltime
					0.f//actionTime
				});
				clip->source[1].push_back(
					ANIMINFO{
					_vec3(0,0,0),
					_vec3(D3DXToRadian(210),0,0),//rotation
					_vec3(0,0,0),
					0.2f,//tilltime
					0.2f//actionTime
				});
			}
			clip->TotalTime = 0.4f;
			clip->Useloop = false;
		}
		m_pAnimation_Arm->AddClip(L"Hang", clip);
		m_pAnimation_Arm->SetAnimation(L"Idle");
	}

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

	_vec3 moveto;
	D3DXVec3Normalize(&moveto, &m_LookVec);
	_float dir = D3DXVec3Dot(&_vec3(0,-1,0), &moveto);
	dir = acosf(dir);
	if ( 0> moveto.x)
		dir = 2 * D3DX_PI - dir;
	
	m_partVec[1]->m_pTransform->m_vAngle.z = lerp_angle(m_partVec[1]->m_pTransform->m_vAngle.z, dir,0.1f);

	m_partVec[2]->m_pTransform->m_vAngle.y = lerp_angle(m_partVec[2]->m_pTransform->m_vAngle.y, dir, 0.1f);
	m_partVec[3]->m_pTransform->m_vAngle.y = lerp_angle(m_partVec[2]->m_pTransform->m_vAngle.y, dir, 0.1f);
	m_partVec[4]->m_pTransform->m_vAngle.y = lerp_angle(m_partVec[2]->m_pTransform->m_vAngle.y, dir, 0.1f);
	m_partVec[5]->m_pTransform->m_vAngle.y = lerp_angle(m_partVec[2]->m_pTransform->m_vAngle.y, dir, 0.1f);

	_int texvalue = (_int)(D3DXToDegree(m_partVec[1]->m_pTransform->m_vAngle.z) / 10) % 36;
	m_partVec[0]->SetTextureIdx((texvalue<0)?36- abs(texvalue): texvalue);

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
	if (m_bRender)
	{
		m_pSlerpParticle->Update_Particle();

		//m_pShadow->Render_Shadow(m_pBufferCom);

		__super::Render_GameObject();
	}
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

	pComponent = m_pShadow = dynamic_cast<CShadow*>(Engine::Clone_Proto(L"Shadow", this));
	NULL_CHECK_RETURN(m_pShadow, E_FAIL);
	m_vecComponent[ID_STATIC].push_back({ L"Shadow", pComponent });

	pComponent = m_pAnimation_Arm = dynamic_cast<CAnimation*>(Engine::Clone_Proto(L"Animation", this));
	NULL_CHECK_RETURN(m_pAnimation_Arm, E_FAIL);
	m_vecComponent[ID_DYNAMIC].push_back({ L"Animation", pComponent });

	pComponent = m_pAnimation_Leg = dynamic_cast<CAnimation*>(Engine::Clone_Proto(L"Animation", this));
	NULL_CHECK_RETURN(m_pAnimation_Leg, E_FAIL);
	m_vecComponent[ID_DYNAMIC].push_back({ L"Animation", pComponent });
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
	{
		PlaySound_Effect(L"78.wav", SOUND_EFFECT, 1.f);
		m_bIsMoving = true;
	}

	if (Engine::Get_DIKeyState(DIK_LEFT) == Engine::KEYDOWN)
		m_byPlayerInputDir |= 8;

	if (Engine::Get_DIKeyState(DIK_RIGHT) == Engine::KEYDOWN)
		m_byPlayerInputDir |= 4;

	if (Engine::Get_DIKeyState(DIK_UP) == Engine::KEYDOWN)
		m_byPlayerInputDir |= 2;

	if (Engine::Get_DIKeyState(DIK_DOWN) == Engine::KEYDOWN)
		m_byPlayerInputDir |= 1;

	if (Engine::Get_DIKeyState(DIK_LEFT) == Engine::KEYUP)
	{
		StopSound(SOUND_EFFECT);
		m_byPlayerInputDir &= 7;
	}
	if (Engine::Get_DIKeyState(DIK_RIGHT) == Engine::KEYUP)
	{
		StopSound(SOUND_EFFECT);
		m_byPlayerInputDir &= 11;
	}
	if (Engine::Get_DIKeyState(DIK_UP) == Engine::KEYUP)
	{
		StopSound(SOUND_EFFECT);
		m_byPlayerInputDir &= 13;
	}
	if (Engine::Get_DIKeyState(DIK_DOWN) == Engine::KEYUP)
	{
		StopSound(SOUND_EFFECT);
		m_byPlayerInputDir &= 14;
	}
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
			if(x!=0||y!=0)
			m_LookVec = _vec3(x,y,0);
			if (x == 0 && y == 0)
			{
				m_pAnimation_Leg->SetAnimation(L"Idle");
				if (m_pGrabObj == nullptr)
					m_pAnimation_Arm->SetAnimation(L"Idle");
			}
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
					m_pAnimation_Arm->SetAnimation(L"Hang");
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
						m_pAnimation_Arm->SetAnimation(L"Hang");
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
					m_pAnimation_Arm->SetAnimation(L"Idle");
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
						m_pAnimation_Arm->SetAnimation(L"Idle");
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
	
	m_pAnimation_Leg->SetAnimation(L"Walk");
	if(m_pGrabObj==nullptr)
		m_pAnimation_Arm->SetAnimation(L"Walk");
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


