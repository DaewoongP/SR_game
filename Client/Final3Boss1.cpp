#include "stdafx.h"
#include "Final3Boss1.h"
#include "AbstractFactory.h"
#include "Export_Function.h"
#include "TopdeeJoint.h"
#include "Boss1Hand.h"
#include "GiantHand.h"
#include "Boss1Head.h"
#include <functional>

#include"FinalCube.h"


#define	BOSS1SCALE 2.f
#define SCALEADD for (int i = 0; i < clip->source.size(); i++)\
for (int j = 0; j < clip->source[i].size(); j++)\
{\
	clip->source[i][j].scale *= BOSS1SCALE;\
	clip->source[i][j].trans *= 4.f;\
}\

CFinal3Boss1::CFinal3Boss1(LPDIRECT3DDEVICE9 pGraphicDev)
	: CGameObject(pGraphicDev), m_bThrow(true)
{
	m_bInit = true;
	m_dwRestTime = 1;
	m_dwThrowCubeTime = 0.f;
}

CFinal3Boss1::~CFinal3Boss1()
{
}

HRESULT CFinal3Boss1::Ready_GameObject(_vec3 & vPos)
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);
	//위치잡는 친구를 넣어주세요
	m_pTransform->m_vInfo[INFO_POS] = vPos;
	m_vOriginPos = vPos;
	m_bTurn_x=false;
	m_bTurn_y=false;
	m_fOffset_x = 0;
	m_fOffset_y = 0;
	m_iHp = 100.f;
	m_pCollider->Set_BoundingBox({100.f,100.f,50.f});

	return S_OK;
}

_int CFinal3Boss1::Update_GameObject(const _float & fTimeDelta)
{
	if (m_bDead)
		return OBJ_DEAD;

	if (m_bInit)
	{
		//삼디를 가지고있어야함.
		CComponent* otherTrans = Engine::Get_Component(L"Layer_GameLogic", L"Thirddee", L"Transform", ID_DYNAMIC);
		if (otherTrans != nullptr)
			m_Player = dynamic_cast<CTransform*>(otherTrans);

		//실행해주는 코드
		CLayer* pStageLayer = dynamic_cast<CLayer*>(Engine::Get_Layer(L"Layer_GameLogic"));
		NULL_CHECK_RETURN(pStageLayer, E_FAIL);

		//몸똥 0
		FAILED_CHECK_RETURN(FACTORY<CTopdeeJoint>::Create(L"Toodo_Body", pStageLayer, _vec3(0,0,0), m_pTransform), E_FAIL);
		//머리통 0-0
		FAILED_CHECK_RETURN(FACTORY<CBoss1Parts>::Create(L"Toodo_Head", pStageLayer, _vec3(0, 0, 0), m_pTransform->GetChild(0), L"Boss1_Parts", 6, false), E_FAIL);
		//눈섭 0-0-0
		FAILED_CHECK_RETURN(FACTORY<CBoss1Parts>::Create(L"Toodo_EyeBrow", pStageLayer, _vec3(0, 0, 0), m_pTransform->GetChild(0)->GetChild(0), L"Boss1_Parts", 8, false), E_FAIL);
		//수염 0-0-1~2
		FAILED_CHECK_RETURN(FACTORY<CBoss1Parts>::Create(L"Toodo_Beard", pStageLayer, _vec3(0, 0, 0), m_pTransform->GetChild(0)->GetChild(0), L"Boss1_Parts", 9, false), E_FAIL);
		FAILED_CHECK_RETURN(FACTORY<CBoss1Parts>::Create(L"Toodo_Beard", pStageLayer, _vec3(0, 0, 0), m_pTransform->GetChild(0)->GetChild(0), L"Boss1_Parts", 9, false), E_FAIL);
		//입 0 0 3
		FAILED_CHECK_RETURN(FACTORY<CBoss1Parts>::Create(L"Toodo_Mouse", pStageLayer, _vec3(0, 0, 0), m_pTransform->GetChild(0)->GetChild(0), L"Boss1_Parts", 4, false), E_FAIL);
		//코 0-0-4
		FAILED_CHECK_RETURN(FACTORY<CBoss1Parts>::Create(L"Toodo_Nose", pStageLayer, _vec3(0, 0, 0), m_pTransform->GetChild(0)->GetChild(0), L"Boss1_Parts", 10, false), E_FAIL);
		//눈 0 0 5
		FAILED_CHECK_RETURN(FACTORY<CBoss1Parts>::Create(L"Toodo_Eye", pStageLayer, _vec3(0, 0, 0), m_pTransform->GetChild(0)->GetChild(0), L"Boss1_Parts", 7, false), E_FAIL);
		//눈화장 0 0 6
		FAILED_CHECK_RETURN(FACTORY<CBoss1Parts>::Create(L"Toodo_MakeUp", pStageLayer, _vec3(0, 0, 0), m_pTransform->GetChild(0)->GetChild(0), L"Boss2_MakeUp", 8, true), E_FAIL);

		//잘 만들어진 탑디의 조인트를 훔쳐씁니다.
		//어 0 1~2
		FAILED_CHECK_RETURN(FACTORY<CTopdeeJoint>::Create(L"Toodo_Shoulder", pStageLayer, _vec3(0, 0, 0), m_pTransform->GetChild(0)), E_FAIL);
		FAILED_CHECK_RETURN(FACTORY<CTopdeeJoint>::Create(L"Toodo_Shoulder", pStageLayer, _vec3(0, 0, 0), m_pTransform->GetChild(0)), E_FAIL);
		
		//어깨의 자식 삼두																										
		FAILED_CHECK_RETURN(FACTORY<CBoss1Parts>::Create(L"Toodo_Samdoo", pStageLayer, _vec3(0, 0, 0), m_pTransform->GetChild(0)->GetChild(1), L"Boss1_Parts", 1, false), E_FAIL);
		FAILED_CHECK_RETURN(FACTORY<CBoss1Parts>::Create(L"Toodo_Samdoo", pStageLayer, _vec3(0, 0, 0), m_pTransform->GetChild(0)->GetChild(2), L"Boss1_Parts", 1, false), E_FAIL);
		
		//삼두의 자식 팔
		FAILED_CHECK_RETURN(FACTORY<CTopdeeJoint>::Create(L"Toodo_Arm", pStageLayer, _vec3(0, 0, 0), m_pTransform->GetChild(0)->GetChild(1)->GetChild(0)), E_FAIL);
		FAILED_CHECK_RETURN(FACTORY<CTopdeeJoint>::Create(L"Toodo_Arm", pStageLayer, _vec3(0, 0, 0), m_pTransform->GetChild(0)->GetChild(2)->GetChild(0)), E_FAIL);
		
		//팔의 자식 완근이								
		FAILED_CHECK_RETURN(FACTORY<CBoss1Parts>::Create(L"Toodo_Wan", pStageLayer, _vec3(0, 0, 0), m_pTransform->GetChild(0)->GetChild(1)->GetChild(0)->GetChild(0), L"Boss1_Parts", 2, false), E_FAIL);
		FAILED_CHECK_RETURN(FACTORY<CBoss1Parts>::Create(L"Toodo_Wan", pStageLayer, _vec3(0, 0, 0), m_pTransform->GetChild(0)->GetChild(2)->GetChild(0)->GetChild(0), L"Boss1_Parts", 2, false), E_FAIL);
		
		//완근이의 자식 손목
		FAILED_CHECK_RETURN(FACTORY<CTopdeeJoint>::Create(L"Toodo_HandNeck", pStageLayer, _vec3(0, 0, 0), m_pTransform->GetChild(0)->GetChild(1)->GetChild(0)->GetChild(0)->GetChild(0)), E_FAIL);
		FAILED_CHECK_RETURN(FACTORY<CTopdeeJoint>::Create(L"Toodo_HandNeck", pStageLayer, _vec3(0, 0, 0), m_pTransform->GetChild(0)->GetChild(2)->GetChild(0)->GetChild(0)->GetChild(0)), E_FAIL);
		
		//손목이의 자식 손																									
		FAILED_CHECK_RETURN(FACTORY<CBoss1Parts>::Create(L"Toodo_Hand", pStageLayer, _vec3(0, 0, 0), m_pTransform->GetChild(0)->GetChild(1)->GetChild(0)->GetChild(0)->GetChild(0)->GetChild(0), L"Boss1_Parts", 3, false), E_FAIL);
		FAILED_CHECK_RETURN(FACTORY<CBoss1Parts>::Create(L"Toodo_Hand", pStageLayer, _vec3(0, 0, 0), m_pTransform->GetChild(0)->GetChild(2)->GetChild(0)->GetChild(0)->GetChild(0)->GetChild(0), L"Boss1_Parts", 3, false), E_FAIL);

		FAILED_CHECK_RETURN(FACTORY<CBoss1Parts>::Create(L"Toodo_Body", pStageLayer, _vec3(0, 0, 0), m_pTransform->GetChild(0), L"Boss1_Parts", 0, false), E_FAIL);
		m_PartsVec.push_back(m_pTransform->GetChild(0));
		
		//재귀람다람다
		function<void(CTransform*, vector<CTransform*>&)> func = [&](CTransform* parent,vector<CTransform*>& vec) -> void{
			for (int i = 0; i < parent->GetChildCount(); i++)
			{
				vec.push_back(parent->GetChild(i));
				func(parent->GetChild(i), vec);
			}
		};
		func(m_pTransform->GetChild(0), m_PartsVec);

		//몸
		m_PartsVec[0]->Set_SRT(_vec3(12,12,12), _vec3(0,0,0), _vec3(0,-3,0));
		//머리
		m_PartsVec[1]->Set_SRT(_vec3(1.f,1.f,1.f), _vec3(0,0,0), _vec3(0,12,-0.1f));
		//눈섭
		m_PartsVec[2]->Set_SRT(_vec3(13,13,13), _vec3(0,0,0), _vec3(0,3,-0.2f));
		//수염
		m_PartsVec[3]->Set_SRT(_vec3(13, 13, 13), _vec3(0, 0, 0), _vec3(1.3f, -1.0f, -0.2f));
		m_PartsVec[4]->Set_SRT(_vec3(13, 13, 13), _vec3(0, D3DXToRadian(180), 0), _vec3(-2.3f, -1.0f, -0.2f));
		//입
		m_PartsVec[5]->Set_SRT(_vec3(10, 10, 10), _vec3(0, D3DXToRadian(180), 0), _vec3(-0.5f, -1.2f, -0.1f));
		//코
		m_PartsVec[6]->Set_SRT(_vec3(13,13,13), _vec3(0, 0, 0), _vec3(-1, 0.5f, -0.3f));
		//눈
		m_PartsVec[7]->Set_SRT(_vec3(11, 11, 11), _vec3(0, 0, 0), _vec3(-0.2f, 1, -0.2f));
		//눈화장
		m_PartsVec[8]->Set_SRT(_vec3(11, 11, 11), _vec3(0,0,0), _vec3(-0.1f,1.4f,-0.1f));

		dynamic_cast<CBoss1Parts*>(m_PartsVec[8]->m_pGameObject)->MakeAnim(L"Attack",0,8,0.6f,true);
		dynamic_cast<CBoss1Parts*>(m_PartsVec[8]->m_pGameObject)->MakeAnim(L"Idle", 0, 0, 1, false);
		dynamic_cast<CBoss1Parts*>(m_PartsVec[8]->m_pGameObject)->SetAnim(L"Idle");
		//오른
		//어깨조인트
		m_PartsVec[9]->Set_SRT(_vec3(13,13,13), _vec3(0,D3DXToRadian(0), D3DXToRadian(-30)), _vec3(-9,6,0.1f));
		//삼두
		m_PartsVec[10]->Set_SRT(_vec3(1,1,1), _vec3(0,0,0), _vec3(0,-3,0));
		//꿈치
		m_PartsVec[11]->Set_SRT(_vec3(13, 13, 13), _vec3(0, 0, D3DXToRadian(120)), _vec3(0, -4, -0.1f));
		//완근이
		m_PartsVec[12]->Set_SRT(_vec3(1,1,1), _vec3(0,0,0), _vec3(0,-2,0));
		//손목이
		m_PartsVec[13]->Set_SRT(_vec3(13, 13, 13), _vec3(0,0, D3DXToRadian(0)), _vec3(0,-3, -0.1f));
		//손
		m_PartsVec[14]->Set_SRT(_vec3(1,1,1), _vec3(0,0,0), _vec3(0,-1,0));

		//왼
		//어깨조인트
		m_PartsVec[15]->Set_SRT(_vec3(-13, 13, 13), _vec3(0, D3DXToRadian(0), D3DXToRadian(30)), _vec3(9, 6, 0.1f));
		//삼두
		m_PartsVec[16]->Set_SRT(_vec3(1, 1, 1), _vec3(0, 0, 0), _vec3(0, -3, 0));
		//꿈치
		m_PartsVec[17]->Set_SRT(_vec3(-13, 13, 13), _vec3(0, 0, D3DXToRadian(-120)), _vec3(0, -4, -0.1f));
		//완근이
		m_PartsVec[18]->Set_SRT(_vec3(1, 1, 1), _vec3(0, 0, 0), _vec3(0, -2, 0));
		//손목이
		m_PartsVec[19]->Set_SRT(_vec3(13, 13, 13), _vec3(0, 0, D3DXToRadian(0)), _vec3(0, -3, -0.1f));
		//손
		m_PartsVec[20]->Set_SRT(_vec3(-1, 1, 1), _vec3(0, 0, 0), _vec3(0, -1, 0));

		m_bInit = false;

		for (int i = 0; i < m_PartsVec.size(); i++)
		{
			if (i == 0)
				continue;
			m_PartsVec[i]->m_vScale *= BOSS1SCALE;
			m_PartsVec[i]->m_vInfo[INFO_POS] *= BOSS1SCALE;
		}

		//그냥 둥둥이임.
		AnimClip* clip = nullptr;
		clip = new AnimClip();
		{
			clip->parts.push_back(m_PartsVec[0]); //몸통
			clip->parts.push_back(m_PartsVec[1]); //머리통
			clip->parts.push_back(m_PartsVec[9]);//어깨도 동동
			clip->parts.push_back(m_PartsVec[11]);//팔도 동동
			clip->parts.push_back(m_PartsVec[15]);//어깨도 동동
			clip->parts.push_back(m_PartsVec[17]);//팔도 동동
			clip->parts.push_back(m_PartsVec[13]);//손목이
			clip->source.resize(7);

			{
				clip->source[0].push_back(
					ANIMINFO{
					_vec3(0,-3,0) + _vec3(0,1,0),
					_vec3(D3DXToRadian(0.1f),0,0),//rotation
					_vec3(13,13,13),//scale
					1.5f,//tilltime
					0.f//actionTime
				});

				clip->source[0].push_back(
					ANIMINFO{
					_vec3(0,-3,0) + _vec3(0,-1,0),
					_vec3(D3DXToRadian(0.1f),0,0),//rotation
					_vec3(13,13,13),
					1.5f,//tilltime
					1.5f//actionTime
				});
			}
			{
				clip->source[1].push_back(
					ANIMINFO{
					_vec3(0,12,-0.1f) + _vec3(0,1,0),
					_vec3(D3DXToRadian(0.1f),0,0),//rotation
					_vec3(1,1,1),
					1.5f,//tilltime
					0.f//actionTime
				});
				clip->source[1].push_back(
					ANIMINFO{
					_vec3(0,12,-0.1f) + _vec3(0,-2,0),
					_vec3(D3DXToRadian(0.1f),0,0),//rotation
					_vec3(1,1,1),
					1.5f,//tilltime
					1.5f//actionTime
				});
			}
			{
				clip->source[2].push_back(
					ANIMINFO{
					_vec3(-9, 6, 0.1f) + _vec3(0,0.5f,0),
					_vec3(D3DXToRadian(0.1f),0,D3DXToRadian(-30)),//rotation
					_vec3(13,13,13),
					1.5f,//tilltime
					0.f//actionTime
				});
				clip->source[2].push_back(
					ANIMINFO{
					_vec3(-9, 6, 0.1f) + _vec3(0,-2,0),
					_vec3(D3DXToRadian(0.1f),0,D3DXToRadian(-30)),//rotation
					_vec3(13,13,13),
					1.5f,//tilltime
					1.5f//actionTime
				});
			}
			{
				clip->source[3].push_back(
					ANIMINFO{
					_vec3(0, -4, -0.1f) + _vec3(0,0.5f,0),
					_vec3(D3DXToRadian(0.1f),0,D3DXToRadian(120)),//rotation
					_vec3(13,13,13),
					1.5f,//tilltime
					0.f//actionTime
				});
				clip->source[3].push_back(
					ANIMINFO{
					_vec3(0, -4, -0.1f) + _vec3(0,-2,0),
					_vec3(D3DXToRadian(0.1f),0,D3DXToRadian(120)),//rotation
					_vec3(13,13,13),
					1.5f,//tilltime
					1.5f//actionTime
				});
			}
			{
				clip->source[4].push_back(
					ANIMINFO{
					_vec3(9, 6, 0.1f) + _vec3(0,0.5f,0),
					_vec3(D3DXToRadian(0.1f),0,D3DXToRadian(30)),//rotation
					_vec3(-13,13,13),
					1.5f,//tilltime
					0.f//actionTime
				});
				clip->source[4].push_back(
					ANIMINFO{
					_vec3(9, 6, 0.1f) + _vec3(0,-2,0),
					_vec3(D3DXToRadian(0.1f),0,D3DXToRadian(30)),//rotation
					_vec3(-13,13,13),
					1.5f,//tilltime
					1.5f//actionTime
				});
			}
			{
				clip->source[5].push_back(
					ANIMINFO{
					_vec3(0, -4, -0.1f) + _vec3(0,0.5f,0),
					_vec3(D3DXToRadian(0.1f),0,D3DXToRadian(-120)),//rotation
					_vec3(-13,13,13),
					1.5f,//tilltime
					0.f//actionTime
				});
				clip->source[5].push_back(
					ANIMINFO{
					_vec3(0, -4, -0.1f) + _vec3(0,-2,0),
					_vec3(D3DXToRadian(0.1f),0,D3DXToRadian(-120)),//rotation
					_vec3(-13,13,13),
					1.5f,//tilltime
					1.5f//actionTime
				});
			}
			{
				clip->source[6].push_back(
					ANIMINFO{
					_vec3(0, -4, -0.1f) + _vec3(0,0,0),
					_vec3(D3DXToRadian(0.1f),0,0),//rotation
					_vec3(11,11,11),
					1.5f,//tilltime
					0.0f//actionTime
				});
				clip->source[6].push_back(
					ANIMINFO{
					_vec3(0, -4, -0.1f) + _vec3(0,0,0),
					_vec3(D3DXToRadian(0.1f),0,0),//rotation
					_vec3(11,11,11),
					1.5f,//tilltime
					1.5f//actionTime
				});
			}
			SCALEADD;
			clip->TotalTime = 3.f;
			clip->Useloop = true;
		}
		m_pAnimation_Whole->AddClip(L"Idle", clip);
			
		//손가락 공격임
		clip = new AnimClip();
		{ PlaySound_Effect(L"34", SOUND_EFFECT_ENEMY, 1.f);
			clip->parts.push_back(m_PartsVec[0]); //몸통
			clip->parts.push_back(m_PartsVec[1]); //머리통
			clip->parts.push_back(m_PartsVec[9]);//어깨도 동동
			clip->parts.push_back(m_PartsVec[11]);//팔도 동동
			clip->parts.push_back(m_PartsVec[15]);//어깨도 동동
			clip->parts.push_back(m_PartsVec[17]);//팔도 동동
			//손목
			clip->parts.push_back(m_PartsVec[13]);//손목이
			clip->source.resize(7);

			LerpClipAdd(clip, 0, 0.2f, _vec3(12,12,12), _vec3(2,2,2), _vec3(8, -8, 0), _vec3(0, 2, 0), _vec3(0, 0, 0), _vec3(0, 0, 0), 18);
			LerpClipAdd(clip, 1, 0.2f, _vec3(1,1,1), _vec3(0.3f, 0.3f, 0.3f), _vec3(0, 12, -0.1f), _vec3(0, 2, 0), _vec3(0, 0, 0), _vec3(0, 0, D3DXToRadian(-50)), 18);

			//들어올릴 어깨/팔 세팅
			{
				clip->source[2].push_back(
					ANIMINFO{
					_vec3(-9, 6, 0.1f) + _vec3(0,1,0),
					_vec3(D3DXToRadian(180),0,0),//rotation
					_vec3(0,0,0),
					0.1f,//tilltime
					0.f//actionTime
				});
				clip->source[2].push_back(
					ANIMINFO{
					_vec3(-9, 6, 0.1f) + _vec3(0,-2,0),
					_vec3(D3DXToRadian(180),0,0),//rotation
					_vec3(0,0,0),
					0.1f,//tilltime
					0.1f//actionTime
				});
			}
			{
				clip->source[3].push_back(
					ANIMINFO{
					_vec3(0, -4, -0.1f) + _vec3(0,1,0),
					_vec3(D3DXToRadian(20),0,0),//rotation
					_vec3(0,0,0),
					0.1f,//tilltime
					0.f//actionTime
				});
				clip->source[3].push_back(
					ANIMINFO{
					_vec3(0, -4, -0.1f) + _vec3(0,-2,0),
					_vec3(D3DXToRadian(20),0,0),//rotation
					_vec3(0,0,0),
					0.1f,//tilltime
					0.1f//actionTime
				});
			}

			LerpClipAdd(clip, 6, 0.2f, _vec3(13, 13, 13), _vec3(4,4,4), _vec3(0, -5, -0.1f), _vec3(0, 2, 0), _vec3(0, 0, D3DXToRadian(0)), _vec3(0, 0, 0), 18);
			LerpClipAdd(clip, 4, 0.2f, _vec3(-13, 13, 13), _vec3(-4,4,4), _vec3(9, 6, 0.1f), _vec3(0, 2, 0), _vec3(0, D3DXToRadian(0), D3DXToRadian(30)), _vec3(0, 0, 0), 18);
			LerpClipAdd(clip, 5, 0.2f, _vec3(-13, 13, 13), _vec3(-4,4,4), _vec3(0, -4, -0.1f), _vec3(0, 2, 0), _vec3(0, 0, D3DXToRadian(-120)), _vec3(0, 0, 0), 18);
			SCALEADD;
			clip->TotalTime = 0.2f*18;
			clip->Useloop = false;
		}
		m_pAnimation_Whole->AddClip(L"Finger_Ready", clip);

		//손가락 공격임
		clip = new AnimClip();
		{
			clip->parts.push_back(m_PartsVec[0]); //몸통
			clip->parts.push_back(m_PartsVec[1]); //머리통
			clip->parts.push_back(m_PartsVec[9]);//어깨도 동동
			clip->parts.push_back(m_PartsVec[11]);//팔도 동동
			clip->parts.push_back(m_PartsVec[15]);//어깨도 동동
			clip->parts.push_back(m_PartsVec[17]);//팔도 동동
			clip->parts.push_back(m_PartsVec[13]);//손목이
			clip->source.resize(7);

			LerpClipAdd(clip, 0, 0.2f, _vec3(12,12,12), _vec3(0.4f, 0.4f, 0.4f), _vec3(8, -8, 0), _vec3(0, 1, 0), _vec3(0, 0, 0), _vec3(0, 0, 0), 2);
			LerpClipAdd(clip, 1, 0.2f, _vec3(1,1,1), _vec3(0.1f, 0.1f, 0.1f), _vec3(0, 12, -0.1f), _vec3(0, 1, 0), _vec3(0, 0, 0), _vec3(0, 0, 0), 2);
			{
				clip->source[2].push_back(
					ANIMINFO{
					_vec3(-9, 6, 0.1f) + _vec3(0,3,0),
					_vec3(D3DXToRadian(180),0,D3DXToRadian(-40)),//rotation
					_vec3(0,0,0),
					0.3f,//tilltime
					0.f//actionTime
				});
				clip->source[2].push_back(
					ANIMINFO{
					_vec3(-9, 6, 0.1f) + _vec3(0,-3,0),
					_vec3(D3DXToRadian(180),0,D3DXToRadian(-40)),//rotation
					_vec3(0,0,0),
					0.1f,//tilltime
					0.3f//actionTime
				});
			}
			{
				clip->source[3].push_back(
					ANIMINFO{
					_vec3(0, -4, -0.1f) + _vec3(0,0,0),
					_vec3(D3DXToRadian(0),0,D3DXToRadian(40)),//rotation
					_vec3(0,0,0),
					0.3f,//tilltime
					0.f//actionTime
				});
				clip->source[3].push_back(
					ANIMINFO{
					_vec3(0, -4, -0.1f) + _vec3(0,0,0),
					_vec3(D3DXToRadian(0),0,D3DXToRadian(40)),//rotation
					_vec3(0,0,0),
					0.1f,//tilltime
					0.3f//actionTime
				});
			}
			{
				clip->source[6].push_back(
					ANIMINFO{
					_vec3(0, -4, -0.1f) + _vec3(0,0,0),
					_vec3(D3DXToRadian(20),0,0),//rotation
					_vec3(18,18,18),
					0.3f,//tilltime
					0.f//actionTime
				});
				clip->source[6].push_back(
					ANIMINFO{
					_vec3(0, -4, -0.1f) + _vec3(0,0,0),
					_vec3(D3DXToRadian(20),0,0),//rotation
					_vec3(11,11,11),
					0.1f,//tilltime
					0.3f//actionTime
				});
			}
			LerpClipAdd(clip, 4, 0.2f, _vec3(-13, 13, 13), _vec3(-0.4f, 0.4f, 0.4f), _vec3(9, 6, 0.1f), _vec3(0, 1, 0), _vec3(0, D3DXToRadian(0), D3DXToRadian(30)), _vec3(0, 0, 0), 2);
			LerpClipAdd(clip, 5, 0.2f, _vec3(-13, 13, 13), _vec3(-0.4f, 0.4f, 0.4f), _vec3(0, -4, -0.1f), _vec3(0, 1, 0), _vec3(0, 0, D3DXToRadian(-120)), _vec3(0, 0, 0), 2);
			SCALEADD;
			clip->TotalTime = 0.2f * 2;
			clip->Useloop = true;
		}
		m_pAnimation_Whole->AddClip(L"Finger_Shoot", clip);

		//발공격임
		clip = new AnimClip();
		{
			clip->parts.push_back(m_PartsVec[0]); //몸통
			clip->parts.push_back(m_PartsVec[1]); //머리통
			clip->parts.push_back(m_PartsVec[9]);//어깨도 동동
			clip->parts.push_back(m_PartsVec[11]);//팔도 동동
			clip->parts.push_back(m_PartsVec[15]);//어깨도 동동
			clip->parts.push_back(m_PartsVec[17]);//팔도 동동
			clip->source.resize(6);
										//스케	//이동								//회전
			LerpClipAdd(clip, 0, 0.2f, _vec3(12,12,12), _vec3(2,2,2), _vec3(0, -3, 0), _vec3(0, 2, 0), _vec3(0, 0, 0), _vec3(0, 0, 0), 18);
			LerpClipAdd(clip, 1, 0.2f, _vec3(1, 1, 1), _vec3(0.3f, 0.3f, 0.3f), _vec3(0, 12, -0.1f), _vec3(0, 2, 0), _vec3(0, 0, 0), _vec3(0, 0, D3DXToRadian(-50)), 18);
			LerpClipAdd(clip, 2, 0.2f, _vec3(13,13,13), _vec3(4,4,4), _vec3(-9, 6, 0.1f), _vec3(0, 2, 0), _vec3(0, D3DXToRadian(0), D3DXToRadian(-30)), _vec3(0, 0, D3DXToRadian(50)), 18);
			LerpClipAdd(clip, 3, 0.2f, _vec3(13,13,13), _vec3(4,4,4), _vec3(0, -4, -0.1f), _vec3(0, 2, 0), _vec3(0, 0, D3DXToRadian(120)), _vec3(0, 0, 0), 18);
			LerpClipAdd(clip, 4, 0.2f, _vec3(-13,13,13), _vec3(-4,4,4), _vec3(9, 6, 0.1f), _vec3(0, 2, 0), _vec3(0, D3DXToRadian(0), D3DXToRadian(30)), _vec3(0, 0, 0), 18);
			LerpClipAdd(clip, 5, 0.2f, _vec3(-13,13,13), _vec3(-4,4,4), _vec3(0, -4, -0.1f), _vec3(0, 2, 0), _vec3(0, 0, D3DXToRadian(-120)), _vec3(0, 0, 0), 18);
			
			SCALEADD;
			clip->TotalTime = 0.2f*18;
			clip->Useloop = false;
		}
		m_pAnimation_Whole->AddClip(L"Foot", clip);

		//발공격임
		clip = new AnimClip();
		{
			clip->parts.push_back(m_PartsVec[0]); //몸통
			clip->parts.push_back(m_PartsVec[1]); //머리통
			clip->parts.push_back(m_PartsVec[9]);//어깨도 동동
			clip->parts.push_back(m_PartsVec[11]);//팔도 동동
			clip->parts.push_back(m_PartsVec[13]);//팔도 동동
			clip->parts.push_back(m_PartsVec[15]);//어깨도 동동
			clip->parts.push_back(m_PartsVec[17]);//팔도 동동
			clip->parts.push_back(m_PartsVec[19]);//팔도 동동
			clip->source.resize(8);
			LerpClipAdd(clip, 0, 0.2f, _vec3(12, 12, 12), _vec3(2, 2, 2), _vec3(8, -8, 0), _vec3(0, 2, 0), _vec3(0, 0, 0), _vec3(0, 0, 0), 18);
			LerpClipAdd(clip, 1, 0.2f, _vec3(1, 1, 1), _vec3(0.3f, 0.3f, 0.3f), _vec3(0, 12, -0.1f), _vec3(0, 2, 0), _vec3(0, 0, 0), _vec3(0, 0, D3DXToRadian(-50)), 18);

			//들어올릴 어깨/팔 세팅
			{
				clip->source[2].push_back(
					ANIMINFO{
					_vec3(-9, 6, 0.1f),
					_vec3(D3DXToRadian(0.1f),D3DXToRadian(0),D3DXToRadian(0)),//rotation
					_vec3(0,0,0),
					.1f,//tilltime
					0.f//actionTime
				});
				clip->source[2].push_back(
					ANIMINFO{
					_vec3(-9, 6, 0.1f) + _vec3(0,0,0),
					_vec3(D3DXToRadian(0),D3DXToRadian(0),D3DXToRadian(-200)),//rotation
					_vec3(0,0,0),
					1.1f,//tilltime
					.1f//actionTime
				});
				clip->source[2].push_back(
					ANIMINFO{
					_vec3(-9, 6, 0.1f) + _vec3(0,-2,0),
					_vec3(D3DXToRadian(90),D3DXToRadian(-360),D3DXToRadian(160)),//rotation
					_vec3(0,0,0),
					0.6f,//tilltime
					1.2f//actionTime
				});
			}
			{
				clip->source[3].push_back(
					ANIMINFO{
					_vec3(0, -4, -0.1f),
					_vec3(D3DXToRadian(0),0,D3DXToRadian(0.1f)),//rotation
					_vec3(0,0,0),
					.2f,//tilltime
					0.f//actionTime
				});
				clip->source[3].push_back(
					ANIMINFO{
					_vec3(0, -4, -0.1f) + _vec3(-0.2f,0,0),
					_vec3(D3DXToRadian(0),0,D3DXToRadian(-170)),//rotation
					_vec3(0,0,0),
					1.f,//tilltime
					.2f//actionTime
				});
				clip->source[3].push_back(
					ANIMINFO{
					_vec3(0, -4, -0.1f) + _vec3(-0.2f,-4,0),
					_vec3(D3DXToRadian(0),D3DXToRadian(0),D3DXToRadian(70)),//rotation
					_vec3(0,0,0),
					0.6f,//tilltime
					1.2f//actionTime
				});
			}
			{
				clip->source[4].push_back(
					ANIMINFO{
					_vec3(0,-3, -0.1f),
					_vec3(D3DXToRadian(0),0,D3DXToRadian(0.1f)),//rotation
					_vec3(0,0,0),
					.2f,//tilltime
					0.f//actionTime
				});
				clip->source[4].push_back(
					ANIMINFO{
					_vec3(0,-3, -0.1f) + _vec3(-4,-2,.2f),
					_vec3(D3DXToRadian(0),0,D3DXToRadian(-120)),//rotation
					_vec3(0,0,0),
					1.f,//tilltime
					.2f//actionTime
				});
				clip->source[4].push_back(
					ANIMINFO{
					_vec3(0,-3, -0.1f) + _vec3(0,-3,0),
					_vec3(D3DXToRadian(0),0,D3DXToRadian(60)),//rotation
					_vec3(0,0,0),
					0.6f,//tilltime
					1.2f//actionTime
				});
			}
			{
				clip->source[5].push_back(
					ANIMINFO{
					_vec3(9, 6, 0.1f),
					_vec3(D3DXToRadian(0.1f),D3DXToRadian(0),D3DXToRadian(0)),//rotation
					_vec3(0,0,0),
					.1f,//tilltime
					0.f//actionTime
				});
				clip->source[5].push_back(
					ANIMINFO{
					_vec3(9, 6, 0.1f) + _vec3(0,0,0),
					_vec3(D3DXToRadian(0),D3DXToRadian(0),D3DXToRadian(200)),//rotation
					_vec3(0,0,0),
					1.1f,//tilltime
					.1f//actionTime
				});
				clip->source[5].push_back(
					ANIMINFO{
					_vec3(9, 6, 0.1f) + _vec3(0,-2,0),
					_vec3(D3DXToRadian(-20),D3DXToRadian(360),D3DXToRadian(-90)),//rotation
					_vec3(0,0,0),
					0.6f,//tilltime
					1.2f//actionTime
				});
			}
			{
				clip->source[6].push_back(
					ANIMINFO{
					_vec3(0, -4, -0.1f),
					_vec3(D3DXToRadian(0),0,D3DXToRadian(0.1f)),//rotation
					_vec3(0,0,0),
					.2f,//tilltime
					0.f//actionTime
				});
				clip->source[6].push_back(
					ANIMINFO{
					_vec3(0, -4, -0.1f) + _vec3(0.2f,0,0),
					_vec3(D3DXToRadian(0),0,D3DXToRadian(170)),//rotation
					_vec3(0,0,0),
					1.f,//tilltime
					.2f//actionTime
				});
				clip->source[6].push_back(
					ANIMINFO{
					_vec3(0, -4, -0.1f) + _vec3(0.2f,-4,0),
					_vec3(D3DXToRadian(0),D3DXToRadian(0),D3DXToRadian(-70)),//rotation
					_vec3(0,0,0),
					0.6f,//tilltime
					1.2f//actionTime
				});
			}
			{
				clip->source[7].push_back(
					ANIMINFO{
					_vec3(0,-3, -0.1f),
					_vec3(D3DXToRadian(0),0,D3DXToRadian(0.1f)),//rotation
					_vec3(0,0,0),
					.2f,//tilltime
					0.f//actionTime
				});
				clip->source[7].push_back(
					ANIMINFO{
					_vec3(0,-3, -0.1f) + _vec3(4,-2,.2f),
					_vec3(D3DXToRadian(0),0,D3DXToRadian(120)),//rotation
					_vec3(0,0,0),
					1.f,//tilltime
					.2f//actionTime
				});
				clip->source[7].push_back(
					ANIMINFO{
					_vec3(0,-3, -0.1f) + _vec3(0,-3,0),
					_vec3(D3DXToRadian(0),0,D3DXToRadian(-60)),//rotation
					_vec3(0,0,0),
					0.6f,//tilltime
					1.2f//actionTime
				});
			}

			SCALEADD;
			clip->TotalTime =1.8f;
			clip->Useloop = true;
		}
		m_pAnimation_Whole->AddClip(L"Throw_L", clip);
		m_pAnimation_Whole->SetAnimation(L"Throw_L");

		//그냥 과묵이
		clip = new AnimClip();
		{
			clip->parts.push_back(m_PartsVec[2]); //눈섭
			clip->parts.push_back(m_PartsVec[3]); //수염
			clip->parts.push_back(m_PartsVec[4]);//수염
			clip->parts.push_back(m_PartsVec[5]);//입
			clip->parts.push_back(m_PartsVec[6]);//코
			clip->parts.push_back(m_PartsVec[7]);//눈
			clip->parts.push_back(m_PartsVec[8]);//눈화장
			clip->source.resize(7);

			{
				clip->source[0].push_back(
					ANIMINFO{
					_vec3(0,3,-0.2f) + _vec3(0,0.3f,0),
					_vec3(D3DXToRadian(0.1f),0,0),//rotation
					_vec3(0,0,0),//scale
					1.8f,//tilltime
					0.f//actionTime
				});

				clip->source[0].push_back(
					ANIMINFO{
					_vec3(0,3,-0.2f) + _vec3(0,-0.3f,0),
					_vec3(D3DXToRadian(0.1f),0,0),//rotation
					_vec3(0,0,0),
					1.2f,//tilltime
					1.8f//actionTime
				});
			}
			{
				clip->source[1].push_back(
					ANIMINFO{
					_vec3(1.3f, -1.0f, -0.2f) + _vec3(0,0.3f,0),
					_vec3(D3DXToRadian(0.1f),0,0),//rotation
					_vec3(0,0,0),
					1.8f,//tilltime
					0.f//actionTime
				});
				clip->source[1].push_back(
					ANIMINFO{
					_vec3(1.3f, -1.0f, -0.2f) + _vec3(0,-0.3f,0),
					_vec3(D3DXToRadian(0.1f),0,0),//rotation
					_vec3(0,0,0),
					1.2f,//tilltime
					1.8f//actionTime
				});
			}
			{
				clip->source[2].push_back(
					ANIMINFO{
					_vec3(-2.3f, -1.0f, -0.2f) + _vec3(0,0.3f,0),
					_vec3(D3DXToRadian(0),D3DXToRadian(180),0),//rotation
					_vec3(0,0,0),
					1.8f,//tilltime
					0.f//actionTime
				});
				clip->source[2].push_back(
					ANIMINFO{
					_vec3(-2.3f, -1.0f, -0.2f) + _vec3(0,-0.3f,0),
					_vec3(D3DXToRadian(0),D3DXToRadian(180),0),//rotation
					_vec3(0,0,0),
					1.2f,//tilltime
					1.8f//actionTime
				});
			}
			{
				clip->source[3].push_back(
					ANIMINFO{
					_vec3(-0.5f, -1.2f, -0.1f) + _vec3(0,0.3f,0),
					_vec3(D3DXToRadian(0),0,0),//rotation
					_vec3(0,0,0),
					1.8f,//tilltime
					0.f//actionTime
				});
				clip->source[3].push_back(
					ANIMINFO{
					_vec3(-0.5f, -1.2f, -0.1f) + _vec3(0,-0.3f,0),
					_vec3(D3DXToRadian(0),0,0),//rotation
					_vec3(0,0,0),
					1.2f,//tilltime
					1.8f//actionTime
				});
			}
			{
				clip->source[4].push_back(
					ANIMINFO{
					_vec3(-1, 0.5f, -0.3f) + _vec3(0,0.3f,0),
					_vec3(D3DXToRadian(0),0,0),//rotation
					_vec3(0,0,0),
					1.8f,//tilltime
					0.f//actionTime
				});
				clip->source[4].push_back(
					ANIMINFO{
					_vec3(-1, 0.5f, -0.3f) + _vec3(0,-0.3f,0),
					_vec3(D3DXToRadian(0),0,0),//rotation
					_vec3(0,0,0),
					1.2f,//tilltime
					1.8f//actionTime
				});
			}
			{
				clip->source[5].push_back(
					ANIMINFO{
					_vec3(-0.2f, 1, -0.2f) + _vec3(0,0.3f,0),
					_vec3(D3DXToRadian(0),0,0),//rotation
					_vec3(0,0,0),
					1.8f,//tilltime
					0.f//actionTime
				});
				clip->source[5].push_back(
					ANIMINFO{
					_vec3(-0.2f, 1, -0.2f) + _vec3(0,-0.3f,0),
					_vec3(D3DXToRadian(0),0,0),//rotation
					_vec3(0,0,0),
					1.2f,//tilltime
					1.8f//actionTime
				});
			}
			{
				clip->source[6].push_back(
					ANIMINFO{
					_vec3(-0.1f,1.4f,-0.1f) + _vec3(0,0.3f,0),
					_vec3(D3DXToRadian(0),0,0),//rotation
					_vec3(0,0,0),
					1.8f,//tilltime
					0.f//actionTime
				});
				clip->source[6].push_back(
					ANIMINFO{
					_vec3(-0.1f,1.4f,-0.1f) + _vec3(0,-0.3f,0),
					_vec3(D3DXToRadian(0),0,0),//rotation
					_vec3(0,0,0),
					1.2f,//tilltime
					1.8f//actionTime
				});
			}
			SCALEADD;

			clip->TotalTime = 3.f;
			clip->Useloop = true;
		}
		m_pAnimation_Face->AddClip(L"Idle", clip);
		//스마일~
		clip = new AnimClip();
		{
			clip->parts.push_back(m_PartsVec[2]); //눈섭
			clip->parts.push_back(m_PartsVec[3]); //수염
			clip->parts.push_back(m_PartsVec[4]);//수염
			clip->parts.push_back(m_PartsVec[5]);//입
			clip->parts.push_back(m_PartsVec[6]);//코
			clip->parts.push_back(m_PartsVec[7]);//눈
			clip->parts.push_back(m_PartsVec[8]);//눈화장
			clip->source.resize(7);

			
			LerpClipAdd(clip, 0, 0.2f, _vec3(13,13,13),_vec3(2,2,2), _vec3(0, 3, -0.6f), _vec3(0, 2, 0), _vec3(0, 0, 0), _vec3(0, 0, 0), 18);
			LerpClipAdd(clip, 1, 0.2f, _vec3(13,13,13),_vec3(2,2,2), _vec3(1.3f, -1.0f, -0.3f), _vec3(0, 4, 0), _vec3(0, 0, 0), _vec3(0, 0, D3DXToRadian(-50)), 18);
			LerpClipAdd(clip, 2, 0.2f, _vec3(13,13,13),_vec3(2,2,2), _vec3(-2.3f, -1.0f, -0.3f), _vec3(0, 4, 0), _vec3(0, D3DXToRadian(180), 0), _vec3(0, 0, D3DXToRadian(50)), 18);
			LerpClipAdd(clip, 3, 0.2f, _vec3(10,10,10), _vec3(4,4,4), _vec3(-0.5f, -1.2f, -0.2f), _vec3(0, 0, 0), _vec3(0, 0, 0), _vec3(0, 0, 0), 18);
			LerpClipAdd(clip, 4, 0.2f, _vec3(13, 13, 13), _vec3(2, 2, 2), _vec3(-1, 0.5f, -0.4f), _vec3(0, 2, 0), _vec3(0, 0, 0), _vec3(0, 0, 0), 18);
			LerpClipAdd(clip, 5, 0.2f, _vec3(11, 11, 11), _vec3(2, 2, 2), _vec3(-0.2f, 1, -0.2f), _vec3(0, 2, 0), _vec3(0, 0, 0), _vec3(0, 0, 0), 18);
			LerpClipAdd(clip, 6, 0.2f, _vec3(11, 11, 11), _vec3(2, 2, 2), _vec3(-0.1f, 1.4f, -0.1f), _vec3(0, 2, 0), _vec3(0, 0, 0), _vec3(0, 0, 0), 18);
			SCALEADD;
			clip->TotalTime = 0.2f * 18 ;
			clip->Useloop = false;
		}
		m_pAnimation_Face->AddClip(L"Smile", clip);
		m_pAnimation_Face->SetAnimation(L"Idle");
	}
		
	Move(fTimeDelta);

	if (m_bThrow)
		Throw_Cube(fTimeDelta);

	__super::Update_GameObject(fTimeDelta);

	Engine::Add_RenderGroup(RENDER_NONE, this);

	return 0;
}

void CFinal3Boss1::LateUpdate_GameObject(void)
{
	if (0.f >= m_iHp)
		m_bDead = true;

	__super::LateUpdate_GameObject();
}

void CFinal3Boss1::Render_GameObject(void)
{
	__super::Render_GameObject();
}

void CFinal3Boss1::OnCollisionEnter(const Collision * collision)
{
	if (!lstrcmp(collision->otherObj->m_pTag, L"Bullet") ||
		!lstrcmp(collision->otherObj->m_pTag, L"FireBullet"))
	{
		--m_iHp;
		collision->otherObj->m_pTransform->m_vInfo[INFO_POS].y = 220.f;
	}		
	if (!lstrcmp(collision->otherObj->m_pTag, L"SwordBullet"))
	{
		m_iHp -= 2;
		collision->otherObj->m_pTransform->m_vInfo[INFO_POS].y = 220.f;
	}
	if (!lstrcmp(collision->otherObj->m_pTag, L"ShootingLaser"))
	{
		m_iHp -= 0.00005f;
		collision->otherObj->m_pTransform->m_vInfo[INFO_POS].y = 220.f;
		if (Get_GameObject(L"Layer_GameLogic", L"ShootingLaser")->Get_Damage() == true)
		{
			m_iHp -= 1.f;
		}
	}
}
void CFinal3Boss1::OnCollisionStay(const Collision* collision)
{
	if (!lstrcmp(collision->otherObj->m_pTag, L"ShootingLaser"))
	{
		m_iHp -= 0.00005f;
		collision->otherObj->m_pTransform->m_vInfo[INFO_POS].y = 220.f;
		if (Get_GameObject(L"Layer_GameLogic", L"ShootingLaser")->Get_Damage() == true)
		{
			m_iHp -= 1.f;
		}
	}
}
void CFinal3Boss1::SwapTrigger()
{
}


HRESULT CFinal3Boss1::Add_Component(void)
{
	CComponent*		pComponent = nullptr;

	m_pTransform->m_bIsStatic = false;

	pComponent = m_pCollider = dynamic_cast<CCollider*>(Engine::Clone_Proto(L"Collider", this));
	NULL_CHECK_RETURN(m_pCollider, E_FAIL);
	m_vecComponent[ID_DYNAMIC].push_back({ L"Collider", pComponent });
	m_pCollider->Set_Options({ 100.f, 100.f, 100.f }, COL_OBJ, false);

	pComponent = m_pSlerpParticle = dynamic_cast<CSlerpParticle*>(Engine::Clone_Proto(L"SlerpParticle", this));
	NULL_CHECK_RETURN(m_pSlerpParticle, E_FAIL);
	m_vecComponent[ID_STATIC].push_back({ L"SlerpParticle", pComponent });

	pComponent = m_pShadow = dynamic_cast<CShadow*>(Engine::Clone_Proto(L"Shadow", this));
	NULL_CHECK_RETURN(m_pShadow, E_FAIL);
	m_vecComponent[ID_STATIC].push_back({ L"Shadow", pComponent });

	pComponent = m_pAnimation_Whole = dynamic_cast<CAnimation*>(Engine::Clone_Proto(L"Animation", this));
	NULL_CHECK_RETURN(m_pAnimation_Whole, E_FAIL);
	m_vecComponent[ID_DYNAMIC].push_back({ L"Animation", pComponent });

	pComponent = m_pAnimation_Face = dynamic_cast<CAnimation*>(Engine::Clone_Proto(L"Animation", this));
	NULL_CHECK_RETURN(m_pAnimation_Face, E_FAIL);
	m_vecComponent[ID_DYNAMIC].push_back({ L"Animation", pComponent });
	return S_OK;
}

void CFinal3Boss1::LerpClipAdd(AnimClip* clip,_int idx, _float itv, _vec3 osc, _vec3 csc, _vec3 otr, _vec3 ctr, _vec3 orot, _vec3 crot, _int count)
{
	_float pre = 0;
	for (int i = 0; i < count; i++)
	{
		//현재 위치에서 변화량만큼 변화를 줍니다.
		pre = (_float)(i + 1) / count;
		csc = Lerp(csc, _vec3(0, 0, 0), pre);
		ctr = Lerp(ctr, _vec3(0,0,0), pre);
		crot = Lerp(crot, _vec3(0, 0, 0), pre);
		clip->source[idx].push_back(
			ANIMINFO{
			(i % 2 == 0) ? (otr + ctr):(otr -ctr),
			(i % 2 == 0) ? (orot + crot) : (orot -crot),//rotation
			(i % 2 == 0) ? (osc + csc) : (osc - csc),//scale
			itv,//tilltime
			itv*i//actionTime
		});
	}
}

void CFinal3Boss1::Move(const _float& fTimeDelta)
{
	if (m_fOffset_x>3.f)
		m_bTurn_x = false;
	else if(m_fOffset_x<-3.f)
		m_bTurn_x = true;

	if (m_fOffset_y>3.f)
		m_bTurn_y = false;
	else if (m_fOffset_y<-3.f)
		m_bTurn_y = true;

	m_fOffset_x += (m_bTurn_x) ? (fTimeDelta) : (-fTimeDelta);
	m_fOffset_y += (m_bTurn_y) ? (fTimeDelta) : (-fTimeDelta);

	m_pTransform->m_vInfo[INFO_POS].x = m_vOriginPos.x + m_fOffset_x;
	m_pTransform->m_vInfo[INFO_POS].y = m_vOriginPos.y + m_fOffset_y;
}

void CFinal3Boss1::Throw_Cube(const _float & fTimeDelta)
{
	m_dwThrowCubeTime += fTimeDelta;

	int iRandValue = rand() % 18;

	if (1.f < m_dwThrowCubeTime)
	{
		MakeCube(L"FinalCube", iRandValue);

		m_dwThrowCubeTime = 0;
	}

	for (auto& iter = m_vecCube.begin(); iter != m_vecCube.end();)
	{
		if (lstrcmp((*iter)->m_pTag, L"FinalCube"))
			iter = m_vecCube.erase(iter);
		else if (-20.f >= (*iter)->m_pTransform->m_vInfo[INFO_POS].z)
		{
			(*iter)->m_bDead = true;
			iter = m_vecCube.erase(iter);
		}
		else
		{
			(*iter)->m_pTransform->m_vInfo[INFO_POS].z -= 50.f * fTimeDelta;
			++iter;
		}
	}
}

void CFinal3Boss1::MakeCube(const _tchar * pTag, _int iIndex)
{
	CLayer* pLayer = Engine::Get_Layer(L"Layer_GameLogic");

	_int iRandValue = rand() % 5;
	_vec3 vPos;

	switch (iRandValue)
	{
	case 0:
		vPos = _vec3{ 25.f, 20.f, 200.f };
		break;
	case 1:
		vPos = _vec3{ 37.f, 20.f, 200.f };
		break;
	case 2:
		vPos = _vec3{ 28.f, 17.f, 200.f };
		break;
	case 3:
		vPos = _vec3{ 34.f, 17.f, 200.f };
		break;
	case 4:
		vPos = _vec3{ 31.f, 14.f, 200.f };
		break;
	default:
		vPos = _vec3{ 31.f, 14.f, 200.f };
		break;
	}

	CGameObject* pGameObject = CFinalCube::Create(m_pGraphicDev, vPos, iIndex);
	pGameObject->Sort_Component();
	pLayer->Add_GameObject(pTag, pGameObject);
	m_vecCube.push_back(pGameObject);
}

CFinal3Boss1 * CFinal3Boss1::Create(LPDIRECT3DDEVICE9 pGraphicDev, _vec3 & vPos)
{
	CFinal3Boss1*		pInstance = new CFinal3Boss1(pGraphicDev);

	if (FAILED(pInstance->Ready_GameObject(vPos)))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CFinal3Boss1::Free(void)
{
	__super::Free();
}