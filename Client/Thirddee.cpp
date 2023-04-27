#include "stdafx.h"
#include "Thirddee.h"
#include "AbstractFactory.h"
#include "MoveCube.h"
#include "PortalCube.h"
#include "Export_Function.h"
#include <functional>
#include "ThirdCamera.h"

CThirddee::CThirddee(LPDIRECT3DDEVICE9 pGraphicDev)
	:CTookee(pGraphicDev)
{
}

CThirddee::~CThirddee()
{
}

HRESULT CThirddee::Ready_GameObject(_vec3 & vPos, _int stage)
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);
	m_ThirdDee_Stage = stage;
	m_MovetoPos = m_pTransform->m_vInfo[INFO_POS];
	m_LookVec = _vec3(0, 0, 0);
	m_EndingTrigger = false;
	m_pTransform->m_vScale = { 0.6f, 0.6f, 0.6f };
	m_pTransform->m_vInfo[INFO_POS] = vPos;
	m_pTransform->m_bIsStatic = false;
	m_pCollider->m_bIsTrigger = false;
	// �ִϸ��̼�
	m_bInit2 = true;
	m_bRender = true;
	m_pCollider->Set_BoundingBox({ 1.f,2.f,1.f });
	m_pCollider->Set_BoundOffset(_vec3(0,0,0));
	BoundingBox box;
	m_moveTrue = true;
	box.Offset(vPos);
	m_pLandingParticle->Set_BoundingBox(box);
	m_pSparkParticle->Set_LifeTime();
	m_fTopOffset = 0;
	//옾셋만큼 팔다리 몸통을 내리면 될듯요.

	return S_OK;
}

_int CThirddee::Update_GameObject(const _float & fTimeDelta)
{
	if (m_bInit2)
	{
		//�������ִ� �ڵ�
		CLayer* pStageLayer = dynamic_cast<CLayer*>(Engine::Get_Layer(L"Layer_GameLogic"));
		NULL_CHECK_RETURN(pStageLayer, E_FAIL);

		//�Ӹ�
		FAILED_CHECK_RETURN(FACTORY<CTopdeeParts>::Create(L"Third_Head", pStageLayer, _vec3(0,0,0), m_pTransform, L"Third_Head", 0, false), E_FAIL);
		FAILED_CHECK_RETURN(FACTORY<CTopdeeParts>::Create(L"Third_BackHead", pStageLayer, _vec3(0,0, -0.01f), m_pTransform->GetChild(0), L"Third_Head", 1, false), E_FAIL);
		//����
		FAILED_CHECK_RETURN(FACTORY<CTopdeeParts>::Create(L"Third_Body", pStageLayer, _vec3(-1.f, 0.f, -0.2f), m_pTransform, L"Third_Body", 0, false), E_FAIL);

		//����Ʈ�� ���벨
		FAILED_CHECK_RETURN(FACTORY<CTopdeeJoint>::Create(L"Third_Joint", pStageLayer, _vec3(-1.f, 0.f, -0.2f), m_pTransform->GetChild(1)), E_FAIL);
		//����Ʈ
		FAILED_CHECK_RETURN(FACTORY<CTopdeeJoint>::Create(L"Third_Joint", pStageLayer, _vec3(-1.f, 0.f, -0.2f), m_pTransform->GetChild(1)), E_FAIL);
		//����Ʈ
		FAILED_CHECK_RETURN(FACTORY<CTopdeeJoint>::Create(L"Third_Joint", pStageLayer, _vec3(-1.f, 0.f, -0.2f), m_pTransform->GetChild(1)), E_FAIL);
		//����Ʈ
		FAILED_CHECK_RETURN(FACTORY<CTopdeeJoint>::Create(L"Third_Joint", pStageLayer, _vec3(-1.f, 0.f, -0.2f), m_pTransform->GetChild(1)), E_FAIL);

		//���� ����Ʈ ��
		FAILED_CHECK_RETURN(FACTORY<CTopdeeParts>::Create(L"Third_Hand", pStageLayer, _vec3(-1.f, 0.f, -0.2f), m_pTransform->GetChild(1)->GetChild(0), L"Third_Hand", 0, false), E_FAIL);
		//��
		FAILED_CHECK_RETURN(FACTORY<CTopdeeParts>::Create(L"Third_Hand", pStageLayer, _vec3(-1.f, 0.f, -0.2f), m_pTransform->GetChild(1)->GetChild(1), L"Third_Hand", 0, false), E_FAIL);
		//�ٸ�
		FAILED_CHECK_RETURN(FACTORY<CTopdeeParts>::Create(L"Third_Foot", pStageLayer, _vec3(-1.f, 0.f, -0.2f), m_pTransform->GetChild(1)->GetChild(2), L"Third_Foot", 0, false), E_FAIL);
		//�ٸ�
		FAILED_CHECK_RETURN(FACTORY<CTopdeeParts>::Create(L"Third_Foot", pStageLayer, _vec3(-1.f, 0.f, -0.2f), m_pTransform->GetChild(1)->GetChild(3), L"Third_Foot", 0, false), E_FAIL);

		m_partVec.push_back(dynamic_cast<CTopdeeParts*>(m_pTransform->GetChild(0)->m_pGameObject));
		m_partVec.push_back(dynamic_cast<CTopdeeParts*>(m_pTransform->GetChild(1)->m_pGameObject));
		for (int i = 0; i < m_pTransform->GetChild(1)->GetChildCount(); i++)
			if (dynamic_cast<CTopdeeParts*>(m_pTransform->GetChild(1)->GetChild(i)->GetChild(0)->m_pGameObject))
				m_partVec.push_back(dynamic_cast<CTopdeeParts*>(m_pTransform->GetChild(1)->GetChild(i)->GetChild(0)->m_pGameObject));

		//�Ӹ�
		m_partVec[0]->m_pTransform->m_vScale = _vec3(1.8f, 1.8f, 1.8f);
		m_partVec[0]->m_pTransform->m_vInfo[INFO_POS] = _vec3(0, 1+ m_fTopOffset, -0.01f);

		//���
		m_partVec[0]->m_pTransform->GetChild(0)->m_vInfo[INFO_POS] = _vec3(0,0,+0.02f);
		dynamic_cast<CTopdeeParts*>(m_partVec[0]->m_pTransform->GetChild(0)->m_pGameObject)->SetTextureIdx(1);


		//����
		m_partVec[1]->m_pTransform->m_vScale = _vec3(2.2f, 2.2f, 2.2f);
		m_partVec[1]->m_pTransform->m_vInfo[INFO_POS] = _vec3(0.0f, -0.0f, 0.f);

		//���̴�
		m_partVec[2]->m_pTransform->GetParent()->m_vScale = _vec3(1, 1, 1);
		m_partVec[2]->m_pTransform->GetParent()->m_vInfo[INFO_POS] = _vec3(-0.6f, 0, -0.f);
		m_partVec[3]->m_pTransform->GetParent()->m_vScale = _vec3(-1, 1, 1);
		m_partVec[3]->m_pTransform->GetParent()->m_vInfo[INFO_POS] = _vec3(0.6f, 0, -0.f);
		m_partVec[4]->m_pTransform->GetParent()->m_vScale = _vec3(1, 1, 1);
		m_partVec[4]->m_pTransform->GetParent()->m_vInfo[INFO_POS] = _vec3(-0.5f, -0.6f, 0.f);
		m_partVec[5]->m_pTransform->GetParent()->m_vScale = _vec3(1, 1, 1);
		m_partVec[5]->m_pTransform->GetParent()->m_vInfo[INFO_POS] = _vec3(0.5f, -0.6f, 0.f);

		//�ȴٸ�
		m_partVec[2]->m_pTransform->m_vScale = _vec3(2.2f,2.8f,2.2f);
		m_partVec[2]->m_pTransform->m_vInfo[INFO_POS] = _vec3(-0.2f, -0.2f, 0.f);
		m_partVec[2]->m_pTransform->m_vAngle = _vec3(D3DXToRadian(00), D3DXToRadian(00), 0);
		m_partVec[3]->m_pTransform->m_vScale = _vec3(2.2f, 2.8f, 2.2f);
		m_partVec[3]->m_pTransform->m_vInfo[INFO_POS] = _vec3(0.2f, -0.2f, 0.f);
		m_partVec[3]->m_pTransform->m_vAngle = _vec3(D3DXToRadian(00), D3DXToRadian(00), 0);
		m_partVec[4]->m_pTransform->m_vScale = _vec3(2.2f, 2.2f, 2.2f);
		m_partVec[4]->m_pTransform->m_vInfo[INFO_POS] = _vec3(0, -0.2f, 0.f);
		m_partVec[4]->m_pTransform->m_vAngle = _vec3(D3DXToRadian(00), D3DXToRadian(00), 0);
		m_partVec[5]->m_pTransform->m_vScale = _vec3(2.2f, 2.2f, 2.2f);
		m_partVec[5]->m_pTransform->m_vInfo[INFO_POS] = _vec3(0, -0.2f, 0.f);
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
					_vec3(D3DXToRadian(180),D3DXToRadian(0),D3DXToRadian(0)),//rotation
					_vec3(0,0,0),//scale
					0.3f,//tilltime
					0.f//actionTime
				});
				clip->source[0].push_back(
					ANIMINFO{
					_vec3(0,0,0),
					_vec3(D3DXToRadian(180),D3DXToRadian(0),D3DXToRadian(0)),//rotation
					_vec3(0,0,0),
					0.3f,//tilltime
					0.3f//actionTime
				});
			}
			{
				clip->source[1].push_back(
					ANIMINFO{
					_vec3(0,0,0),
					_vec3(D3DXToRadian(-180),D3DXToRadian(0),D3DXToRadian(0)),//rotation
					_vec3(0,0,0),
					0.3f,//tilltime
					0.f//actionTime
				});
				clip->source[1].push_back(
					ANIMINFO{
					_vec3(0,0,0),
					_vec3(D3DXToRadian(-180),D3DXToRadian(0),D3DXToRadian(0)),//rotation
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
					_vec3(0.01f,1+ m_fTopOffset,-0.01f),
					_vec3(D3DXToRadian(0.0f),0,0),//rotation
					_vec3(0,0,0),//scale
					0.1f,//tilltime
					0.f//actionTime
				});

				clip->source[0].push_back(
					ANIMINFO{
					_vec3(0.01f,1+ m_fTopOffset,-0.01f),
					_vec3(D3DXToRadian(0.0f),0,0),//rotation
					_vec3(0,0,0),
					0.1f,//tilltime
					0.1f//actionTime
				});
			}
			clip->TotalTime = 0.2f;
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
					_vec3(0.5f,1+ m_fTopOffset,-0.01f),
					_vec3(0,0,0),//rotation
					_vec3(0,0,0),//scale
					0.3f,//tilltime
					0.f//actionTime
				});

				clip->source[0].push_back(
					ANIMINFO{
					_vec3(-0.5f,1+ m_fTopOffset,-0.01f),
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

		m_pTransform->m_vScale = _vec3(1, 1, 1);
		SetRenderONOFF(true);
		m_bInit2 = false;
	}
	
	if(m_bDead)
		return OBJ_DEAD;
	if (m_DiePart != nullptr&&m_DiePart->GetDieAnimEnd())
	{
	/*	for (int i = 0; i < m_partVec.size(); i++)
			m_partVec[i]->Set_Dead();
		m_DiePart->Set_Dead();*/
		m_bDead = true;
	}
		

	Spiwn_End(fTimeDelta);
	Engine::Add_RenderGroup(RENDER_NONALPHA, this);
	CGameObject::Update_GameObject(fTimeDelta);
	
	return 0;
}

_int CThirddee::Update_Too(const _float & fTimeDelta)
{
	if (!m_bDead)
	{
		Key_Input(fTimeDelta);

		DoFlip();
	}
	
	return 0;
}

void CThirddee::DoFlip()
{
	if (m_eKeyState == DIR_LEFT)
		m_pTransform->m_vAngle.y = Lerp(m_pTransform->m_vAngle.y, D3DXToRadian(90), 0.1f);
	else if (m_eKeyState == DIR_RIGHT)
		m_pTransform->m_vAngle.y = Lerp(m_pTransform->m_vAngle.y, D3DXToRadian(270), 0.1f);
}
void CThirddee::SetRenderONOFF(_bool value)
{
	function<void(CTransform*)> func = [&](CTransform* parent) -> void {
		for (int i = 0; i < parent->GetChildCount(); i++)
		{
			CTopdeeParts* parts = dynamic_cast<CTopdeeParts*>(parent->GetChild(i)->m_pGameObject);
			if (parts != nullptr)
				parts->SetRenderState(value);
			func(parent->GetChild(i));
		}
	};
	func(m_pTransform);
}
void CThirddee::Spiwn_End(const _float& fTimeDelta)
{
	if (m_EndingTrigger)
	{
		m_pRigid->AddTorque(_vec3(0, 1, 0), fTimeDelta*600);
		m_pTransform->m_vInfo[INFO_POS].z -= 3.f*fTimeDelta;
	}
}

_int CThirddee::Update_Top(const _float & fTimedDelte)
{
	if (!m_bDead)
	{
		if (m_EndingTrigger)
			return 0;
		_vec3 moveto;
		D3DXVec3Normalize(&moveto, &m_LookVec);

		Key_Input2(fTimedDelte);
		_float dir = D3DXVec3Dot(&_vec3(0, -1, 0), &moveto);

		if (!lstrcmp(m_pAnimation_Leg->GetAnimationName(), L"Idle"))
			m_pAnimation_Head->SetAnimation(L"Idle");
		else
			m_pAnimation_Head->SetAnimation(L"Walk");
		RayDiskey();

		if (m_bIsMoving)
			Move(fTimedDelte);
		PlayerState(fTimedDelte);
	}
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

void CThirddee::SwapTrigger()
{
	int x = round(m_pTransform->m_vInfo[INFO_POS].x);
	int y = round(m_pTransform->m_vInfo[INFO_POS].y);

	if (x % 2 == 0)
		m_MovetoPos.x = x;
	else if (x > m_pTransform->m_vInfo[INFO_POS].x)
		m_MovetoPos.x = x - 1;
	else
		m_MovetoPos.x = x + 1;

	if (y % 2 == 0)
		m_MovetoPos.y = y;
	else if (y > m_pTransform->m_vInfo[INFO_POS].y)
		m_MovetoPos.y = y - 1;
	else
		m_MovetoPos.y = y + 1;
	if (g_Is2D)
	{
		m_pRigid->m_bUseGrivaty = true;
		m_pCollider->m_bIsTrigger = false;
		m_pCollider->Set_BoundingBox({ 1.f,2.f,0.2f });
		m_pTransform->m_vAngle = _vec3(D3DXToRadian(0), D3DXToRadian(0), 0);
		m_pTransform->m_vInfo[INFO_POS].z = 10;
		m_MovetoPos.z = 10;
		m_fTopOffset = 0;
	}
	else
	{ 
		_int fdir[MD_END] = { 2,1,8,4,6,10,5,9 };
		//플레이어가 이동하려는 방향으로만 검출합니다
		int cnt = 0;
		for (int i = 0; i < MD_END; i++)
		{
			if (m_ThirdDee_Stage == 1)
				if (RayDisKey_part_1((COL_MOVEDIR)i))
					cnt++;
		}
		if (cnt == 0)
			Set_Dead();
		
		m_pRigid->m_bUseGrivaty = false;
		m_pTransform->m_vInfo[INFO_POS].x =
			((int)m_pTransform->m_vInfo[INFO_POS].x % 2 == 0) ? ((int)m_pTransform->m_vInfo[INFO_POS].x) : ((int)m_pTransform->m_vInfo[INFO_POS].x + 1);
		m_pTransform->m_vInfo[INFO_POS].y =
			((int)m_pTransform->m_vInfo[INFO_POS].y % 2 == 0) ? ((int)m_pTransform->m_vInfo[INFO_POS].y) : ((int)m_pTransform->m_vInfo[INFO_POS].y + 1);
		m_pCollider->Set_BoundingBox({ 0.999f,1.999f,1.3f });
		m_pCollider->m_bIsTrigger = true;
		m_pTransform->m_vAngle = _vec3(D3DXToRadian(-90), D3DXToRadian(-90), D3DXToRadian(0));
		m_pTransform->m_vInfo[INFO_POS].z = 11;
		m_MovetoPos.z = 11;
		m_fTopOffset = 1;
	}
	m_bIsMoving = true;

	m_partVec[1]->m_pTransform->m_vInfo[INFO_POS] = _vec3(0, 0+ m_fTopOffset, +0.02f);

	m_partVec[0]->m_pTransform->m_vInfo[INFO_POS] = _vec3(0, 1 + m_fTopOffset, -0.01f);
	m_partVec[0]->m_pTransform->m_vInfo[INFO_POS] = _vec3(0, 1 + m_fTopOffset, -0.01f);

	m_pAnimation_Head->DynamicChangeAnimation(L"Idle", 0, 0, VECTYPE_TRANS, _vec3(0, 1 + m_fTopOffset, -0.01f));
	m_pAnimation_Head->DynamicChangeAnimation(L"Idle", 0, 1, VECTYPE_TRANS, _vec3(0, 1 + m_fTopOffset, -0.01f));
	m_pAnimation_Head->DynamicChangeAnimation(L"Walk", 0, 0, VECTYPE_TRANS, _vec3(0.5f, 1 + m_fTopOffset, -0.01f));
	m_pAnimation_Head->DynamicChangeAnimation(L"Walk", 0, 1, VECTYPE_TRANS, _vec3(-0.5f, 1 + m_fTopOffset, -0.01f));
	
	//m_MovetoPos = m_pTransform->m_vInfo[INFO_POS];
	m_byPlayerInputDir = 0;
	m_pRigid->m_Velocity = _vec3(0, 0, 0);
	m_pTransform->m_vScale = _vec3(1, 1, 1);
	SetRenderONOFF(true);
}


void CThirddee::Key_Input2(const _float & fTimeDelta)
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
		m_byPlayerInputDir |= 2;

	if (Engine::Get_DIKeyState(DIK_RIGHT) == Engine::KEYDOWN)
		m_byPlayerInputDir |= 1;

	if (Engine::Get_DIKeyState(DIK_UP) == Engine::KEYDOWN)
		m_byPlayerInputDir |= 4;

	if (Engine::Get_DIKeyState(DIK_DOWN) == Engine::KEYDOWN)
		m_byPlayerInputDir |= 8;

	if (Engine::Get_DIKeyState(DIK_LEFT) == Engine::KEYUP)
	{
		StopSound(SOUND_EFFECT);
		m_byPlayerInputDir &= 13;
	}
	if (Engine::Get_DIKeyState(DIK_RIGHT) == Engine::KEYUP)
	{
		StopSound(SOUND_EFFECT);
		m_byPlayerInputDir &= 14;
	}
	if (Engine::Get_DIKeyState(DIK_UP) == Engine::KEYUP)
	{
		StopSound(SOUND_EFFECT);
		m_byPlayerInputDir &= 11;
	}
	if (Engine::Get_DIKeyState(DIK_DOWN) == Engine::KEYUP)
	{
		StopSound(SOUND_EFFECT);
		m_byPlayerInputDir &= 7;
	}
	if (Engine::Get_DIKeyState(DIK_Z) == Engine::KEYDOWN&&!m_bIsMoving)
		m_eState = TD_FINDING;

	if (Engine::Get_DIKeyState(DIK_Z) == Engine::KEYUP&&m_eState != TD_SOMETHING)
		m_eState = TD_MOVE;

	if (m_byPlayerInputDir != 0)
		m_byLookDir = m_byPlayerInputDir;
}

void CThirddee::OnCollisionEnter(const Collision * collision)
{
	//������ũ�� �浹�� ���� ���� Ʈ��
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

	if (!lstrcmp(collision->otherObj->m_pTag, L"SemiColon"))
	{
		m_EndingTrigger = true;
		m_pAnimation_Leg->SetAnimation(L"Idle");
		m_pAnimation_Arm->SetAnimation(L"Idle");
		m_pAnimation_Head->SetAnimation(L"Idle");
		dynamic_cast<CThirdCamera*>(Engine::Get_GameObject(L"Layer_Environment", L"Camera"))->Start_Camera_Shake(20.f, 20.0f, SHAKE_ALL);
	}
	m_bStage01_Col = true;
}

void CThirddee::RayDiskey()
{
	_int fdir[MD_END] = { 2,1,8,4,6,10,5,9 };
	//플레이어가 이동하려는 방향으로만 검출합니다
	int cnt=0;
	for (int i = 0; i < MD_END; i++)
	{
		if (m_byPlayerInputDir == fdir[i])
		{
			if (m_ThirdDee_Stage == 1)
				if (RayDisKey_part_1((COL_MOVEDIR)i))
					cnt++;

			RayDisKey_part((COL_MOVEDIR)i);
		}
	}
}

void CThirddee::RayDisKey_part(COL_MOVEDIR dir)
{
	//벽이 있으면 움직일수 없고, 
	_vec3 vdir[MD_END] = { { 0,1,0 },{ 0,-1,0 },{ -1,0,0 },{ 1,0,0 },{ 1,1,0 },{ -1,1,0 },{ 1,-1,0 },{ -1,-1,0 } };
	_int fdir[MD_END] = { 13,14,7,11,9,5,10,6 };
	vector<RayCollision> _detectedCOL = Engine::Check_Collision_Ray(RAYCAST(m_pTransform->m_vInfo[INFO_POS] - _vec3(0, 0, 0.2f), vdir[dir], 2.125f), m_pCollider);
	for (int i = 0; i < _detectedCOL.size(); i++)
	{
		if (!lstrcmp(_detectedCOL[i].tag, L"MapCube") ||
			!lstrcmp(_detectedCOL[i].tag, L"InstallGrid") ||
			!lstrcmp(_detectedCOL[i].tag, L"CrackCube") ||
			!lstrcmp(_detectedCOL[i].tag, L"InstallCube") ||
			!lstrcmp(_detectedCOL[i].tag, L"SwitchCube") ||
			!lstrcmp(_detectedCOL[i].tag, L"Boss3") ||
			!lstrcmp(_detectedCOL[i].tag, L"Boss3Left") ||
			!lstrcmp(_detectedCOL[i].tag, L"Boss3Right") ||
			!lstrcmp(_detectedCOL[i].tag, L"KeyCube")||
			!lstrcmp(_detectedCOL[i].tag, L"GiantHand")
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

_bool CThirddee::RayDisKey_part_1(COL_MOVEDIR dir)
{
	//뭐가 안걸리면 해당방향 꺼줘야함.
	_vec3 vdir[MD_END] = { { 0,1,0 },{ 0,-1,0 },{ -1,0,0 },{ 1,0,0 },{ 1,1,0 },{ -1,1,0 },{ 1,-1,0 },{ -1,-1,0 } };
	_int fdir[MD_END] = { 13,14,7,11,9,5,10,6 };
	vector<RayCollision> _detectedCOL = Engine::Check_Collision_Ray(RAYCAST(m_pTransform->m_vInfo[INFO_POS] - _vec3(-vdir[dir].x*2.f,-vdir[dir].y*2.0f, -1.f), vdir[dir], 2.125f), m_pCollider);
	if (_detectedCOL.size() == 0)
	{
		m_byPlayerInputDir &= fdir[dir];
		return false;
	}
	return true;
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
	m_bStage01_Col = false;
}

void CThirddee::Set_Die()
{
	if (m_DiePart != nullptr)
		return;
	function<void(CTransform*)> func = [&](CTransform* parent) -> void {
		for (int i = 0; i < parent->GetChildCount(); i++)
		{
			CTopdeeParts* parts = dynamic_cast<CTopdeeParts*>(parent->GetChild(i)->m_pGameObject);
			if(parts!=nullptr)
				parts->SetRenderState(false);
			func(parent->GetChild(i));
		}
	};
	func(m_pTransform);
	CLayer* pStageLayer = dynamic_cast<CLayer*>(Engine::Get_Layer(L"Layer_GameLogic"));
	if (pStageLayer != nullptr)
	{
		StopSound(SOUND_EFFECT);
		PlaySound_Effect(L"9.wav", SOUND_EFFECT, 1.f);
		FACTORY<CTopdeeParts>::Create(L"Third_Die", pStageLayer, _vec3(0, 0, 0), m_pTransform, L"Third_Die", 0, true);
		CGameObject* die = Engine::Get_GameObject(L"Layer_GameLogic", L"Third_Die");
		dynamic_cast<CTopdeeParts*>(die)->MakeAnim(L"Die", 0, 3, 0.2f, false);
		dynamic_cast<CTopdeeParts*>(die)->SetAnim(L"Die");
		m_DiePart = dynamic_cast<CTopdeeParts*>(die);

		m_pRigid->m_bUseGrivaty = false;
		m_pRigid->m_Velocity = _vec3(0, 0, 0);
	}
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
		//�����Ҹ� 59��,�������� 58��

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

CThirddee * CThirddee::Create(LPDIRECT3DDEVICE9 pGraphicDev, _vec3 & vPos,_int stage)
{
	CThirddee*		pInstance = new CThirddee(pGraphicDev);

	if (FAILED(pInstance->Ready_GameObject(vPos, stage)))
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
