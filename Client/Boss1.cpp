#include "stdafx.h"
#include "Boss1.h"
#include "AbstractFactory.h"
#include "Export_Function.h"
#include "TopdeeJoint.h"
#include <functional>
CBoss1::CBoss1(LPDIRECT3DDEVICE9 pGraphicDev)
	: CGameObject(pGraphicDev)
{
	m_bInit = true;
}

CBoss1::~CBoss1()
{
}

//FAILED_CHECK_RETURN(FACTORY<CBoss1Hand>::Create(L"Boss1Hand", pLayer, _vec3(60.f, 15.f, 11.f)), E_FAIL);
//FAILED_CHECK_RETURN(FACTORY<CBoss1Head>::Create(L"Boss1Head", pLayer, _vec3(60.f, 8.f, 11.f)), E_FAIL);
//FAILED_CHECK_RETURN(FACTORY<CGiantHand>::Create(L"GiantHand", pLayer, _vec3(60.f, 23.f, 11.f)), E_FAIL);

HRESULT CBoss1::Ready_GameObject(_vec3 & vPos)
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);
	//��ġ��� ģ���� �־��ּ���
	m_pTransform->m_vInfo[INFO_POS] = vPos;
	m_pTransform->m_vAngle = _vec3(D3DXToRadian(-90), D3DXToRadian(90), 0);
	//m_pCollider->Set_BoundingBox({ 10.999f,11.999f,1.0f });
	return S_OK;
}

_int CBoss1::Update_GameObject(const _float & fTimeDelta)
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
		FAILED_CHECK_RETURN(FACTORY<CBoss1Parts>::Create(L"Toodo_Body", pStageLayer, _vec3(0,0,0), m_pTransform, L"Boss1_Parts", 0, false), E_FAIL);
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
		FAILED_CHECK_RETURN(FACTORY<CBoss1Parts>::Create(L"Toodo_MakeUp", pStageLayer, _vec3(0, 0, 0), m_pTransform->GetChild(0)->GetChild(0), L"Boss2_MakeUp", 0, false), E_FAIL);

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

		//�Ӹ���
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
		m_PartsVec[8]->Set_SRT(_vec3(11, 11, 11), _vec3(0,0,0), _vec3(0.4f,1.4f,-0.1f));
		
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
			clip->source.resize(6);

			{
				clip->source[0].push_back(
					ANIMINFO{
					_vec3(0,-3,0) + _vec3(0,1,0),
					_vec3(D3DXToRadian(0.1f),0,0),//rotation
					_vec3(0,0,0),//scale
					1.5f,//tilltime
					0.f//actionTime
				});

				clip->source[0].push_back(
					ANIMINFO{
					_vec3(0,-3,0) + _vec3(0,-1,0),
					_vec3(D3DXToRadian(0.1f),0,0),//rotation
					_vec3(0,0,0),
					1.5f,//tilltime
					1.5f//actionTime
				});
			}
			{
				clip->source[1].push_back(
					ANIMINFO{
					_vec3(0,12,-0.1f) + _vec3(0,1,0),
					_vec3(D3DXToRadian(0.1f),0,0),//rotation
					_vec3(0,0,0),
					1.5f,//tilltime
					0.f//actionTime
				});
				clip->source[1].push_back(
					ANIMINFO{
					_vec3(0,12,-0.1f) + _vec3(0,-2,0),
					_vec3(D3DXToRadian(0.1f),0,0),//rotation
					_vec3(0,0,0),
					1.5f,//tilltime
					1.5f//actionTime
				});
			}
			{
				clip->source[2].push_back(
					ANIMINFO{
					_vec3(-9, 6, 0.1f) + _vec3(0,1,0),
					_vec3(D3DXToRadian(0),0,0),//rotation
					_vec3(0,0,0),
					1.5f,//tilltime
					0.f//actionTime
				});
				clip->source[2].push_back(
					ANIMINFO{
					_vec3(-9, 6, 0.1f) + _vec3(0,-2,0),
					_vec3(D3DXToRadian(0),0,0),//rotation
					_vec3(0,0,0),
					1.5f,//tilltime
					1.5f//actionTime
				});
			}
			{
				clip->source[3].push_back(
					ANIMINFO{
					_vec3(0, -4, -0.1f) + _vec3(0,1,0),
					_vec3(D3DXToRadian(0),0,0),//rotation
					_vec3(0,0,0),
					1.5f,//tilltime
					0.f//actionTime
				});
				clip->source[3].push_back(
					ANIMINFO{
					_vec3(0, -4, -0.1f) + _vec3(0,-2,0),
					_vec3(D3DXToRadian(0),0,0),//rotation
					_vec3(0,0,0),
					1.5f,//tilltime
					1.5f//actionTime
				});
			}
			{
				clip->source[4].push_back(
					ANIMINFO{
					_vec3(9, 6, 0.1f) + _vec3(0,1,0),
					_vec3(D3DXToRadian(0),0,0),//rotation
					_vec3(0,0,0),
					1.5f,//tilltime
					0.f//actionTime
				});
				clip->source[4].push_back(
					ANIMINFO{
					_vec3(9, 6, 0.1f) + _vec3(0,-2,0),
					_vec3(D3DXToRadian(0),0,0),//rotation
					_vec3(0,0,0),
					1.5f,//tilltime
					1.5f//actionTime
				});
			}
			{
				clip->source[5].push_back(
					ANIMINFO{
					_vec3(0, -4, -0.1f) + _vec3(0,1,0),
					_vec3(D3DXToRadian(0),0,0),//rotation
					_vec3(0,0,0),
					1.5f,//tilltime
					0.f//actionTime
				});
				clip->source[5].push_back(
					ANIMINFO{
					_vec3(0, -4, -0.1f) + _vec3(0,-2,0),
					_vec3(D3DXToRadian(0),0,0),//rotation
					_vec3(0,0,0),
					1.5f,//tilltime
					1.5f//actionTime
				});
			}
			clip->TotalTime = 3.f;
			clip->Useloop = true;
		}
		m_pAnimation_Whole->AddClip(L"Idle", clip);

		//�հ��� ������
		clip = new AnimClip();
		{
			clip->parts.push_back(m_PartsVec[0]); //����
			clip->parts.push_back(m_PartsVec[1]); //�Ӹ���
			clip->parts.push_back(m_PartsVec[9]);//����� ����
			clip->parts.push_back(m_PartsVec[11]);//�ȵ� ����
			clip->parts.push_back(m_PartsVec[15]);//����� ����
			clip->parts.push_back(m_PartsVec[17]);//�ȵ� ����
			//�ո�
			clip->parts.push_back(m_PartsVec[13]);//�ո���
			clip->source.resize(7);

			LerpClipAdd(clip, 0, 0.2f, 12, 2, _vec3(16, -16, 0), _vec3(0, 2, 0), _vec3(0, 0, 0), _vec3(0, 0, 0), 18);
			LerpClipAdd(clip, 1, 0.2f, 1, 0.3f, _vec3(0, 12, -0.1f), _vec3(0, 2, 0), _vec3(0, 0, 0), _vec3(0, 0, D3DXToRadian(-50)), 18);

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

			LerpClipAdd(clip, 6, 0.2f, 13, 4, _vec3(0, -5, -0.1f), _vec3(0, 2, 0), _vec3(0, 0, D3DXToRadian(0)), _vec3(0, 0, 0), 18);
			LerpClipAdd(clip, 4, 0.2f, 13, 4, _vec3(9, 6, 0.1f), _vec3(0, 2, 0), _vec3(0, D3DXToRadian(0), D3DXToRadian(30)), _vec3(0, 0, 0), 18);
			LerpClipAdd(clip, 5, 0.2f, 13, 4, _vec3(0, -4, -0.1f), _vec3(0, 2, 0), _vec3(0, 0, D3DXToRadian(-120)), _vec3(0, 0, 0), 18);
			
			clip->TotalTime = 0.2f*18;
			clip->Useloop = true;
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
												  //�ո�
			clip->parts.push_back(m_PartsVec[13]);//�ո���
			clip->source.resize(7);

			LerpClipAdd(clip, 0, 0.2f, 12, 0.4f, _vec3(16, -16, 0), _vec3(0, 1, 0), _vec3(0, 0, 0), _vec3(0, 0, 0), 2);
			LerpClipAdd(clip, 1, 0.2f, 1, 0.1f, _vec3(0, 12, -0.1f), _vec3(0, 1, 0), _vec3(0, 0, 0), _vec3(0, 0, 0), 2);
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
			LerpClipAdd(clip, 4, 0.2f, 13, 0.4f, _vec3(9, 6, 0.1f), _vec3(0, 1, 0), _vec3(0, D3DXToRadian(0), D3DXToRadian(30)), _vec3(0, 0, 0), 2);
			LerpClipAdd(clip, 5, 0.2f, 13, 0.4f, _vec3(0, -4, -0.1f), _vec3(0, 1, 0), _vec3(0, 0, D3DXToRadian(-120)), _vec3(0, 0, 0), 2);

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
			LerpClipAdd(clip, 0, 0.2f, 12, 2, _vec3(0, -3, 0), _vec3(0, 2, 0), _vec3(0, 0, 0), _vec3(0, 0, 0), 18);
			LerpClipAdd(clip, 1, 0.2f, 1, 0.3f, _vec3(0, 12, -0.1f), _vec3(0, 2, 0), _vec3(0, 0, 0), _vec3(0, 0, D3DXToRadian(-50)), 18);
			LerpClipAdd(clip, 2, 0.2f, 13, 4, _vec3(-9, 6, 0.1f), _vec3(0, 2, 0), _vec3(0, D3DXToRadian(0), D3DXToRadian(-30)), _vec3(0, 0, D3DXToRadian(50)), 18);
			LerpClipAdd(clip, 3, 0.2f, 13, 4, _vec3(0, -4, -0.1f), _vec3(0, 2, 0), _vec3(0, 0, D3DXToRadian(120)), _vec3(0, 0, 0), 18);
			LerpClipAdd(clip, 4, 0.2f, 13, 4, _vec3(9, 6, 0.1f), _vec3(0, 2, 0), _vec3(0, D3DXToRadian(0), D3DXToRadian(30)), _vec3(0, 0, 0), 18);
			LerpClipAdd(clip, 5, 0.2f, 13, 4, _vec3(0, -4, -0.1f), _vec3(0, 2, 0), _vec3(0, 0, D3DXToRadian(-120)), _vec3(0, 0, 0), 18);
			clip->TotalTime = 0.2f*18;
			clip->Useloop = true;
		}
		m_pAnimation_Whole->AddClip(L"Foot", clip);
		m_pAnimation_Whole->SetAnimation(L"Idle");

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
					_vec3(0.4f,1.4f,-0.1f) + _vec3(0,0.3f,0),
					_vec3(D3DXToRadian(0),0,0),//rotation
					_vec3(0,0,0),
					1.8f,//tilltime
					0.f//actionTime
				});
				clip->source[6].push_back(
					ANIMINFO{
					_vec3(0.4f,1.4f,-0.1f) + _vec3(0,-0.3f,0),
					_vec3(D3DXToRadian(0),0,0),//rotation
					_vec3(0,0,0),
					1.2f,//tilltime
					1.8f//actionTime
				});
			}
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

			LerpClipAdd(clip, 0, 0.2f, 13, 2, _vec3(0, 3, -0.6f), _vec3(0, 2, 0), _vec3(0, 0, 0), _vec3(0, 0, 0), 18);
			LerpClipAdd(clip, 1, 0.2f, 13, 2, _vec3(1.3f, -1.0f, -0.3f), _vec3(0, 4, 0), _vec3(0, 0, 0), _vec3(0, 0, D3DXToRadian(-50)), 18);
			LerpClipAdd(clip, 2, 0.2f, 13, 2, _vec3(-2.3f, -1.0f, -0.3f), _vec3(0, 4, 0), _vec3(0, D3DXToRadian(180), 0), _vec3(0, 0, D3DXToRadian(50)), 18);
			LerpClipAdd(clip, 3, 0.2f, 10, 4, _vec3(-0.5f, -1.2f, -0.2f), _vec3(0, 0, 0), _vec3(0, 0, 0), _vec3(0, 0, 0), 18);
			LerpClipAdd(clip, 4, 0.2f, 13, 2, _vec3(-1, 0.5f, -0.4f), _vec3(0, 2, 0), _vec3(0, 0, 0), _vec3(0, 0, 0), 18);
			LerpClipAdd(clip, 5, 0.2f, 11, 2, _vec3(-0.2f, 1, -0.2f), _vec3(0, 2, 0), _vec3(0, 0, 0), _vec3(0, 0, 0), 18);
			LerpClipAdd(clip, 6, 0.2f, 11, 2, _vec3(0.4f, 1.4f, -0.1f), _vec3(0, 2, 0), _vec3(0, 0, 0), _vec3(0, 0, 0), 18);
			
			clip->TotalTime = 0.2f * 18 ;
			clip->Useloop = true;
		}
		m_pAnimation_Face->AddClip(L"Smile", clip);
		m_pAnimation_Face->SetAnimation(L"Smile");
	}
	Engine::Add_RenderGroup(RENDER_NONALPHA, this);
	__super::Update_GameObject(fTimeDelta);
	return 0;
}

void CBoss1::LateUpdate_GameObject(void)
{
	__super::LateUpdate_GameObject();
}

void CBoss1::Render_GameObject(void)
{
	__super::Render_GameObject();
}
void CBoss1::SwapTrigger()
{
}

HRESULT CBoss1::Add_Component(void)
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

void CBoss1::LerpClipAdd(AnimClip* clip,_int idx, _float itv,_float osc, _float csc, _vec3 otr, _vec3 ctr, _vec3 orot, _vec3 crot, _int count)
{
	_float pre = 0;
	for (int i = 0; i < count; i++)
	{
		//���� ��ġ���� ��ȭ����ŭ ��ȭ�� �ݴϴ�.
		pre = (_float)(i + 1) / count;
		csc = Lerp(csc, 0, pre);
		ctr = Lerp(ctr, _vec3(0,0,0), pre);
		crot = Lerp(crot, _vec3(0, 0, 0), pre);
		clip->source[idx].push_back(
			ANIMINFO{
			(i % 2 == 0) ? (otr + ctr):(otr -ctr),
			(i % 2 == 0) ? (orot + crot) : (orot -crot),//rotation
			(i % 2 == 0) ? (_vec3(osc,osc,osc) + _vec3(csc,csc,csc)): (_vec3(osc,osc,osc) + _vec3(-csc,-csc,-csc)),//scale
			itv,//tilltime
			itv*i//actionTime
		});
	}
}

CBoss1 * CBoss1::Create(LPDIRECT3DDEVICE9 pGraphicDev, _vec3 & vPos)
{
	CBoss1*		pInstance = new CBoss1(pGraphicDev);

	if (FAILED(pInstance->Ready_GameObject(vPos)))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CBoss1::Free(void)
{
	__super::Free();
}
