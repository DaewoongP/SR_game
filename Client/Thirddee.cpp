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
	m_pTextureCom->Add_Anim(L"Die", 67, 72, 0.6f, false);
	m_pTextureCom->Switch_Anim(L"Idle");
	m_pTextureCom->m_bUseFrameAnimation = true;
	m_bRender = true;
	m_pCollider->Set_BoundingBox({ 1.f,2.f,1.0f });
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
	return _int();
}

_int CThirddee::Update_Too(const _float & fTimeDelta)
{
	__super::Update_Too(fTimeDelta);
	return _int();
}

_int CThirddee::Update_Top(const _float & fTimedDelte)
{
	__super::Update_Top(fTimedDelte);
	return _int();
}

void CThirddee::LateUpdate_GameObject(void)
{
	__super::LateUpdate_GameObject();
}

void CThirddee::Render_GameObject(void)
{
	__super::Render_GameObject();
}

void CThirddee::Render_Too(void)
{
	__super::Render_Too();
}

void CThirddee::Render_Top(void)
{
	__super::Render_Top();
}

void CThirddee::SwapTrigger()
{
	__super::SwapTrigger();
}

HRESULT CThirddee::Add_Component(void)
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
