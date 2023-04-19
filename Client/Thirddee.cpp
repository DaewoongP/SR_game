#include "stdafx.h"
#include "Thirddee.h"
#include "AbstractFactory.h"
#include "MoveCube.h"
#include "PortalCube.h"
#include "Export_Function.h"

CThirddee::CThirddee(LPDIRECT3DDEVICE9 pGraphicDev)
	:CTookee(pGraphicDev)
{
}

CThirddee::~CThirddee()
{
}

HRESULT CThirddee::Ready_GameObject(_vec3 & vPos)
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);
	m_MovetoPos = m_pTransform->m_vInfo[INFO_POS];
	m_LookVec = _vec3(0, 0, 0);

	m_pTransform->m_vScale = { 0.6f, 0.6f, 0.6f };
	m_pTransform->m_vInfo[INFO_POS] = vPos;
	m_pTransform->m_bIsStatic = false;
	m_pCollider->m_bIsTrigger = false;
	// 애니메이션
	m_bInit2 = true;
	m_bRender = true;
	m_pCollider->Set_BoundingBox({ 1.f,2.f,1.0f });
	m_pCollider->Set_BoundOffset(_vec3(0,0,0));
	BoundingBox box;
	m_moveTrue = true;
	box.Offset(vPos);
	m_pLandingParticle->Set_BoundingBox(box);
	m_pSparkParticle->Set_LifeTime();
	return S_OK;
}

_int CThirddee::Update_GameObject(const _float & fTimeDelta)
{
	if (m_bInit2)
	{
		//실행해주는 코드
		CLayer* pStageLayer = dynamic_cast<CLayer*>(Engine::Get_Layer(L"Layer_GameLogic"));
		NULL_CHECK_RETURN(pStageLayer, E_FAIL);

		//머리
		FAILED_CHECK_RETURN(FACTORY<CTopdeeParts>::Create(L"Third_Head", pStageLayer, _vec3(0,0,0), m_pTransform, L"Third_Head", 0, false), E_FAIL);
		FAILED_CHECK_RETURN(FACTORY<CTopdeeParts>::Create(L"Third_BackHead", pStageLayer, _vec3(0,0, 0), m_pTransform->GetChild(0), L"Third_Head", 1, false), E_FAIL);
		dynamic_cast<CTopdeeParts*>(m_pTransform->GetChild(0)->GetChild(0)->m_pGameObject)->SetTextureIdx(1);
		//몸통
		FAILED_CHECK_RETURN(FACTORY<CTopdeeParts>::Create(L"Third_Body", pStageLayer, _vec3(-1.f, 0.f, -0.2f), m_pTransform, L"Third_Body", 0, false), E_FAIL);

		//조인트는 몸통꺼
		FAILED_CHECK_RETURN(FACTORY<CTopdeeJoint>::Create(L"Third_Joint", pStageLayer, _vec3(-1.f, 0.f, -0.2f), m_pTransform->GetChild(1)), E_FAIL);
		//조인트
		FAILED_CHECK_RETURN(FACTORY<CTopdeeJoint>::Create(L"Third_Joint", pStageLayer, _vec3(-1.f, 0.f, -0.2f), m_pTransform->GetChild(1)), E_FAIL);
		//조인트
		FAILED_CHECK_RETURN(FACTORY<CTopdeeJoint>::Create(L"Third_Joint", pStageLayer, _vec3(-1.f, 0.f, -0.2f), m_pTransform->GetChild(1)), E_FAIL);
		//조인트
		FAILED_CHECK_RETURN(FACTORY<CTopdeeJoint>::Create(L"Third_Joint", pStageLayer, _vec3(-1.f, 0.f, -0.2f), m_pTransform->GetChild(1)), E_FAIL);

		//팔은 조인트 꺼
		FAILED_CHECK_RETURN(FACTORY<CTopdeeParts>::Create(L"Third_Hand", pStageLayer, _vec3(-1.f, 0.f, -0.2f), m_pTransform->GetChild(1)->GetChild(0), L"Third_Hand", 0, false), E_FAIL);
		//팔
		FAILED_CHECK_RETURN(FACTORY<CTopdeeParts>::Create(L"Third_Hand", pStageLayer, _vec3(-1.f, 0.f, -0.2f), m_pTransform->GetChild(1)->GetChild(1), L"Third_Hand", 0, false), E_FAIL);
		//다리
		FAILED_CHECK_RETURN(FACTORY<CTopdeeParts>::Create(L"Third_Foot", pStageLayer, _vec3(-1.f, 0.f, -0.2f), m_pTransform->GetChild(1)->GetChild(2), L"Third_Foot", 0, false), E_FAIL);
		//다리
		FAILED_CHECK_RETURN(FACTORY<CTopdeeParts>::Create(L"Third_Foot", pStageLayer, _vec3(-1.f, 0.f, -0.2f), m_pTransform->GetChild(1)->GetChild(3), L"Third_Foot", 0, false), E_FAIL);

		m_partVec.push_back(dynamic_cast<CTopdeeParts*>(m_pTransform->GetChild(0)->m_pGameObject));
		m_partVec.push_back(dynamic_cast<CTopdeeParts*>(m_pTransform->GetChild(1)->m_pGameObject));
		for (int i = 0; i < m_pTransform->GetChild(1)->GetChildCount(); i++)
			if (dynamic_cast<CTopdeeParts*>(m_pTransform->GetChild(1)->GetChild(i)->GetChild(0)->m_pGameObject))
				m_partVec.push_back(dynamic_cast<CTopdeeParts*>(m_pTransform->GetChild(1)->GetChild(i)->GetChild(0)->m_pGameObject));

		//머리
		m_partVec[0]->m_pTransform->m_vScale = _vec3(1.8f, 1.8f, 1.8f);
		m_partVec[0]->m_pTransform->m_vInfo[INFO_POS] = _vec3(0, 1, -0.01f);

		//통수
		m_partVec[0]->m_pTransform->GetChild(0)->m_vInfo[INFO_POS] = _vec3(0,0,+0.02f);


		//몸통
		m_partVec[1]->m_pTransform->m_vScale = _vec3(2.2f, 2.2f, 2.2f);
		m_partVec[1]->m_pTransform->m_vInfo[INFO_POS] = _vec3(0.0f, -0.0f, 0.f);

		//조이느
		m_partVec[2]->m_pTransform->GetParent()->m_vScale = _vec3(1, 1, 1);
		m_partVec[2]->m_pTransform->GetParent()->m_vInfo[INFO_POS] = _vec3(-0.8f, 0, -0.f);
		m_partVec[3]->m_pTransform->GetParent()->m_vScale = _vec3(-1, 1, 1);
		m_partVec[3]->m_pTransform->GetParent()->m_vInfo[INFO_POS] = _vec3(0.8f, 0, -0.f);
		m_partVec[4]->m_pTransform->GetParent()->m_vScale = _vec3(1, 1, 1);
		m_partVec[4]->m_pTransform->GetParent()->m_vInfo[INFO_POS] = _vec3(-0.5f, -0.8f, 0.f);
		m_partVec[5]->m_pTransform->GetParent()->m_vScale = _vec3(1, 1, 1);
		m_partVec[5]->m_pTransform->GetParent()->m_vInfo[INFO_POS] = _vec3(0.5f, -0.8f, 0.f);

		//팔다리
		m_partVec[2]->m_pTransform->m_vScale = _vec3(2.2f,2.2f,2.2f);
		m_partVec[2]->m_pTransform->m_vInfo[INFO_POS] = _vec3(0, 0, 0.f);
		m_partVec[2]->m_pTransform->m_vAngle = _vec3(D3DXToRadian(00), D3DXToRadian(00), 0);
		m_partVec[3]->m_pTransform->m_vScale = _vec3(2.2f, 2.2f, 2.2f);
		m_partVec[3]->m_pTransform->m_vInfo[INFO_POS] = _vec3(0, 0, 0.f);
		m_partVec[3]->m_pTransform->m_vAngle = _vec3(D3DXToRadian(00), D3DXToRadian(00), 0);
		m_partVec[4]->m_pTransform->m_vScale = _vec3(2.2f, 2.2f, 2.2f);
		m_partVec[4]->m_pTransform->m_vInfo[INFO_POS] = _vec3(0, 0, 0.f);
		m_partVec[4]->m_pTransform->m_vAngle = _vec3(D3DXToRadian(00), D3DXToRadian(00), 0);
		m_partVec[5]->m_pTransform->m_vScale = _vec3(2.2f, 2.2f, 2.2f);
		m_partVec[5]->m_pTransform->m_vInfo[INFO_POS] = _vec3(0, 0, 0.f);
		m_partVec[5]->m_pTransform->m_vAngle = _vec3(D3DXToRadian(00), D3DXToRadian(00), 0);
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
					_vec3(D3DXToRadian(0.01f),0,0),//rotation
					_vec3(0,0,0),//scale
					0.2f,//tilltime
					0.f//actionTime
				});

				clip->source[0].push_back(
					ANIMINFO{
					_vec3(0,0,0),
					_vec3(D3DXToRadian(0.01f),0,0),//rotation
					_vec3(0,0,0),
					0.2f,//tilltime
					0.5f//actionTime
				});
			}
			{
				clip->source[1].push_back(
					ANIMINFO{
					_vec3(0,0,0),
					_vec3(D3DXToRadian(0.01f),0,0),//rotation
					_vec3(0,0,0),
					0.2f,//tilltime
					0.f//actionTime
				});
				clip->source[1].push_back(
					ANIMINFO{
					_vec3(0,0,0),
					_vec3(D3DXToRadian(0.01f),0,0),//rotation
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
					_vec3(D3DXToRadian(60),0,0),//rotation
					_vec3(0,0,0),
					0.3f,//tilltime
					0.3f//actionTime
				});
			}
			clip->TotalTime = 0.6f;
			clip->Useloop = true;
		}
		m_pAnimation_Leg->AddClip(L"Walk", clip);
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
					_vec3(D3DXToRadian(90),0,0),//rotation
					_vec3(0,0,0),//scale
					0.3f,//tilltime
					0.f//actionTime
				});

				clip->source[0].push_back(
					ANIMINFO{
					_vec3(0,0,0),
					_vec3(D3DXToRadian(90),0,0),//rotation
					_vec3(0,0,0),
					0.3f,//tilltime
					0.3f//actionTime
				});
			}
			{
				clip->source[1].push_back(
					ANIMINFO{
					_vec3(0,0,0),
					_vec3(D3DXToRadian(-30),0,0),//rotation
					_vec3(0,0,0),
					0.3f,//tilltime
					0.f//actionTime
				});
				clip->source[1].push_back(
					ANIMINFO{
					_vec3(0,0,0),
					_vec3(D3DXToRadian(-30),0,0),//rotation
					_vec3(0,0,0),
					0.3f,//tilltime
					0.3f//actionTime
				});
			}
			clip->TotalTime = 0.6f;
			clip->Useloop = false;
		}
		m_pAnimation_Leg->AddClip(L"Jump", clip);
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
					_vec3(D3DXToRadian(0.01f),0,D3DXToRadian(30)),//rotation
					_vec3(0,0,0),//scale
					0.3f,//tilltime
					0.f//actionTime
				});

				clip->source[0].push_back(
					ANIMINFO{
					_vec3(0,0,0),
					_vec3(D3DXToRadian(0.01f),0,D3DXToRadian(30)),//rotation
					_vec3(0,0,0),
					0.3f,//tilltime
					0.3f//actionTime
				});
			}
			{
				clip->source[1].push_back(
					ANIMINFO{
					_vec3(0,0,0),
					_vec3(D3DXToRadian(0.01f),0,D3DXToRadian(-30)),//rotation
					_vec3(0,0,0),
					0.3f,//tilltime
					0.f//actionTime
				});
				clip->source[1].push_back(
					ANIMINFO{
					_vec3(0,0,0),
					_vec3(D3DXToRadian(0.01f),0,D3DXToRadian(-30)),//rotation
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
					_vec3(0,D3DXToRadian(60),0),//rotation
					_vec3(0,0,0),//scale
					0.3f,//tilltime
					0.f//actionTime
				});

				clip->source[0].push_back(
					ANIMINFO{
					_vec3(0,0,0),
					_vec3(0,D3DXToRadian(-60),0),//rotation
					_vec3(0,0,0),
					0.3f,//tilltime
					0.3f//actionTime
				});
			}
			{
				clip->source[1].push_back(
					ANIMINFO{
					_vec3(0,0,0),
					_vec3(0,D3DXToRadian(-60),0),//rotation
					_vec3(0,0,0),
					0.3f,//tilltime
					0.f//actionTime
				});
				clip->source[1].push_back(
					ANIMINFO{
					_vec3(0,0,0),
					_vec3(0,D3DXToRadian(60),0),//rotation
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
					_vec3(D3DXToRadian(0.01f),0,D3DXToRadian(-90)),//rotation
					_vec3(0,0,0),//scale
					0.3f,//tilltime
					0.f//actionTime
				});

				clip->source[0].push_back(
					ANIMINFO{
					_vec3(0,0,0),
					_vec3(D3DXToRadian(0.01f),0,D3DXToRadian(-90)),//rotation
					_vec3(0,0,0),
					0.3f,//tilltime
					0.3f//actionTime
				});
			}
			{
				clip->source[1].push_back(
					ANIMINFO{
					_vec3(0,0,0),
					_vec3(D3DXToRadian(0.01f),0,D3DXToRadian(90)),//rotation
					_vec3(0,0,0),
					0.3f,//tilltime
					0.f//actionTime
				});
				clip->source[1].push_back(
					ANIMINFO{
					_vec3(0,0,0),
					_vec3(D3DXToRadian(0.01f),0,D3DXToRadian(90)),//rotation
					_vec3(0,0,0),
					0.3f,//tilltime
					0.3f//actionTime
				});
			}
			clip->TotalTime = 0.6f;
			clip->Useloop = false;
		}
		m_pAnimation_Arm->AddClip(L"Jump", clip);
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
					_vec3(D3DXToRadian(-90),D3DXToRadian(0),D3DXToRadian(0)),//rotation
					_vec3(0,0,0),//scale
					0.3f,//tilltime
					0.f//actionTime
				});

				clip->source[0].push_back(
					ANIMINFO{
					_vec3(0,0,0),
					_vec3(D3DXToRadian(-90),D3DXToRadian(0),D3DXToRadian(180)),//rotation
					_vec3(0,0,0),
					0.3f,//tilltime
					0.3f//actionTime
				});
			}
			{
				clip->source[1].push_back(
					ANIMINFO{
					_vec3(0,0,0),
					_vec3(D3DXToRadian(-90),D3DXToRadian(0),D3DXToRadian(0)),//rotation
					_vec3(0,0,0),
					0.3f,//tilltime
					0.f//actionTime
				});
				clip->source[1].push_back(
					ANIMINFO{
					_vec3(0,0,0),
					_vec3(D3DXToRadian(-90),D3DXToRadian(0),D3DXToRadian(180)),//rotation
					_vec3(0,0,0),
					0.3f,//tilltime
					0.3f//actionTime
				});
			}
			clip->TotalTime = 0.6f;
			clip->Useloop = false;
		}
		m_pAnimation_Arm->AddClip(L"Hang", clip);
		m_pAnimation_Arm->SetAnimation(L"Idle");
		

		clip = new AnimClip();
		{
			//Face
			clip->parts.push_back(m_partVec[0]->m_pTransform);
			clip->source.resize(1);
			{
				clip->source[0].push_back(
					ANIMINFO{
					_vec3(0.01f,1,-0.01f),
					_vec3(D3DXToRadian(0.0f),0,0),//rotation
					_vec3(0,0,0),//scale
					0.2f,//tilltime
					0.f//actionTime
				});

				clip->source[0].push_back(
					ANIMINFO{
					_vec3(0.01f,1,-0.01f),
					_vec3(D3DXToRadian(0.0f),0,0),//rotation
					_vec3(0,0,0),
					0.2f,//tilltime
					0.5f//actionTime
				});
			}
			clip->TotalTime = 0.4f;
			clip->Useloop = true;
		}
		m_pAnimation_Head->AddClip(L"Idle", clip);
		clip = new AnimClip();
		{
			//Face
			clip->parts.push_back(m_partVec[0]->m_pTransform);
			clip->source.resize(1);
			{
				clip->source[0].push_back(
					ANIMINFO{
					_vec3(0.5f,1,-0.01f),
					_vec3(0,0,0),//rotation
					_vec3(0,0,0),//scale
					0.3f,//tilltime
					0.f//actionTime
				});

				clip->source[0].push_back(
					ANIMINFO{
					_vec3(-0.5f,1,-0.01f),
					_vec3(0,0,0),//rotation
					_vec3(0,0,0),
					0.3f,//tilltime
					0.3f//actionTime
				});
			}
			clip->TotalTime = 0.6f;
			clip->Useloop = true;
		}
		m_pAnimation_Head->AddClip(L"Walk", clip);
		m_pAnimation_Head->SetAnimation(L"Idle");

		SetRenderONOFF(true);
		m_bInit2 = false;
	}
	Engine::Add_RenderGroup(RENDER_NONALPHA, this);
	CGameObject::Update_GameObject(fTimeDelta);
	return 0;
}

_int CThirddee::Update_Too(const _float & fTimeDelta)
{
	if (m_bDead)
		return OBJ_DEAD;
	Key_Input(fTimeDelta);
		
	DoFlip();
	return 0;
}

_int CThirddee::Update_Top(const _float & fTimedDelte)
{
	
	_vec3 moveto;
	D3DXVec3Normalize(&moveto, &m_LookVec);

	Key_Input2(fTimedDelte);
	_float dir = D3DXVec3Dot(&_vec3(0, -1, 0), &moveto);

	if(!lstrcmp(m_pAnimation_Leg->GetAnimationName(),L"Idle"))
		m_pAnimation_Head->SetAnimation(L"Idle");
	else
		m_pAnimation_Head->SetAnimation(L"Walk");

	RayDiskey();
	if (m_bIsMoving)
		Move(fTimedDelte);
	PlayerState(fTimedDelte);
	return 0;
}

void CThirddee::LateUpdate_GameObject(void)
{
	__super::LateUpdate_GameObject();
}

void CThirddee::Render_GameObject(void)
{
	CGameObject::Render_GameObject();
}

void CThirddee::Render_Too(void)
{
}

void CThirddee::Render_Top(void)
{
}

void CThirddee::SwapTrigger()
{
	if (g_Is2D)
	{
		m_pRigid->m_bUseGrivaty = true;
		
		m_pTransform->m_vInfo[INFO_POS].z = 10;
		m_pCollider->m_bIsTrigger = false;
		m_pCollider->Set_BoundingBox({ 1.f,2.f,1.0f });
	}
	else
	{
		m_pTransform->m_vInfo[INFO_POS].z = 11;
		m_pRigid->m_bUseGrivaty = false;
		m_pTransform->m_vInfo[INFO_POS].x =
			((int)m_pTransform->m_vInfo[INFO_POS].x % 2 == 0) ? ((int)m_pTransform->m_vInfo[INFO_POS].x) : ((int)m_pTransform->m_vInfo[INFO_POS].x + 1);
		m_pTransform->m_vInfo[INFO_POS].y =
			((int)m_pTransform->m_vInfo[INFO_POS].y % 2 == 0) ? ((int)m_pTransform->m_vInfo[INFO_POS].y) : ((int)m_pTransform->m_vInfo[INFO_POS].y + 1);
		m_pCollider->Set_BoundingBox({ 0.999f,1.999f,1.0f });
		m_pCollider->m_bIsTrigger = true;
	}
	m_bIsMoving = false;
	m_MovetoPos = m_pTransform->m_vInfo[INFO_POS];
	m_byPlayerInputDir = 0;
	m_pRigid->m_Velocity = _vec3(0, 0, 0);
	m_pTransform->m_vAngle = _vec3(0, 0, 0);
	m_pTransform->m_vScale = _vec3(1, 1, 1);
	SetRenderONOFF(true);
}

void CThirddee::OnCollisionEnter(const Collision * collision)
{
	//스파이크랑 충돌시 본인 데드 트루
	if (!lstrcmp(collision->otherObj->m_pTag, L"Spike") &&
		collision->_dir == DIR_DOWN)
	if (!lstrcmp(collision->otherObj->m_pTag, L"PinkCloud") &&
		collision->_dir == DIR_DOWN)
	{
		StopSound(SOUND_EFFECT);
		PlaySound_Effect(L"58.wav", SOUND_EFFECT, 1.f);
	}
	if (collision->_dir == DIR_DOWN&&m_bJumpable == false)
	{
		StopSound(SOUND_EFFECT);
		PlaySound_Effect(L"59.wav", SOUND_EFFECT, 1.f);
	}

	if (collision->_dir == DIR_DOWN)
		LandingParticle_logic(collision->otherObj->m_pTag);
}

void CThirddee::OnCollisionStay(const Collision * collision)
{
	if (g_Is2D)
	{
		if (collision->_dir == DIR_DOWN)
			m_bJumpable = true;

		if (fabsf(m_pRigid->m_Velocity.y) > 2.f && m_bJumpable)
		{
			m_pAnimation_Leg->SetAnimation(L"Jump");
			m_pAnimation_Arm->SetAnimation(L"Jump");
			m_pAnimation_Head->SetAnimation(L"Idle");
			m_bJumpable = false;
		}
		else if (m_bJumpable)
			if ((fabsf(m_pRigid->m_Velocity.x) > 1.f))
			{
				Set_WalkParticle();
				m_pAnimation_Head->SetAnimation(L"Walk");
				m_pAnimation_Leg->SetAnimation(L"Walk");
				m_pAnimation_Arm->SetAnimation(L"Walk");
			}
			else {
				m_pAnimation_Head->SetAnimation(L"Idle");
				m_pAnimation_Leg->SetAnimation(L"Idle");
				m_pAnimation_Arm->SetAnimation(L"Idle");
			}
	}
	
}

void CThirddee::OnCollisionExit(const Collision * collision)
{
	m_bJumpable = false;
}

void CThirddee::Key_Input(const _float & fTimeDelta)
{
	m_fWalkTime += fTimeDelta;
	if (Engine::Get_DIKeyState(DIK_LEFT) == Engine::KEYDOWN)
		m_eKeyState = DIR_LEFT;


	if (Engine::Get_DIKeyState(DIK_RIGHT) == Engine::KEYDOWN)
		m_eKeyState = DIR_RIGHT;


	if (Engine::Get_DIKeyState(DIK_LEFT) == Engine::KEYPRESS)
	{
		m_pRigid->m_Velocity.x = -m_fSpeed;
		PlaySound_Effect(L"78.wav", SOUND_EFFECT, 1.f);

	}
	if (Engine::Get_DIKeyState(DIK_RIGHT) == Engine::KEYPRESS)
	{
		m_pRigid->m_Velocity.x = m_fSpeed;
		PlaySound_Effect(L"78.wav", SOUND_EFFECT, 1.f);
	}

	if (Engine::Get_DIKeyState(DIK_LEFT) == Engine::KEYUP)
	{
		m_pRigid->m_Velocity.x = -m_fSpeed * 0.2f;
		StopSound(SOUND_EFFECT);
	}
	if (Engine::Get_DIKeyState(DIK_RIGHT) == Engine::KEYUP)
	{
		m_pRigid->m_Velocity.x = m_fSpeed * 0.2f;
		StopSound(SOUND_EFFECT);
	}
	if (Engine::Get_DIKeyState(DIK_SPACE) == Engine::KEYDOWN && m_bJumpable)
	{
		Engine::StopSound(SOUND_EFFECT);
		Engine::PlaySound_Effect(L"54.wav", SOUND_EFFECT, 1.f);
		//착지소리 59번,구름착지 58번

		m_pRigid->AddForce(_vec3(0, 1, 0), 90.f, IMPULSE, fTimeDelta);
		BoundingBox box;
		box.Offset(m_pTransform->m_vInfo[INFO_POS]);
		m_pJumpParticle->Set_BoundingBox(box);
		m_pJumpParticle->Set_Size(1.f);
		m_pJumpParticle->Start_Particle();
	}
}

HRESULT CThirddee::Add_Component(void)
{
	CComponent*		pComponent = nullptr;

	pComponent = m_pRigid = dynamic_cast<CRigidbody*>(Engine::Clone_Proto(L"Rigidbody", this));
	NULL_CHECK_RETURN(m_pRigid, E_FAIL);
	m_vecComponent[ID_DYNAMIC].push_back({ L"Rigidbody", pComponent });

	pComponent = m_pJumpParticle = dynamic_cast<CJumpParticle*>(Engine::Clone_Proto(L"JumpParticle", this));
	NULL_CHECK_RETURN(m_pJumpParticle, E_FAIL);
	m_vecComponent[ID_STATIC].push_back({ L"JumpParticle", pComponent });

	pComponent = m_pLandingParticle = dynamic_cast<CLandingParticle*>(Engine::Clone_Proto(L"LandingParticle", this));
	NULL_CHECK_RETURN(m_pLandingParticle, E_FAIL);
	m_vecComponent[ID_STATIC].push_back({ L"LandingParticle", pComponent });

	pComponent = m_pSparkParticle = dynamic_cast<CSparkParticle*>(Engine::Clone_Proto(L"WalkParticle", this));
	NULL_CHECK_RETURN(m_pSparkParticle, E_FAIL);
	m_vecComponent[ID_STATIC].push_back({ L"WalkParticle", pComponent });

	pComponent = m_pCollider = dynamic_cast<CCollider*>(Engine::Clone_Proto(L"Collider", this));
	NULL_CHECK_RETURN(m_pCollider, E_FAIL);
	m_vecComponent[ID_DYNAMIC].push_back({ L"Collider", pComponent });

	pComponent = m_pShadow = dynamic_cast<CShadow*>(Engine::Clone_Proto(L"Shadow", this));
	NULL_CHECK_RETURN(m_pShadow, E_FAIL);
	m_vecComponent[ID_STATIC].push_back({ L"Shadow", pComponent });

	pComponent = m_pAnimation_Arm = dynamic_cast<CAnimation*>(Engine::Clone_Proto(L"Animation", this));
	NULL_CHECK_RETURN(m_pAnimation_Arm, E_FAIL);
	m_vecComponent[ID_DYNAMIC].push_back({ L"Animation", pComponent });

	pComponent = m_pAnimation_Leg = dynamic_cast<CAnimation*>(Engine::Clone_Proto(L"Animation", this));
	NULL_CHECK_RETURN(m_pAnimation_Leg, E_FAIL);
	m_vecComponent[ID_DYNAMIC].push_back({ L"Animation", pComponent });

	pComponent = m_pAnimation_Head = dynamic_cast<CAnimation*>(Engine::Clone_Proto(L"Animation", this));
	NULL_CHECK_RETURN(m_pAnimation_Head, E_FAIL);
	m_vecComponent[ID_DYNAMIC].push_back({ L"Animation", pComponent });

	return S_OK;
}

CThirddee * CThirddee::Create(LPDIRECT3DDEVICE9 pGraphicDev, _vec3 & vPos)
{
	CThirddee*		pInstance = new CThirddee(pGraphicDev);

	if (FAILED(pInstance->Ready_GameObject(vPos)))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CThirddee::Free(void)
{
	__super::Free();
}
