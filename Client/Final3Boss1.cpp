#include "stdafx.h"
#include "Final3Boss1.h"
#include "AbstractFactory.h"
#include "Export_Function.h"
#include "TopdeeJoint.h"
#include "Boss1Hand.h"
#include "GiantHand.h"
#include "Boss1Head.h"
#include <functional>

#define	BOSS1SCALE 2.f
#define SCALEADD for (int i = 0; i < clip->source.size(); i++)\
for (int j = 0; j < clip->source[i].size(); j++)\
{\
	clip->source[i][j].scale *= BOSS1SCALE;\
	clip->source[i][j].trans *= 4.f;\
}\

CFinal3Boss1::CFinal3Boss1(LPDIRECT3DDEVICE9 pGraphicDev)
	: CGameObject(pGraphicDev)
{
	m_bInit = true;
	m_dwRestTime = 1;
}

CFinal3Boss1::~CFinal3Boss1()
{
}

HRESULT CFinal3Boss1::Ready_GameObject(_vec3 & vPos)
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);
	//��ġ��� ģ���� �־��ּ���
	m_pTransform->m_vInfo[INFO_POS] = vPos;
	m_vOriginPos = vPos;
	m_pTransform->m_vAngle = _vec3(D3DXToRadian(-90), D3DXToRadian(0), 0);
	m_bTurn_x=false;
	m_bTurn_y=false;
	m_fOffset_x = 0;
	m_fOffset_y = 0;
	return S_OK;
}

_int CFinal3Boss1::Update_GameObject(const _float & fTimeDelta)
{
	if (m_bInit)
	{
		//��� �������־����.
		CComponent* otherTrans = Engine::Get_Component(L"Layer_GameLogic", L"Thirddee", L"Transform", ID_DYNAMIC);
		if (otherTrans != nullptr)
			m_Player = dynamic_cast<CTransform*>(otherTrans);

		//�������ִ� �ڵ�
		CLayer* pStageLayer = dynamic_cast<CLayer*>(Engine::Get_Layer(L"Layer_GameLogic"));
		NULL_CHECK_RETURN(pStageLayer, E_FAIL);

		//���� 0
		FAILED_CHECK_RETURN(FACTORY<CTopdeeJoint>::Create(L"Toodo_Body", pStageLayer, _vec3(0,0,0), m_pTransform), E_FAIL);
		//�Ӹ��� 0-0
		FAILED_CHECK_RETURN(FACTORY<CBoss1Parts>::Create(L"Toodo_Head", pStageLayer, _vec3(0, 0, 0), m_pTransform->GetChild(0), L"Boss1_Parts", 6, false), E_FAIL);
		//���� 0-0-0
		FAILED_CHECK_RETURN(FACTORY<CBoss1Parts>::Create(L"Toodo_EyeBrow", pStageLayer, _vec3(0, 0, 0), m_pTransform->GetChild(0)->GetChild(0), L"Boss1_Parts", 8, false), E_FAIL);
		//���� 0-0-1~2
		FAILED_CHECK_RETURN(FACTORY<CBoss1Parts>::Create(L"Toodo_Beard", pStageLayer, _vec3(0, 0, 0), m_pTransform->GetChild(0)->GetChild(0), L"Boss1_Parts", 9, false), E_FAIL);
		FAILED_CHECK_RETURN(FACTORY<CBoss1Parts>::Create(L"Toodo_Beard", pStageLayer, _vec3(0, 0, 0), m_pTransform->GetChild(0)->GetChild(0), L"Boss1_Parts", 9, false), E_FAIL);
		//�� 0 0 3
		FAILED_CHECK_RETURN(FACTORY<CBoss1Parts>::Create(L"Toodo_Mouse", pStageLayer, _vec3(0, 0, 0), m_pTransform->GetChild(0)->GetChild(0), L"Boss1_Parts", 4, false), E_FAIL);
		//�� 0-0-4
		FAILED_CHECK_RETURN(FACTORY<CBoss1Parts>::Create(L"Toodo_Nose", pStageLayer, _vec3(0, 0, 0), m_pTransform->GetChild(0)->GetChild(0), L"Boss1_Parts", 10, false), E_FAIL);
		//�� 0 0 5
		FAILED_CHECK_RETURN(FACTORY<CBoss1Parts>::Create(L"Toodo_Eye", pStageLayer, _vec3(0, 0, 0), m_pTransform->GetChild(0)->GetChild(0), L"Boss1_Parts", 7, false), E_FAIL);
		//��ȭ�� 0 0 6
		FAILED_CHECK_RETURN(FACTORY<CBoss1Parts>::Create(L"Toodo_MakeUp", pStageLayer, _vec3(0, 0, 0), m_pTransform->GetChild(0)->GetChild(0), L"Boss2_MakeUp", 8, true), E_FAIL);

		//�� ������� ž���� ����Ʈ�� ���ľ��ϴ�.
		//��� 0 1~2
		FAILED_CHECK_RETURN(FACTORY<CTopdeeJoint>::Create(L"Toodo_Shoulder", pStageLayer, _vec3(0, 0, 0), m_pTransform->GetChild(0)), E_FAIL);
		FAILED_CHECK_RETURN(FACTORY<CTopdeeJoint>::Create(L"Toodo_Shoulder", pStageLayer, _vec3(0, 0, 0), m_pTransform->GetChild(0)), E_FAIL);
		
		//����� �ڽ� ���																										
		FAILED_CHECK_RETURN(FACTORY<CBoss1Parts>::Create(L"Toodo_Samdoo", pStageLayer, _vec3(0, 0, 0), m_pTransform->GetChild(0)->GetChild(1), L"Boss1_Parts", 1, false), E_FAIL);
		FAILED_CHECK_RETURN(FACTORY<CBoss1Parts>::Create(L"Toodo_Samdoo", pStageLayer, _vec3(0, 0, 0), m_pTransform->GetChild(0)->GetChild(2), L"Boss1_Parts", 1, false), E_FAIL);
		
		//����� �ڽ� ��
		FAILED_CHECK_RETURN(FACTORY<CTopdeeJoint>::Create(L"Toodo_Arm", pStageLayer, _vec3(0, 0, 0), m_pTransform->GetChild(0)->GetChild(1)->GetChild(0)), E_FAIL);
		FAILED_CHECK_RETURN(FACTORY<CTopdeeJoint>::Create(L"Toodo_Arm", pStageLayer, _vec3(0, 0, 0), m_pTransform->GetChild(0)->GetChild(2)->GetChild(0)), E_FAIL);
		
		//���� �ڽ� �ϱ���								
		FAILED_CHECK_RETURN(FACTORY<CBoss1Parts>::Create(L"Toodo_Wan", pStageLayer, _vec3(0, 0, 0), m_pTransform->GetChild(0)->GetChild(1)->GetChild(0)->GetChild(0), L"Boss1_Parts", 2, false), E_FAIL);
		FAILED_CHECK_RETURN(FACTORY<CBoss1Parts>::Create(L"Toodo_Wan", pStageLayer, _vec3(0, 0, 0), m_pTransform->GetChild(0)->GetChild(2)->GetChild(0)->GetChild(0), L"Boss1_Parts", 2, false), E_FAIL);
		
		//�ϱ����� �ڽ� �ո�
		FAILED_CHECK_RETURN(FACTORY<CTopdeeJoint>::Create(L"Toodo_HandNeck", pStageLayer, _vec3(0, 0, 0), m_pTransform->GetChild(0)->GetChild(1)->GetChild(0)->GetChild(0)->GetChild(0)), E_FAIL);
		FAILED_CHECK_RETURN(FACTORY<CTopdeeJoint>::Create(L"Toodo_HandNeck", pStageLayer, _vec3(0, 0, 0), m_pTransform->GetChild(0)->GetChild(2)->GetChild(0)->GetChild(0)->GetChild(0)), E_FAIL);
		
		//�ո����� �ڽ� ��																									
		FAILED_CHECK_RETURN(FACTORY<CBoss1Parts>::Create(L"Toodo_Hand", pStageLayer, _vec3(0, 0, 0), m_pTransform->GetChild(0)->GetChild(1)->GetChild(0)->GetChild(0)->GetChild(0)->GetChild(0), L"Boss1_Parts", 3, false), E_FAIL);
		FAILED_CHECK_RETURN(FACTORY<CBoss1Parts>::Create(L"Toodo_Hand", pStageLayer, _vec3(0, 0, 0), m_pTransform->GetChild(0)->GetChild(2)->GetChild(0)->GetChild(0)->GetChild(0)->GetChild(0), L"Boss1_Parts", 3, false), E_FAIL);

		FAILED_CHECK_RETURN(FACTORY<CBoss1Parts>::Create(L"Toodo_Body", pStageLayer, _vec3(0, 0, 0), m_pTransform->GetChild(0), L"Boss1_Parts", 0, false), E_FAIL);
		m_PartsVec.push_back(m_pTransform->GetChild(0));
		
		//��Ͷ��ٶ���
		function<void(CTransform*, vector<CTransform*>&)> func = [&](CTransform* parent,vector<CTransform*>& vec) -> void{
			for (int i = 0; i < parent->GetChildCount(); i++)
			{
				vec.push_back(parent->GetChild(i));
				func(parent->GetChild(i), vec);
			}
		};
		func(m_pTransform->GetChild(0), m_PartsVec);

		//��
		m_PartsVec[0]->Set_SRT(_vec3(12,12,12), _vec3(0,0,0), _vec3(0,-3,0));
		//�Ӹ�
		m_PartsVec[1]->Set_SRT(_vec3(1.f,1.f,1.f), _vec3(0,0,0), _vec3(0,12,-0.1f));
		//����
		m_PartsVec[2]->Set_SRT(_vec3(13,13,13), _vec3(0,0,0), _vec3(0,3,-0.2f));
		//����
		m_PartsVec[3]->Set_SRT(_vec3(13, 13, 13), _vec3(0, 0, 0), _vec3(1.3f, -1.0f, -0.2f));
		m_PartsVec[4]->Set_SRT(_vec3(13, 13, 13), _vec3(0, D3DXToRadian(180), 0), _vec3(-2.3f, -1.0f, -0.2f));
		//��
		m_PartsVec[5]->Set_SRT(_vec3(10, 10, 10), _vec3(0, D3DXToRadian(180), 0), _vec3(-0.5f, -1.2f, -0.1f));
		//��
		m_PartsVec[6]->Set_SRT(_vec3(13,13,13), _vec3(0, 0, 0), _vec3(-1, 0.5f, -0.3f));
		//��
		m_PartsVec[7]->Set_SRT(_vec3(11, 11, 11), _vec3(0, 0, 0), _vec3(-0.2f, 1, -0.2f));
		//��ȭ��
		m_PartsVec[8]->Set_SRT(_vec3(11, 11, 11), _vec3(0,0,0), _vec3(-0.1f,1.4f,-0.1f));

		dynamic_cast<CBoss1Parts*>(m_PartsVec[8]->m_pGameObject)->MakeAnim(L"Attack",0,8,0.6f,true);
		dynamic_cast<CBoss1Parts*>(m_PartsVec[8]->m_pGameObject)->MakeAnim(L"Idle", 0, 0, 1, false);
		dynamic_cast<CBoss1Parts*>(m_PartsVec[8]->m_pGameObject)->SetAnim(L"Idle");
		//����
		//�������Ʈ
		m_PartsVec[9]->Set_SRT(_vec3(13,13,13), _vec3(0,D3DXToRadian(0), D3DXToRadian(-30)), _vec3(-9,6,0.1f));
		//���
		m_PartsVec[10]->Set_SRT(_vec3(1,1,1), _vec3(0,0,0), _vec3(0,-3,0));
		//��ġ
		m_PartsVec[11]->Set_SRT(_vec3(13, 13, 13), _vec3(0, 0, D3DXToRadian(120)), _vec3(0, -4, -0.1f));
		//�ϱ���
		m_PartsVec[12]->Set_SRT(_vec3(1,1,1), _vec3(0,0,0), _vec3(0,-2,0));
		//�ո���
		m_PartsVec[13]->Set_SRT(_vec3(13, 13, 13), _vec3(0,0, D3DXToRadian(0)), _vec3(0,-3, -0.1f));
		//��
		m_PartsVec[14]->Set_SRT(_vec3(1,1,1), _vec3(0,0,0), _vec3(0,-1,0));

		//��
		//�������Ʈ
		m_PartsVec[15]->Set_SRT(_vec3(-13, 13, 13), _vec3(0, D3DXToRadian(0), D3DXToRadian(30)), _vec3(9, 6, 0.1f));
		//���
		m_PartsVec[16]->Set_SRT(_vec3(1, 1, 1), _vec3(0, 0, 0), _vec3(0, -3, 0));
		//��ġ
		m_PartsVec[17]->Set_SRT(_vec3(-13, 13, 13), _vec3(0, 0, D3DXToRadian(-120)), _vec3(0, -4, -0.1f));
		//�ϱ���
		m_PartsVec[18]->Set_SRT(_vec3(1, 1, 1), _vec3(0, 0, 0), _vec3(0, -2, 0));
		//�ո���
		m_PartsVec[19]->Set_SRT(_vec3(13, 13, 13), _vec3(0, 0, D3DXToRadian(0)), _vec3(0, -3, -0.1f));
		//��
		m_PartsVec[20]->Set_SRT(_vec3(-1, 1, 1), _vec3(0, 0, 0), _vec3(0, -1, 0));

		m_bInit = false;

		for (int i = 0; i < m_PartsVec.size(); i++)
		{
			if (i == 0)
				continue;
			m_PartsVec[i]->m_vScale *= BOSS1SCALE;
			m_PartsVec[i]->m_vInfo[INFO_POS] *= BOSS1SCALE;
		}

		//�׳� �յ�����.
		AnimClip* clip = nullptr;
		clip = new AnimClip();
		{
			clip->parts.push_back(m_PartsVec[0]); //����
			clip->parts.push_back(m_PartsVec[1]); //�Ӹ���
			clip->parts.push_back(m_PartsVec[9]);//����� ����
			clip->parts.push_back(m_PartsVec[11]);//�ȵ� ����
			clip->parts.push_back(m_PartsVec[15]);//����� ����
			clip->parts.push_back(m_PartsVec[17]);//�ȵ� ����
			clip->parts.push_back(m_PartsVec[13]);//�ո���
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
			
		//�հ��� ������
		clip = new AnimClip();
		{ PlaySound_Effect(L"34", SOUND_EFFECT_ENEMY, 1.f);
			clip->parts.push_back(m_PartsVec[0]); //����
			clip->parts.push_back(m_PartsVec[1]); //�Ӹ���
			clip->parts.push_back(m_PartsVec[9]);//����� ����
			clip->parts.push_back(m_PartsVec[11]);//�ȵ� ����
			clip->parts.push_back(m_PartsVec[15]);//����� ����
			clip->parts.push_back(m_PartsVec[17]);//�ȵ� ����
			//�ո�
			clip->parts.push_back(m_PartsVec[13]);//�ո���
			clip->source.resize(7);

			LerpClipAdd(clip, 0, 0.2f, _vec3(12,12,12), _vec3(2,2,2), _vec3(8, -8, 0), _vec3(0, 2, 0), _vec3(0, 0, 0), _vec3(0, 0, 0), 18);
			LerpClipAdd(clip, 1, 0.2f, _vec3(1,1,1), _vec3(0.3f, 0.3f, 0.3f), _vec3(0, 12, -0.1f), _vec3(0, 2, 0), _vec3(0, 0, 0), _vec3(0, 0, D3DXToRadian(-50)), 18);

			//���ø� ���/�� ����
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

		//�հ��� ������
		clip = new AnimClip();
		{
			clip->parts.push_back(m_PartsVec[0]); //����
			clip->parts.push_back(m_PartsVec[1]); //�Ӹ���
			clip->parts.push_back(m_PartsVec[9]);//����� ����
			clip->parts.push_back(m_PartsVec[11]);//�ȵ� ����
			clip->parts.push_back(m_PartsVec[15]);//����� ����
			clip->parts.push_back(m_PartsVec[17]);//�ȵ� ����
			clip->parts.push_back(m_PartsVec[13]);//�ո���
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

		//�߰�����
		clip = new AnimClip();
		{
			clip->parts.push_back(m_PartsVec[0]); //����
			clip->parts.push_back(m_PartsVec[1]); //�Ӹ���
			clip->parts.push_back(m_PartsVec[9]);//����� ����
			clip->parts.push_back(m_PartsVec[11]);//�ȵ� ����
			clip->parts.push_back(m_PartsVec[15]);//����� ����
			clip->parts.push_back(m_PartsVec[17]);//�ȵ� ����
			clip->source.resize(6);
										//����	//�̵�								//ȸ��
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

		//�߰�����
		clip = new AnimClip();
		{
			clip->parts.push_back(m_PartsVec[0]); //����
			clip->parts.push_back(m_PartsVec[1]); //�Ӹ���
			clip->parts.push_back(m_PartsVec[9]);//����� ����
			clip->parts.push_back(m_PartsVec[11]);//�ȵ� ����
			clip->parts.push_back(m_PartsVec[13]);//�ȵ� ����
			clip->parts.push_back(m_PartsVec[15]);//����� ����
			clip->parts.push_back(m_PartsVec[17]);//�ȵ� ����
			clip->parts.push_back(m_PartsVec[19]);//�ȵ� ����
			clip->source.resize(8);
			LerpClipAdd(clip, 0, 0.2f, _vec3(12, 12, 12), _vec3(2, 2, 2), _vec3(8, -8, 0), _vec3(0, 2, 0), _vec3(0, 0, 0), _vec3(0, 0, 0), 18);
			LerpClipAdd(clip, 1, 0.2f, _vec3(1, 1, 1), _vec3(0.3f, 0.3f, 0.3f), _vec3(0, 12, -0.1f), _vec3(0, 2, 0), _vec3(0, 0, 0), _vec3(0, 0, D3DXToRadian(-50)), 18);

			//���ø� ���/�� ����
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

		//�׳� ������
		clip = new AnimClip();
		{
			clip->parts.push_back(m_PartsVec[2]); //����
			clip->parts.push_back(m_PartsVec[3]); //����
			clip->parts.push_back(m_PartsVec[4]);//����
			clip->parts.push_back(m_PartsVec[5]);//��
			clip->parts.push_back(m_PartsVec[6]);//��
			clip->parts.push_back(m_PartsVec[7]);//��
			clip->parts.push_back(m_PartsVec[8]);//��ȭ��
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
		//������~
		clip = new AnimClip();
		{
			clip->parts.push_back(m_PartsVec[2]); //����
			clip->parts.push_back(m_PartsVec[3]); //����
			clip->parts.push_back(m_PartsVec[4]);//����
			clip->parts.push_back(m_PartsVec[5]);//��
			clip->parts.push_back(m_PartsVec[6]);//��
			clip->parts.push_back(m_PartsVec[7]);//��
			clip->parts.push_back(m_PartsVec[8]);//��ȭ��
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

	__super::Update_GameObject(fTimeDelta);
	return 0;
}

void CFinal3Boss1::LateUpdate_GameObject(void)
{
	__super::LateUpdate_GameObject();
}

void CFinal3Boss1::Render_GameObject(void)
{
	__super::Render_GameObject();
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
		//���� ��ġ���� ��ȭ����ŭ ��ȭ�� �ݴϴ�.
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
