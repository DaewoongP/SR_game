#include "stdafx.h"
#include "Tookee.h"
#include "AbstractFactory.h"
#include "MoveCube.h"
#include "PortalCube.h"
#include "Export_Function.h"

CTookee::CTookee(LPDIRECT3DDEVICE9 pGraphicDev)
	:CGameObject(pGraphicDev)
	, m_bJumpable(false)
	, m_eKeyState(DIR_END)
{
	m_bInit = true;
}

CTookee::~CTookee()
{
}

HRESULT CTookee::Ready_GameObject(_vec3 & vPos)
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);
	__super::Ready_GameObject(vPos);

	m_MovetoPos = m_pTransform->m_vInfo[INFO_POS];
	m_LookVec = _vec3(0, 0, 0);

	m_pTransform->m_vScale = { 1.f, 1.f, 1.f };
	m_pTransform->m_vInfo[INFO_POS] = vPos;
	m_pTransform->m_bIsStatic = false;
	m_pCollider->m_bIsTrigger = false;
	// 애니메이션
	m_bInit2 = true;
	m_pTextureCom->Add_Anim(L"Idle", 0, 5, 1.f, true);
	m_pTextureCom->Add_Anim(L"Walk", 6, 13, 1.f, true);
	m_pTextureCom->Add_Anim(L"Jump", 26, 30, 1.f, false);
	m_pTextureCom->Switch_Anim(L"Idle");
	m_pTextureCom->m_bUseFrameAnimation = true;
	m_bRender = true;
	m_pCollider->Set_BoundingBox({ 1.f,2.f,1.0f });
	BoundingBox box;
	box.Offset(vPos);
	m_pLandingParticle->Set_BoundingBox(box);
	m_pSparkParticle->Set_LifeTime();
	return S_OK;
}

_int CTookee::Update_GameObject(const _float & fTimeDelta)
{
	if (m_bInit2)
	{

		CComponent* otherTrans = Engine::Get_Component(L"Layer_GameLogic", L"Toodee", L"Transform", ID_DYNAMIC);
		if (otherTrans != nullptr)
			dynamic_cast<CToodee*>(otherTrans->m_pGameObject)->SetTookee(this);

		otherTrans = Engine::Get_Component(L"Layer_GameLogic", L"Topdee", L"Transform", ID_DYNAMIC);
		if (otherTrans != nullptr)
			dynamic_cast<CTopdee*>(otherTrans->m_pGameObject)->SetTookee(this);

		//실행해주는 코드
		CLayer* pStageLayer = dynamic_cast<CLayer*>(Engine::Get_Layer(L"Layer_GameLogic"));
		NULL_CHECK_RETURN(pStageLayer, E_FAIL);

		//머리
		FAILED_CHECK_RETURN(FACTORY<CTopdeeParts>::Create(L"Tookee_Head", pStageLayer, _vec3(-1.f, 0.f, -0.2f), m_pTransform, L"Tookee_Head", 0, false), E_FAIL);
		//몸통
		FAILED_CHECK_RETURN(FACTORY<CTopdeeParts>::Create(L"Tookee_Body", pStageLayer, _vec3(-1.f, 0.f, -0.2f), m_pTransform, L"Tookee_Body", 0, false), E_FAIL);

		//조인트는 몸통꺼
		FAILED_CHECK_RETURN(FACTORY<CTopdeeJoint>::Create(L"TopdeeBody", pStageLayer, _vec3(-1.f, 0.f, -0.2f), m_pTransform->GetChild(1)), E_FAIL);
		//조인트
		FAILED_CHECK_RETURN(FACTORY<CTopdeeJoint>::Create(L"TopdeeBody", pStageLayer, _vec3(-1.f, 0.f, -0.2f), m_pTransform->GetChild(1)), E_FAIL);
		//조인트
		FAILED_CHECK_RETURN(FACTORY<CTopdeeJoint>::Create(L"TopdeeBody", pStageLayer, _vec3(-1.f, 0.f, -0.2f), m_pTransform->GetChild(1)), E_FAIL);
		//조인트
		FAILED_CHECK_RETURN(FACTORY<CTopdeeJoint>::Create(L"TopdeeBody", pStageLayer, _vec3(-1.f, 0.f, -0.2f), m_pTransform->GetChild(1)), E_FAIL);

		//팔은 조인트 꺼
		FAILED_CHECK_RETURN(FACTORY<CTopdeeParts>::Create(L"TopdeeArm", pStageLayer, _vec3(-1.f, 0.f, -0.2f), m_pTransform->GetChild(1)->GetChild(0), L"Tookee_Arm", 0, false), E_FAIL);
		//팔
		FAILED_CHECK_RETURN(FACTORY<CTopdeeParts>::Create(L"TopdeeArm", pStageLayer, _vec3(-1.f, 0.f, -0.2f), m_pTransform->GetChild(1)->GetChild(1), L"Tookee_Arm", 0, false), E_FAIL);
		//다리
		FAILED_CHECK_RETURN(FACTORY<CTopdeeParts>::Create(L"TopdeeLeg", pStageLayer, _vec3(-1.f, 0.f, -0.2f), m_pTransform->GetChild(1)->GetChild(2), L"Tookee_Leg", 0, false), E_FAIL);
		//다리
		FAILED_CHECK_RETURN(FACTORY<CTopdeeParts>::Create(L"TopdeeLeg", pStageLayer, _vec3(-1.f, 0.f, -0.2f), m_pTransform->GetChild(1)->GetChild(3), L"Tookee_Leg", 0, false), E_FAIL);

		m_partVec.push_back(dynamic_cast<CTopdeeParts*>(m_pTransform->GetChild(0)->m_pGameObject));
		m_partVec.push_back(dynamic_cast<CTopdeeParts*>(m_pTransform->GetChild(1)->m_pGameObject));
		for (int i = 0; i < m_pTransform->GetChild(1)->GetChildCount(); i++)
			if (dynamic_cast<CTopdeeParts*>(m_pTransform->GetChild(1)->GetChild(i)->GetChild(0)->m_pGameObject))
				m_partVec.push_back(dynamic_cast<CTopdeeParts*>(m_pTransform->GetChild(1)->GetChild(i)->GetChild(0)->m_pGameObject));

		//머리
		m_partVec[0]->m_pTransform->m_vScale = _vec3(2.5f, 2.5f, 2.5f);
		m_partVec[0]->m_pTransform->m_vInfo[INFO_POS] = _vec3(0, 0, -1);

		//몸통
		m_partVec[1]->m_pTransform->m_vScale = _vec3(1.6f, 1.7f, 1.2f);
		m_partVec[1]->m_pTransform->m_vInfo[INFO_POS] = _vec3(0.0f, -1.f, -0.9f);

		//조이느
		m_partVec[2]->m_pTransform->GetParent()->m_vScale = _vec3(1, 1, 1);
		m_partVec[2]->m_pTransform->GetParent()->m_vInfo[INFO_POS] = _vec3(-0.8f, -0.1f, -0.4f);
		m_partVec[3]->m_pTransform->GetParent()->m_vScale = _vec3(1, 1, 1);
		m_partVec[3]->m_pTransform->GetParent()->m_vInfo[INFO_POS] = _vec3(0.8f, -0.1f, -0.4f);
		m_partVec[4]->m_pTransform->GetParent()->m_vScale = _vec3(1, 1, 1);
		m_partVec[4]->m_pTransform->GetParent()->m_vInfo[INFO_POS] = _vec3(-0.5f, 0.1f, 0.2f);
		m_partVec[5]->m_pTransform->GetParent()->m_vScale = _vec3(1, 1, 1);
		m_partVec[5]->m_pTransform->GetParent()->m_vInfo[INFO_POS] = _vec3(0.5f, 0.1f, 0.2f);

		//팔다리
		m_partVec[2]->m_pTransform->m_vScale = _vec3(0.3f, 0.8f, 1);
		m_partVec[2]->m_pTransform->m_vInfo[INFO_POS] = _vec3(0, 0, 0.3f);
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

		SetRenderONOFF(false);
		m_bInit2 = false;
	}
	Engine::Add_RenderGroup(RENDER_NONALPHA, this);
	return 0;
}

_int CTookee::Update_Too(const _float & fTimeDelta)
{
	if (m_bDead)
		return OBJ_DEAD;
	if(m_moveTrue)
		Key_Input(fTimeDelta);
	DoStrech();
	CGameObject::Update_GameObject(fTimeDelta);

	m_pTextureCom->Update_Anim(fTimeDelta);

	//텍스쳐컴의 애니가 die고 완료됐다면?
	if (m_pTextureCom->IsAnimationEnd(L"Die"))
		m_bDead = true;

	DoFlip();
	return 0;
}

_int CTookee::Update_Top(const _float & fTimedDelte)
{
	_vec3 moveto;
	D3DXVec3Normalize(&moveto, &m_LookVec);
	
	Key_Input2(fTimedDelte);
	_float dir = D3DXVec3Dot(&_vec3(0, -1, 0), &moveto);
	dir = acosf(dir);
	if (0> moveto.x)
		dir = 2 * D3DX_PI - dir;

	m_partVec[1]->m_pTransform->m_vAngle.z = lerp_angle(m_partVec[1]->m_pTransform->m_vAngle.z, dir, 0.1f);

	m_partVec[2]->m_pTransform->m_vAngle.y = lerp_angle(m_partVec[2]->m_pTransform->m_vAngle.y, dir, 0.1f);
	m_partVec[3]->m_pTransform->m_vAngle.y = lerp_angle(m_partVec[2]->m_pTransform->m_vAngle.y, dir, 0.1f);
	m_partVec[4]->m_pTransform->m_vAngle.y = lerp_angle(m_partVec[2]->m_pTransform->m_vAngle.y, dir, 0.1f);
	m_partVec[5]->m_pTransform->m_vAngle.y = lerp_angle(m_partVec[2]->m_pTransform->m_vAngle.y, dir, 0.1f);

	_int texvalue = (_int)(D3DXToDegree(m_partVec[1]->m_pTransform->m_vAngle.z) / 10) % 36;
	m_partVec[0]->SetTextureIdx((texvalue<0) ? 11 - abs(texvalue/3) : texvalue/3);

	RayDiskey();
	if (m_bIsMoving)
		Move(fTimedDelte);
	PlayerState(fTimedDelte);

	__super::Update_GameObject(fTimedDelte);
	return 0;
}

void CTookee::LateUpdate_GameObject(void)
{
	__super::LateUpdate_GameObject();
}

void CTookee::Render_GameObject(void)
{
	Engine::Add_RenderGroup(RENDER_ALPHA, this);
	__super::Render_GameObject();
}

void CTookee::Render_Too(void)
{
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	m_pGraphicDev->SetRenderState(D3DRS_ALPHATESTENABLE,TRUE);
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransform->Get_WorldMatrixPointer());
	m_pTextureCom->Set_Texture(0);
	m_pShadow->Render_Shadow(m_pBufferCom);
	m_pBufferCom->Render_Buffer();
	m_pGraphicDev->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	Render_Particle();
}

void CTookee::Render_Top(void)
{
	__super::Render_GameObject();
}

void CTookee::SwapTrigger()
{
	if (g_Is2D)
	{
		m_pRigid->m_bUseGrivaty = true;
		SetRenderONOFF(false);
		m_pTransform->m_vInfo[INFO_POS].z=10;
		m_pCollider->m_bIsTrigger = false;
		m_pCollider->Set_BoundingBox({ 1.f,2.f,1.0f });
	}
	else
	{
		m_pTransform->m_vInfo[INFO_POS].z=11;
		m_pRigid->m_bUseGrivaty = false;
		m_pTransform->m_vInfo[INFO_POS].x =
			((int)m_pTransform->m_vInfo[INFO_POS].x % 2 == 0) ? ((int)m_pTransform->m_vInfo[INFO_POS].x) : ((int)m_pTransform->m_vInfo[INFO_POS].x + 1);
		m_pTransform->m_vInfo[INFO_POS].y =
			((int)m_pTransform->m_vInfo[INFO_POS].y % 2 == 0) ? ((int)m_pTransform->m_vInfo[INFO_POS].y) : ((int)m_pTransform->m_vInfo[INFO_POS].y + 1);
		SetRenderONOFF(true);
		m_pCollider->Set_BoundingBox({ 0.999f,1.999f,1.0f });
		m_pCollider->m_bIsTrigger = true;
	}
	m_bIsMoving = false;
	m_MovetoPos = m_pTransform->m_vInfo[INFO_POS];
	m_byPlayerInputDir = 0;
	m_pRigid->m_Velocity = _vec3(0, 0, 0);
	m_pTransform->m_vAngle = _vec3(0, 0, 0);
	m_pTransform->m_vScale = _vec3(1, 1, 1);
}

void CTookee::OnCollisionEnter(const Collision * collision)
{
	if (g_Is2D)
	{
		//스파이크랑 충돌시 본인 데드 트루
		if (!lstrcmp(collision->otherObj->m_pTag, L"Spike") &&
			collision->_dir == DIR_DOWN)
		{
		}
		if (!lstrcmp(m_pTextureCom->Get_AnimState(), L"Die") && m_bDead == false)
		{
			StopSound(SOUND_EFFECT);
			PlaySound_Effect(L"9.wav", SOUND_EFFECT, 1.f);
		}

		if (collision->_dir == DIR_DOWN)
		{
			LandingParticle_logic(collision->otherObj->m_pTag);
		}
			

		__super::OnCollisionEnter(collision);
	}
	else
	{
		if (!lstrcmp(collision->otherObj->m_pTag, L"Boss3Left") && collision->_dir == DIR_FRONT ||
			!lstrcmp(collision->otherObj->m_pTag, L"Boss3Right") && collision->_dir == DIR_FRONT ||
			!lstrcmp(collision->otherObj->m_pTag, L"Boss3") && collision->_dir == DIR_FRONT)
		{
			int i = 0;
		}
		__super::OnCollisionEnter(collision);
	}
	
}

void CTookee::OnCollisionStay(const Collision * collision)
{
	if (g_Is2D)
	{
		if (lstrcmp(m_pTextureCom->Get_AnimState(), L"Die"))
		{
			if (collision->_dir == DIR_DOWN)
				m_bJumpable = true;

			if (fabsf(m_pRigid->m_Velocity.y) > 2.f && m_bJumpable)
			{
				m_bJumpable = false;
				m_pTextureCom->Switch_Anim(L"Jump");
			}
			else if (m_bJumpable)
				if ((fabsf(m_pRigid->m_Velocity.x) > 1.f))
				{
					m_pTextureCom->Switch_Anim(L"Walk");
					Set_WalkParticle();
				}

				else
					m_pTextureCom->Switch_Anim(L"Idle");
		}

		CGameObject::OnCollisionStay(collision);
	}
	else 
	{
		__super::OnCollisionStay(collision);
	}
	
}

void CTookee::OnCollisionExit(const Collision * collision)
{
	if (g_Is2D)
	{
		m_bJumpable = false;
		CGameObject::OnCollisionExit(collision);
	}
	else 
	{
		__super::OnCollisionExit(collision);
	}
}

HRESULT CTookee::Add_Component(void)
{
	CComponent*		pComponent = nullptr;

	pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(Engine::Clone_Proto(L"RcTex", this));
	NULL_CHECK_RETURN(m_pBufferCom, E_FAIL);
	m_vecComponent[ID_STATIC].push_back({ L"RcTex", pComponent });

	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Tookee_Too", this));
	NULL_CHECK_RETURN(m_pTextureCom, E_FAIL);
	m_vecComponent[ID_STATIC].push_back({ L"Texture", pComponent });
	
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

	return S_OK;
}

CTookee * CTookee::Create(LPDIRECT3DDEVICE9 pGraphicDev, _vec3 & vPos)
{
	CTookee*		pInstance = new CTookee(pGraphicDev);

	if (FAILED(pInstance->Ready_GameObject(vPos)))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CTookee::Free(void)
{
	__super::Free();
}

void CTookee::Key_Input(const _float & fTimeDelta)
{
	m_fWalkTime += fTimeDelta;
	if (Engine::Get_DIKeyState(DIK_LEFT) == Engine::KEYDOWN)
		m_eKeyState = DIR_LEFT;


	if (Engine::Get_DIKeyState(DIK_RIGHT) == Engine::KEYDOWN)
		m_eKeyState = DIR_RIGHT;


	if (Engine::Get_DIKeyState(DIK_LEFT) == Engine::KEYPRESS)
	{
		PlaySound_Effect(L"78.wav", SOUND_EFFECT, 1.f);

	}
	if (Engine::Get_DIKeyState(DIK_RIGHT) == Engine::KEYPRESS)
	{
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

void CTookee::Key_Input2(const _float & fTimeDelta)
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

void CTookee::RayDiskey()
{
	_int fdir[MD_END] = { 2,1,8,4,6,10,5,9 };
	//플레이어가 이동하려는 방향으로만 검출합니다
	for (int i = 0; i < MD_END; i++)
	{
		if (m_byPlayerInputDir == fdir[i])
			RayDisKey_part((COL_MOVEDIR)i);
	}
}

void CTookee::RayDisKey_part(COL_MOVEDIR dir)
{
	_vec3 vdir[MD_END] = { { 0,1,0 },{ 0,-1,0 },{ -1,0,0 },{ 1,0,0 },{ 1,1,0 },{ -1,1,0 },{ 1,-1,0 },{ -1,-1,0 } };
	_int fdir[MD_END] = { 13,14,7,11,9,5,10,6 };
	vector<RayCollision> _detectedCOL = Engine::Check_Collision_Ray(RAYCAST(m_pTransform->m_vInfo[INFO_POS], vdir[dir], 2.125f), m_pCollider);
	for (int i = 0; i < _detectedCOL.size(); i++)
	{
		if (!lstrcmp(_detectedCOL[i].tag, L"MapCube") ||
			!lstrcmp(_detectedCOL[i].tag, L"InstallGrid") ||
			!lstrcmp(_detectedCOL[i].tag, L"CrackCube") ||
			!lstrcmp(_detectedCOL[i].tag, L"InstallCube") ||
			!lstrcmp(_detectedCOL[i].tag, L"SwitchCube") ||
			!lstrcmp(_detectedCOL[i].tag, L"Boss3") ||
			!lstrcmp(_detectedCOL[i].tag, L"Boss3Left") ||
			!lstrcmp(_detectedCOL[i].tag, L"Boss3Right")
			) m_byPlayerInputDir &= fdir[dir];
		if (!lstrcmp(_detectedCOL[i].tag, L"MoveCube") ||
			!lstrcmp(_detectedCOL[i].tag, L"GravityCube"))
		{

			if (dir < 4)
			{
				COL_DIR destdir;
				if (dir % 2 == 0)
					destdir = (COL_DIR)(dir + 1);
				else
					destdir = (COL_DIR)(dir - 1);

				if (dynamic_cast<CMoveCube*>(_detectedCOL[i].col->m_pGameObject)->m_bIsCol[destdir])
					m_byPlayerInputDir &= fdir[dir];
			}
		}
		if (!lstrcmp(_detectedCOL[0].tag, L"PortalCube"))
		{
			COL_DIR destdir = (COL_DIR)dynamic_cast<CPortalCube*>(_detectedCOL[i].col->m_pGameObject)->Get_CubeDir();
			if (dir % 2 != 0)
				destdir = (COL_DIR)(destdir + 1);
			else
				destdir = (COL_DIR)(destdir - 1);
			_bool isBlocked = dynamic_cast<CMoveCube*>(_detectedCOL[i].col->m_pGameObject)->m_bIsCol[dir];

			//만약 플레이어가 이동하려는 방향이 입구가 아니고
			//그쪽방향으로 막혀있다면?
			if (destdir == dir&& !isBlocked)
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

				if (dynamic_cast<CMoveCube*>(_detectedCOL[i].col->m_pGameObject)->m_bIsCol[destdir])
					m_byPlayerInputDir &= fdir[dir];
			}

		}
	}
}

void CTookee::PlayerState(const _float & fTimeDelta)
{
	int x = 0; int y = 0;
	switch (m_eState)
	{
	case Engine::TD_MOVE:
		if (!m_bIsMoving)
		{
			if (m_moveTrue)
			{
				DirApply(m_byPlayerInputDir, x, y);
				m_MovetoPos = _vec3((int)m_pTransform->m_vInfo[INFO_POS].x + x, (int)m_pTransform->m_vInfo[INFO_POS].y + y, (int)m_pTransform->m_vInfo[INFO_POS].z);
			}
			if (x != 0 || y != 0)
				m_LookVec = _vec3(x, y, 0);
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
		DirApply(m_byLookDir, x, y);

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

void CTookee::Move(const _float & fTimeDelta)
{
	_vec3 dir;
	D3DXVec3Normalize(&dir, &_vec3(m_MovetoPos - m_pTransform->m_vInfo[INFO_POS]));
	m_pTransform->m_vInfo[INFO_POS] += dir*m_fSpeed*fTimeDelta;

	m_pAnimation_Leg->SetAnimation(L"Walk");
	if (m_pGrabObj == nullptr)
		m_pAnimation_Arm->SetAnimation(L"Walk");
	//만약 도달했다면?
	if (D3DXVec3Length(&_vec3(m_pTransform->m_vInfo[INFO_POS] - m_MovetoPos)) < 0.3f)
	{
		m_pTransform->m_vInfo[INFO_POS] = m_MovetoPos;
		m_bIsMoving = false;
		return;
	}
}

_bool CTookee::CheckCubeExist(_vec3 dir, CCollider ** col)
{
	vector<RayCollision> _detectedCOL = Engine::Check_Collision_Ray(RAYCAST(m_pTransform->m_vInfo[INFO_POS], dir, 1.5f), m_pCollider);

	if (_detectedCOL.size() >= 1)
	{
		if (!lstrcmp(_detectedCOL[0].tag, L"MoveCube") ||
			!lstrcmp(_detectedCOL[0].tag, L"GravityCube") ||
			!lstrcmp(_detectedCOL[0].tag, L"PortalCube"))
		{
			*col = _detectedCOL[0].col;
			return true;
		}
	}
	return false;
}

_bool CTookee::CheckAnythingExist(_vec3 dir, CCollider ** col)
{
	_vec3 centerpos = m_pTransform->m_vInfo[INFO_POS];
	vector<RayCollision> _detectedCOL = Engine::Check_Collision_Ray(RAYCAST(centerpos, dir, 1.5f), m_pCollider);

	if (_detectedCOL.size() >= 1)
	{
		if (!lstrcmp(_detectedCOL[0].tag, L"InstallGrid"))
			return false;
		return true;
	}
	return false;
}

void CTookee::DirApply(_int dir, _int & x, _int & y)
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

void CTookee::DoFlip()
{
	if (m_eKeyState == DIR_LEFT)
		m_pTransform->m_vAngle.y = Lerp(m_pTransform->m_vAngle.y, D3DXToRadian(180), 0.1f);
	else if (m_eKeyState == DIR_RIGHT)
		m_pTransform->m_vAngle.y = Lerp(m_pTransform->m_vAngle.y, 0, 0.1f);
}

void CTookee::DoStrech()
{
	if (-1.f > m_pRigid->m_Velocity.y)
	{
		if (m_pTransform->m_vScale.y > 2.0f)
			return;
		m_pTransform->m_vScale.y *= 1.02f;
	}
	else
		m_pTransform->m_vScale.y = Lerp(m_pTransform->m_vScale.y, 1.f, 0.5f);
}

void CTookee::Render_Particle()
{
	m_pJumpParticle->Update_Particle();
	m_pLandingParticle->Update_Particle();
	m_pSparkParticle->Update_Particle();
}

void CTookee::Check_IsParticleDead()
{
	if (m_pJumpParticle->IsDead())
		m_pJumpParticle->End_Particle();
	if (m_pLandingParticle->OverOneParticleIsDead())
		m_pLandingParticle->End_Particle();
	if (m_pSparkParticle->IsDead())
		m_pSparkParticle->End_Particle();
}

void CTookee::Set_WalkParticle()
{
	_float fTimeDelta = Engine::Get_Timer(L"Timer_FPS60");

	if (Engine::IsPermit_Call(L"1Sec", fTimeDelta))
	{
		BoundingBox box;
		_vec3 vPos = m_pTransform->m_vInfo[INFO_POS];
		box._offsetMin = { -0.1f, -1.f, 0.f };
		box._offsetMax = { 0.f, -0.8f, 0.f };
		box.Offset(vPos);
		m_pSparkParticle->Set_BoundingBox(box);
		m_pSparkParticle->Reset();
		m_pSparkParticle->Set_Size(0.8f);
		m_pSparkParticle->Start_Particle();
	}
}

void CTookee::LandingParticle_logic(const _tchar * pTag)
{
	if (m_bJumpable)
		return;
	if (!lstrcmp(pTag, L"MapCube") ||
		!lstrcmp(pTag, L"CrackCube") ||
		!lstrcmp(pTag, L"KeyCube") ||
		!lstrcmp(pTag, L"MoveCube") ||
		!lstrcmp(pTag, L"GravityCube") ||
		!lstrcmp(pTag, L"PinkCloud"))
	{
		m_pLandingParticle->Reset();
		m_pLandingParticle->Set_Size(1.f);
		m_pLandingParticle->Start_Particle();
	}
}
void CTookee::SetMovePos(COL_DIR dir)
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

void CTookee::SetMovePos_zero()
{
	m_bIsMoving = true;
	m_MovetoPos = m_pTransform->m_vInfo[INFO_POS];
}
