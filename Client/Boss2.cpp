#include "stdafx.h"
#include "Boss2.h"
#include "Export_Function.h"
#include "StageCamera.h"
#include "Cube.h"
#include "Boss2Hand.h"
#include "AbstractFactory.h"
#include "Boss2Eye.h"
#include "Boss2Face.h"
#include "Boss2Jaw.h"
#include "Boss2Nose.h"
#include "Boss2EyeBrow.h"
#include "Boss2TailBody.h"
#include "Boss2Foot.h"
#include "Boss2Body.h"
#include "Boss2Chain.h"
#include "Boss2JointSpot.h"
#include "AbstractFactory.h"
#include "Spike.h"

CBoss2::CBoss2(LPDIRECT3DDEVICE9 pGraphicDev) 
	: CGameObject(pGraphicDev)
{
}


CBoss2::~CBoss2()
{
}

HRESULT CBoss2::Ready_GameObject(_vec3 & vPos)
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	m_pTransform->m_bIsStatic = false;

	m_eCurrentState = B2_JUMPING;
	m_ePreState = B2_END;
	m_bInit = false;
	//?˜ë¨¸ì§€ ?„ì¹˜?????Œí™˜
	m_bIsOnGround = false;
	m_fJumpPos[0] = _vec3(10,25,10);
	m_fJumpPos[1] = _vec3(30,25,10);
	m_fJumpPos[2] = _vec3(50,25,10);
	m_iJumpPosidx = 0;//?ì‹  ?„ì¹˜
	m_dwRestTime = 0;
	ReadyPartten();

	m_pTransform->m_vScale = { 1.f, 1.f, 1.f };
	m_pCollider->Set_BoundingBox(_vec3(6, 6, 2));
	m_pCollider->Set_BoundOffset(_vec3(0,0,0));
	m_pTransform->m_vInfo[INFO_POS] = vPos;
	m_pRigid->m_bUseGrivaty = false;

	return S_OK;
}

_int CBoss2::Update_GameObject(const _float & fTimeDelta)
{
	__super::Update_GameObject(fTimeDelta);

	//?Œë ˆ?´ì–´ ê°€?¸ì˜¤ê¸?
	if (!m_bInit)
	{
		//?ˆì•Œ ?ìª½ ?ì„±
		CLayer* pStageLayer = dynamic_cast<CLayer*>(Engine::Get_Layer(L"Layer_GameLogic"));
		NULL_CHECK_RETURN(pStageLayer, E_FAIL);

		//Head Joint (0)
		FAILED_CHECK_RETURN(FACTORY<CBoss2JointSpot>::Create(L"Boss2JointSpot", pStageLayer, _vec3(0.0f, -0.0f, -0.0f), m_pTransform), E_FAIL);
		//Head Children
		{
			FAILED_CHECK_RETURN(FACTORY<CBoss2Face>::Create(L"Boss2Face", pStageLayer, _vec3(-1.f, 0.f, -0.2f), m_pTransform->GetChild(0)), E_FAIL);
			FAILED_CHECK_RETURN(FACTORY<CBoss2Jaw>::Create(L"Boss2Jaw", pStageLayer, _vec3(-1.f, -1.f, -0.1f), m_pTransform->GetChild(0)), E_FAIL);
			FAILED_CHECK_RETURN(FACTORY<CBoss2Eye>::Create(L"Boss2Eye", pStageLayer, _vec3(-2.1f, 0.6f, -0.3f), m_pTransform->GetChild(0)), E_FAIL);
			FAILED_CHECK_RETURN(FACTORY<CBoss2Eye>::Create(L"Boss2Eye", pStageLayer, _vec3(-0.3f, 0.6f, -0.3f), m_pTransform->GetChild(0)), E_FAIL);
			FAILED_CHECK_RETURN(FACTORY<CBoss2Nose>::Create(L"Boss2EyeNose", pStageLayer, _vec3(-1.6f, 0.2f, -0.7f), m_pTransform->GetChild(0)), E_FAIL);
			FAILED_CHECK_RETURN(FACTORY<CBoss2EyeBrow>::Create(L"Boss2EyeBrow", pStageLayer, _vec3(-2.1f, 1.4f, -0.5f), m_pTransform->GetChild(0)), E_FAIL);
			m_pTransform->GetChild(0)->GetChild(5)->m_vAngle = _vec3(0, 0, D3DXToRadian(-50.f));
			FAILED_CHECK_RETURN(FACTORY<CBoss2EyeBrow>::Create(L"Boss2EyeBrow", pStageLayer, _vec3(-0.3f, 1.4f, -0.5f), m_pTransform->GetChild(0)), E_FAIL);
			m_pTransform->GetChild(0)->GetChild(6)->m_vScale = _vec3(0.7f, 0.7f, 0.7f);
			FAILED_CHECK_RETURN(FACTORY<CBoss2Body>::Create(L"Boss2Body", pStageLayer, _vec3(0.f, 0.0f, 0.0f), m_pTransform->GetChild(0)), E_FAIL);
			m_pTransform->GetChild(0)->GetChild(7)->m_vScale = _vec3(3.8f, 3.8f, 3.8f);
			dynamic_cast<CBoss2Body*>(m_pTransform->GetChild(0)->GetChild(7)->m_pGameObject)->SetRotAngle(30, 60);
		}
		//Body Joint (1)
		FAILED_CHECK_RETURN(FACTORY<CBoss2JointSpot>::Create(L"Boss2JointSpot", pStageLayer, _vec3(3.f, 0.3f, 0.1f), m_pTransform), E_FAIL);
		//Body Children
		{
			//0~4
			FAILED_CHECK_RETURN(FACTORY<CBoss2Foot>::Create(L"Boss2Foot", pStageLayer, _vec3(-2.1f, -3.4f, -0.5f), m_pTransform->GetChild(1)), E_FAIL);
			FAILED_CHECK_RETURN(FACTORY<CBoss2Foot>::Create(L"Boss2Foot", pStageLayer, _vec3(0.f, -3.4f, -0.5f), m_pTransform->GetChild(1)), E_FAIL);
			FAILED_CHECK_RETURN(FACTORY<CBoss2Foot>::Create(L"Boss2Foot", pStageLayer, _vec3(2.1f, -2.8f, -0.5f), m_pTransform->GetChild(1)), E_FAIL);
			FAILED_CHECK_RETURN(FACTORY<CBoss2Foot>::Create(L"Boss2Foot", pStageLayer, _vec3(4.1f, -2.8f, -0.5f), m_pTransform->GetChild(1)), E_FAIL);
			FAILED_CHECK_RETURN(FACTORY<CBoss2Body>::Create(L"Boss2Body", pStageLayer, _vec3(0.0f, 0.0f, 0.0f), m_pTransform->GetChild(1)), E_FAIL);
			m_pTransform->GetChild(1)->GetChild(4)->m_vScale = _vec3(3.8f, 3.8f, 3.8f);
			dynamic_cast<CBoss2Body*>(m_pTransform->GetChild(1)->GetChild(4)->m_pGameObject)->SetRotAngle(30.f, 120.f);

			//5~8
			FAILED_CHECK_RETURN(FACTORY<CBoss2JointSpot>::Create(L"Boss2JointSpot", pStageLayer, _vec3(-1.0f, 0.0f, 0.3f), m_pTransform->GetChild(1)), E_FAIL);
			FAILED_CHECK_RETURN(FACTORY<CBoss2JointSpot>::Create(L"Boss2JointSpot", pStageLayer, _vec3(-1.0f, 0.0f, -0.5f), m_pTransform->GetChild(1)), E_FAIL);
			FAILED_CHECK_RETURN(FACTORY<CBoss2JointSpot>::Create(L"Boss2JointSpot", pStageLayer, _vec3(1.0f, 0.0f, 0.2f), m_pTransform->GetChild(1)), E_FAIL);
			FAILED_CHECK_RETURN(FACTORY<CBoss2JointSpot>::Create(L"Boss2JointSpot", pStageLayer, _vec3(1.0f, 0.0f, -0.2f), m_pTransform->GetChild(1)), E_FAIL);

			//9~12
			FAILED_CHECK_RETURN(FACTORY<CBoss2Chain>::Create(L"Boss2Chain", pStageLayer, _vec3(0.f, 0.f, 0.3f), m_pTransform->GetChild(1)), E_FAIL);
			FAILED_CHECK_RETURN(FACTORY<CBoss2Chain>::Create(L"Boss2Chain", pStageLayer, _vec3(0.f, 0.f, -0.5f), m_pTransform->GetChild(1)), E_FAIL);
			FAILED_CHECK_RETURN(FACTORY<CBoss2Chain>::Create(L"Boss2Chain", pStageLayer, _vec3(0.f, 0.f, 0.2f), m_pTransform->GetChild(1)), E_FAIL);
			FAILED_CHECK_RETURN(FACTORY<CBoss2Chain>::Create(L"Boss2Chain", pStageLayer, _vec3(0.f, 0.f, -0.2f), m_pTransform->GetChild(1)), E_FAIL);

			dynamic_cast<CBoss2Chain*>(m_pTransform->GetChild(1)->GetChild(9)->m_pGameObject)->Set_Joint(m_pTransform->GetChild(1)->GetChild(0), m_pTransform->GetChild(1)->GetChild(5));
			dynamic_cast<CBoss2Chain*>(m_pTransform->GetChild(1)->GetChild(10)->m_pGameObject)->Set_Joint(m_pTransform->GetChild(1)->GetChild(1), m_pTransform->GetChild(1)->GetChild(6));
			dynamic_cast<CBoss2Chain*>(m_pTransform->GetChild(1)->GetChild(11)->m_pGameObject)->Set_Joint(m_pTransform->GetChild(1)->GetChild(2), m_pTransform->GetChild(1)->GetChild(7));
			dynamic_cast<CBoss2Chain*>(m_pTransform->GetChild(1)->GetChild(12)->m_pGameObject)->Set_Joint(m_pTransform->GetChild(1)->GetChild(3), m_pTransform->GetChild(1)->GetChild(8));
		
			FAILED_CHECK_RETURN(FACTORY<CBoss2TailBody>::Create(L"Boss2TailBody", pStageLayer, _vec3(0.0f, 0.0f, 0.2f), m_pTransform->GetChild(1)), E_FAIL);
			m_pTransform->GetChild(1)->GetChild(13)->m_vScale = _vec3(0.0f, 0.0f, 0.0f);
			m_pTransform->GetChild(1)->GetChild(13)->m_vAngle = _vec3(0, 0, D3DXToRadian(0.f));
		}

		
    
		//idle ¾Ö´Ï¸ÞÀÌ¼ÇÀ» ¸¸µé¾îº¸ÀÚ.
		AnimClip* clip = new AnimClip();
		{
			//Face
			clip->parts.push_back(m_pTransform->GetChild(0)->GetChild(0));
			clip->parts.push_back(m_pTransform->GetChild(0)->GetChild(1));
			clip->parts.push_back(m_pTransform->GetChild(0)->GetChild(2));
			clip->parts.push_back(m_pTransform->GetChild(0)->GetChild(3));
			clip->parts.push_back(m_pTransform->GetChild(0)->GetChild(4));
			clip->parts.push_back(m_pTransform->GetChild(0)->GetChild(5));
			clip->parts.push_back(m_pTransform->GetChild(0)->GetChild(6));
			clip->source.resize(7);

			// Face
			{
				clip->source[0].push_back(
					ANIMINFO{
					_vec3(0,0,-0.2f),//trans
					_vec3(0,0,0),//rotation
					_vec3(2.0f,1.5f,2.0f),//scale
					0.5f,//tilltime
					0.f//actionTime
				});

				clip->source[0].push_back(
					ANIMINFO{
					_vec3(-1,0,-0.2f),//trans
					_vec3(0,0,0),//rotation
					_vec3(2.0f,1.5f,2.0f),//scale
					0.5f,//tilltime
					0.5f//actionTime
				});

				clip->source[0].push_back(
					ANIMINFO{
					_vec3(-2,0,-0.2f),//trans
					_vec3(0,0,0),//rotation
					_vec3(2.0f,1.5f,2.0f),//scale
					0.5f,//tilltime
					1.f//actionTime
				});

				clip->source[0].push_back(
					ANIMINFO{
					_vec3(-1,0,-0.2f),//trans
					_vec3(0,0,0),//rotation
					_vec3(2.0f,1.5f,2.0f),//scale
					0.5f,//tilltime
					1.5f//actionTime
				});
			}
			
			//Jaw
			{
				clip->source[1].push_back(
					ANIMINFO{
					_vec3(0,-1,-0.1f),//trans
					_vec3(0,0,0),//rotation
					_vec3(1.5f,2.f,2.f),//scale
					0.5f,//tilltime
					0.f//actionTime
				});
				clip->source[1].push_back(
					ANIMINFO{
					_vec3(-1,-1,-0.1f),//trans
					_vec3(0,0,0),//rotation
					_vec3(1.5f,2.f,2.f),//scale
					0.5f,//tilltime
					0.5f//actionTime
				});

				clip->source[1].push_back(
					ANIMINFO{
					_vec3(-2,-1,-0.1f),//trans
					_vec3(0,0,0),//rotation
					_vec3(1.5f,2.f,2.f),//scale
					0.5f,//tilltime
					1.f//actionTime
				});
				clip->source[1].push_back(
					ANIMINFO{
					_vec3(-1,-1,-0.1f),//trans
					_vec3(0,0,0),//rotation
					_vec3(1.5f,2.f,2.f),//scale
					0.5f,//tilltime
					1.5f//actionTime
				});
			}

			//Eye_L
			{
				clip->source[2].push_back(
					ANIMINFO{
					_vec3(-1.1f,0.6f,-0.3f),//trans
					_vec3(0,0,0),//rotation
					_vec3(0.7f,0.7f,0.7f),//scale
					0.5f,//tilltime
					0.f//actionTime
				});

				clip->source[2].push_back(
					ANIMINFO{
					_vec3(-2.1f,0.6f,-0.3f),//trans
					_vec3(0,0,0),//rotation
					_vec3(1.0f,1.0f,1.0f),//scale
					0.5f,//tilltime
					0.5f//actionTime
				});
				clip->source[2].push_back(
					ANIMINFO{
					_vec3(-3.1f,0.6f,-0.3f),//trans
					_vec3(0,0,0),//rotation
					_vec3(1.0f,1.0f,1.0f),//scale
					0.5f,//tilltime
					1.f//actionTime
				});
				clip->source[2].push_back(
					ANIMINFO{
					_vec3(-2.1f,0.6f,-0.3f),//trans
					_vec3(0,0,0),//rotation
					_vec3(0.7f,0.7f,0.7f),//scale
					0.5f,//tilltime
					1.5f//actionTime
				});
			}
			
			//Eye_R
			{
				clip->source[3].push_back(
					ANIMINFO{
					_vec3(0.7f, 0.6f, -0.3f),//trans
					_vec3(0,0,0),//rotation
					_vec3(0.7f,0.7f,0.7f),//scale
					0.5f,//tilltime
					0.f//actionTime
				});
				clip->source[3].push_back(
					ANIMINFO{
					_vec3(-0.3f, 0.6f, -0.3f),//trans
					_vec3(0,0,0),//rotation
					_vec3(1.0f,1.0f,1.0f),//scale
					0.5f,//tilltime
					0.5f//actionTime
				});
				clip->source[3].push_back(
					ANIMINFO{
					_vec3(-1.3f, 0.6f, -0.3f),//trans
					_vec3(0,0,0),//rotation
					_vec3(1.0f,1.0f,1.0f),//scale
					0.5f,//tilltime
					1.f//actionTime
				});
				clip->source[3].push_back(
					ANIMINFO{
					_vec3(-0.3f, 0.6f, -0.3f),//trans
					_vec3(0,0,0),//rotation
					_vec3(0.7f,0.7f,0.7f),//scale
					0.5f,//tilltime
					1.5f//actionTime
				});
			}
			
			//Nose
			{
				clip->source[4].push_back(
					ANIMINFO{
					_vec3(-0.6f,0.2f,-0.7f),//trans
					_vec3(0,0,0),//rotation
					_vec3(0.5f,0.5f,0.5f),//scale
					0.5f,//tilltime
					0.f//actionTime
				});
				clip->source[4].push_back(
					ANIMINFO{
					_vec3(-1.0f,0.2f,-0.7f),//trans
					_vec3(0,0,0),//rotation
					_vec3(0.5f,0.5f,0.5f),//scale
					0.5f,//tilltime
					0.5f//actionTime
				});
				clip->source[4].push_back(
					ANIMINFO{
					_vec3(-2.0f,0.2f,-0.7f),//trans
					_vec3(0,0,0),//rotation
					_vec3(0.5f,0.5f,0.5f),//scale
					0.5f,//tilltime
					1.f//actionTime
				});
				clip->source[4].push_back(
					ANIMINFO{
					_vec3(-1.0f,0.2f,-0.7f),//trans
					_vec3(0,0,0),//rotation
					_vec3(0.5f,0.5f,0.5f),//scale
					0.5f,//tilltime
					1.5f//actionTime
				});
			}

			//EyeBrow_L
			{
				clip->source[5].push_back(
					ANIMINFO{
					_vec3(-1.1f,1.4f,-0.2f),//trans
					_vec3(0,0,0),//rotation
					_vec3(0.7f,0.7f,0.7f),//scale
					0.5f,//tilltime
					0.f//actionTime
				});
				clip->source[5].push_back(
					ANIMINFO{
					_vec3(-2.1f,1.8f,-0.2f),//trans
					_vec3(0,0,0),//rotation
					_vec3(0.7f,0.7f,0.7f),//scale
					0.5f,//tilltime
					0.5f//actionTime
				});
				clip->source[5].push_back(
					ANIMINFO{
					_vec3(-3.1f,1.8f,-0.2f),//trans
					_vec3(0,0,0),//rotation
					_vec3(0.7f,0.7f,0.7f),//scale
					0.5f,//tilltime
					1.f//actionTime
				});
				clip->source[5].push_back(
					ANIMINFO{
					_vec3(-2.1f,1.4f,-0.2f),//trans
					_vec3(0,0,0),//rotation
					_vec3(0.7f,0.7f,0.7f),//scale
					0.5f,//tilltime
					1.5f//actionTime
				});
			}

			//EyeBrow_R
			{
				clip->source[6].push_back(
				ANIMINFO{
				_vec3(0.7f,1.4f,-0.2f),//trans
				_vec3(0,0,0),//rotation
				_vec3(0.7f,0.7f,0.7f),//scale
				0.5f,//tilltime
				0.f//actionTime
			});
			clip->source[6].push_back(
				ANIMINFO{
				_vec3(-0.3f,1.9f,-0.2f),//trans
				_vec3(0,0,0),//rotation
				_vec3(0.7f,0.7f,0.7f),//scale
				0.5f,//tilltime
				0.5f//actionTime
			});
			clip->source[6].push_back(
				ANIMINFO{
				_vec3(-1.3f,1.9f,-0.2f),//trans
				_vec3(0,0,0),//rotation
				_vec3(0.7f,0.7f,0.7f),//scale
				0.5f,//tilltime
				1.f//actionTime
			});
			clip->source[6].push_back(
				ANIMINFO{
				_vec3(-0.3f,1.4f,-0.2f),//trans
				_vec3(0,0,0),//rotation
				_vec3(0.7f,0.7f,0.7f),//scale
				0.5f,//tilltime
				1.5f//actionTime
			});
			}

			clip->TotalTime = 2;
			clip->Useloop = true;
		}
		m_pAnimation_Face->AddClip(L"Idle", clip);

		//Scream ¾Ö´Ï¸ÞÀÌ¼ÇÀ» ¸¸µé¾îº¸ÀÚ.
		clip = new AnimClip();
		{
			//Face
			clip->parts.push_back(m_pTransform->GetChild(0)->GetChild(0));
			clip->parts.push_back(m_pTransform->GetChild(0)->GetChild(1));
			clip->parts.push_back(m_pTransform->GetChild(0)->GetChild(2));
			clip->parts.push_back(m_pTransform->GetChild(0)->GetChild(3));
			clip->parts.push_back(m_pTransform->GetChild(0)->GetChild(4));
			clip->parts.push_back(m_pTransform->GetChild(0)->GetChild(5));
			clip->parts.push_back(m_pTransform->GetChild(0)->GetChild(6));
			clip->source.resize(7);

			// Face
			{
				clip->source[0].push_back(
					ANIMINFO{
					_vec3(-1,0,-0.2f) + _vec3(1,0,0),//trans
					_vec3(0,0,0),//rotation
					_vec3(2.0f,1.5f,2.0f)*1.3f,//scale
					0.5f,//tilltime
					0.f//actionTime
				});

				clip->source[0].push_back(
					ANIMINFO{
					_vec3(-1,0,-0.2f),//trans
					_vec3(0,0,0),//rotation
					_vec3(2.0f,1.5f,2.0f)*1.2f,//scale
					0.5f,//tilltime
					0.5f//actionTime
				});

				clip->source[0].push_back(
					ANIMINFO{
					_vec3(-1,0,-0.2f) + _vec3(-1,0,0),//trans
					_vec3(0,0,0),//rotation
					_vec3(2.0f,1.5f,2.0f)*1.3f,//scale
					0.5f,//tilltime
					1.f//actionTime
				});

				clip->source[0].push_back(
					ANIMINFO{
					_vec3(-1,0,-0.2f),//trans
					_vec3(0,0,0),//rotation
					_vec3(2.0f,1.5f,2.0f)*1.2f,//scale
					0.5f,//tilltime
					1.5f//actionTime
				});
			}

			//Jaw
			{
				clip->source[1].push_back(
					ANIMINFO{
					_vec3(-1,-1,-0.1f) + _vec3(1,-1,0),//trans
					_vec3(0,0,0),//rotation
					_vec3(1.5f,2.f,2.f)*1.2f,//scale
					0.2f,//tilltime
					0.f//actionTime
				});
				clip->source[1].push_back(
					ANIMINFO{
					_vec3(-1,-1,-0.1f) + _vec3(0,-3,0),//trans
					_vec3(0,0,0),//rotation
					_vec3(1.5f,2.f,2.f)*1.3f,//scale
					0.2f,//tilltime
					0.2f//actionTime
				});
				clip->source[1].push_back(
					ANIMINFO{
					_vec3(-1,-1,-0.1f) + _vec3(-1,-1,0),//trans
					_vec3(0,0,0),//rotation
					_vec3(1.5f,2.f,2.f)*1.2f,//scale
					0.2f,//tilltime
					0.4f//actionTime
				});
				clip->source[1].push_back(
					ANIMINFO{
					_vec3(-1,-1,-0.1f) + _vec3(0,-3,0),//trans
					_vec3(0,0,0),//rotation
					_vec3(1.5f,2.f,2.f)*1.3f,//scale
					0.2f,//tilltime
					0.6f//actionTime
				});
				clip->source[1].push_back(
					ANIMINFO{
					_vec3(-1,-1,-0.1f) + _vec3(1,-1,0),//trans
					_vec3(0,0,0),//rotation
					_vec3(1.5f,2.f,2.f)*1.2f,//scale
					0.2f,//tilltime
					0.8f//actionTime
				});
				clip->source[1].push_back(
					ANIMINFO{
					_vec3(-1,-1,-0.1f) + _vec3(0,-3,0),//trans
					_vec3(0,0,0),//rotation
					_vec3(1.5f,2.f,2.f)*1.3f,//scale
					0.2f,//tilltime
					1.0f//actionTime
				});
				clip->source[1].push_back(
					ANIMINFO{
					_vec3(-1,-1,-0.1f) + _vec3(-1,-1,0),//trans
					_vec3(0,0,0),//rotation
					_vec3(1.5f,2.f,2.f)*1.2f,//scale
					0.2f,//tilltime
					1.2f//actionTime
				});
				clip->source[1].push_back(
					ANIMINFO{
					_vec3(-1,-1,-0.1f) + _vec3(0,-3,0),//trans
					_vec3(0,0,0),//rotation
					_vec3(1.5f,2.f,2.f)*1.3f,//scale
					0.2f,//tilltime
					1.4f//actionTime
				});
				clip->source[1].push_back(
					ANIMINFO{
					_vec3(-1,-1,-0.1f) + _vec3(1,-1,0),//trans
					_vec3(0,0,0),//rotation
					_vec3(1.5f,2.f,2.f)*1.2f,//scale
					0.2f,//tilltime
					1.6f//actionTime
				});
				clip->source[1].push_back(
					ANIMINFO{
					_vec3(-1,-1,-0.1f) + _vec3(0,-3,0),//trans
					_vec3(0,0,0),//rotation
					_vec3(1.5f,2.f,2.f)*1.3f,//scale
					0.2f,//tilltime
					1.8f//actionTime
				});
			}

			//Eye_L
			{
				clip->source[2].push_back(
					ANIMINFO{
					_vec3(-2.1f,0.6f,-0.3f) + _vec3(1,0,0),//trans
					_vec3(0,0,0),//rotation
					_vec3(0.7f,0.7f,0.7f)*1.3f,//scale
					0.5f,//tilltime
					0.f//actionTime
				});

				clip->source[2].push_back(
					ANIMINFO{
					_vec3(-2.1f,0.6f,-0.3f),//trans
					_vec3(0,0,0),//rotation
					_vec3(1.0f,1.0f,1.0f)*1.2f,//scale
					0.5f,//tilltime
					0.5f//actionTime
				});
				clip->source[2].push_back(
					ANIMINFO{
					_vec3(-2.1f,0.6f,-0.3f) + _vec3(-1,0,0),//trans
					_vec3(0,0,0),//rotation
					_vec3(1.0f,1.0f,1.0f)*1.3f,//scale
					0.5f,//tilltime
					1.f//actionTime
				});
				clip->source[2].push_back(
					ANIMINFO{
					_vec3(-2.1f,0.6f,-0.3f),//trans
					_vec3(0,0,0),//rotation
					_vec3(0.7f,0.7f,0.7f)*1.2f,//scale
					0.5f,//tilltime
					1.5f//actionTime
				});
			}

			//Eye_R
			{
				clip->source[3].push_back(
					ANIMINFO{
					_vec3(0.7f, 0.6f, -0.3f),//trans
					_vec3(0,0,0),//rotation
					_vec3(0.7f,0.7f,0.7f)*1.2f,//scale
					0.5f,//tilltime
					0.f//actionTime
				});
				clip->source[3].push_back(
					ANIMINFO{
					_vec3(-0.3f, 0.6f, -0.3f),//trans
					_vec3(0,0,0),//rotation
					_vec3(1.0f,1.0f,1.0f)*1.3f,//scale
					0.5f,//tilltime
					0.5f//actionTime
				});
				clip->source[3].push_back(
					ANIMINFO{
					_vec3(-1.3f, 0.6f, -0.3f),//trans
					_vec3(0,0,0),//rotation
					_vec3(1.0f,1.0f,1.0f)*1.2f,//scale
					0.5f,//tilltime
					1.f//actionTime
				});
				clip->source[3].push_back(
					ANIMINFO{
					_vec3(-0.3f, 0.6f, -0.3f),//trans
					_vec3(0,0,0),//rotation
					_vec3(0.7f,0.7f,0.7f)*1.3f,//scale
					0.5f,//tilltime
					1.5f//actionTime
				});
			}

			//Nose
			{
				clip->source[4].push_back(
					ANIMINFO{
					_vec3(-0.6f,0.2f,-0.7f),//trans
					_vec3(0,0,0),//rotation
					_vec3(0.5f,0.5f,0.5f)*1.2f,//scale
					0.5f,//tilltime
					0.f//actionTime
				});
				clip->source[4].push_back(
					ANIMINFO{
					_vec3(-1.0f,0.2f,-0.7f),//trans
					_vec3(0,0,0),//rotation
					_vec3(0.5f,0.5f,0.5f)*1.3f,//scale
					0.5f,//tilltime
					0.5f//actionTime
				});
				clip->source[4].push_back(
					ANIMINFO{
					_vec3(-2.0f,0.2f,-0.7f),//trans
					_vec3(0,0,0),//rotation
					_vec3(0.5f,0.5f,0.5f)*1.2f,//scale
					0.5f,//tilltime
					1.f//actionTime
				});
				clip->source[4].push_back(
					ANIMINFO{
					_vec3(-1.0f,0.2f,-0.7f),//trans
					_vec3(0,0,0),//rotation
					_vec3(0.5f,0.5f,0.5f)*1.3f,//scale
					0.5f,//tilltime
					1.5f//actionTime
				});
			}

			//EyeBrow_L
			{
				clip->source[5].push_back(
					ANIMINFO{
					_vec3(-1.1f,1.4f,-0.2f),//trans
					_vec3(0,0,0),//rotation
					_vec3(0.7f,0.7f,0.7f)*1.3f,//scale
					0.5f,//tilltime
					0.f//actionTime
				});
				clip->source[5].push_back(
					ANIMINFO{
					_vec3(-2.1f,1.8f,-0.2f),//trans
					_vec3(0,0,0),//rotation
					_vec3(0.7f,0.7f,0.7f)*1.2f,//scale
					0.5f,//tilltime
					0.5f//actionTime
				});
				clip->source[5].push_back(
					ANIMINFO{
					_vec3(-3.1f,1.8f,-0.2f),//trans
					_vec3(0,0,0),//rotation
					_vec3(0.7f,0.7f,0.7f)*1.3f,//scale
					0.5f,//tilltime
					1.f//actionTime
				});
				clip->source[5].push_back(
					ANIMINFO{
					_vec3(-2.1f,1.4f,-0.2f),//trans
					_vec3(0,0,0),//rotation
					_vec3(0.7f,0.7f,0.7f)*1.2f,//scale
					0.5f,//tilltime
					1.5f//actionTime
				});
			}

			//EyeBrow_R
			{
				clip->source[6].push_back(
					ANIMINFO{
					_vec3(0.7f,1.4f,-0.2f),//trans
					_vec3(0,0,0),//rotation
					_vec3(0.7f,0.7f,0.7f)*1.2f,//scale
					0.5f,//tilltime
					0.f//actionTime
				});
				clip->source[6].push_back(
					ANIMINFO{
					_vec3(-0.3f,1.9f,-0.2f),//trans
					_vec3(0,0,0),//rotation
					_vec3(0.7f,0.7f,0.7f)*1.3f,//scale
					0.5f,//tilltime
					0.5f//actionTime
				});
				clip->source[6].push_back(
					ANIMINFO{
					_vec3(-1.3f,1.9f,-0.2f),//trans
					_vec3(0,0,0),//rotation
					_vec3(0.7f,0.7f,0.7f)*1.2f,//scale
					0.5f,//tilltime
					1.f//actionTime
				});
				clip->source[6].push_back(
					ANIMINFO{
					_vec3(-0.3f,1.4f,-0.2f),//trans
					_vec3(0,0,0),//rotation
					_vec3(0.7f,0.7f,0.7f)*1.3f,//scale
					0.5f,//tilltime
					1.5f//actionTime
				});
			}

			clip->TotalTime = 2;
			clip->Useloop = true;
		}
		m_pAnimation_Face->AddClip(L"Scream", clip);
		m_pAnimation_Face->SetAnimation(L"Idle");

		clip = new AnimClip();
		{
			clip->parts.push_back(m_pTransform->GetChild(1)->GetChild(0));
			clip->parts.push_back(m_pTransform->GetChild(1)->GetChild(1));
			clip->parts.push_back(m_pTransform->GetChild(1)->GetChild(2));
			clip->parts.push_back(m_pTransform->GetChild(1)->GetChild(3));
			clip->parts.push_back(m_pTransform->GetChild(1));
			clip->parts.push_back(m_pTransform->GetChild(0));
			clip->parts.push_back(m_pTransform->GetChild(1)->GetChild(5));
			clip->parts.push_back(m_pTransform->GetChild(1)->GetChild(6));
			clip->parts.push_back(m_pTransform->GetChild(1)->GetChild(7)); //팔 조인트
			clip->parts.push_back(m_pTransform->GetChild(1)->GetChild(8)); //팔 조인트
			clip->source.resize(10);
			clip->TotalTime = 2.0f;
			clip->Useloop = true;
			//LeftFront
			{
				clip->source[0].push_back(
					ANIMINFO{
					_vec3(-2.1f, -3.4f, 0.3f) + _vec3(-1,-2,0),//trans
					_vec3(0,0,0) + _vec3(0,0,0),//rotation
					_vec3(0.f,0.f,0.f),//scale
					0.2f,//tilltime
					0.f//actionTime
				});

				clip->source[0].push_back(
					ANIMINFO{
					_vec3(-2.1f, -3.4f, 0.3f),//trans
					_vec3(0,0,0) + _vec3(0,0,0),//rotation
					_vec3(0.f,0.f,0.f),//scale
					0.3f,//tilltime
					0.2f//actionTime
				});

				clip->source[0].push_back(
					ANIMINFO{
					_vec3(0,0,0),//trans
					_vec3(0,0,0) + _vec3(0,0,0),//rotation
					_vec3(0.f,0.f,0.f),//scale
					0.5f,//tilltime
					0.5f//actionTime
				});

				clip->source[0].push_back(
					ANIMINFO{
					_vec3(-2.1f, -3.4f, 0.3f) + _vec3(1,-2,0),//trans
					_vec3(0,0,0) + _vec3(0,0,0),//rotation
					_vec3(0.f,0.f,0.f),//scale
					0.5f,//tilltime
					1.f//actionTime
				});
				clip->source[0].push_back(
					ANIMINFO{
					_vec3(-2.1f, -3.4f, 0.3f),//trans
					_vec3(0,0,0) + _vec3(0,0,0),//rotation
					_vec3(0.f,0.f,0.f),//scale
					0.5f,//tilltime
					1.5f//actionTime
				});
			}

			//Rightfront
			{
				clip->source[1].push_back(
					ANIMINFO{
					_vec3(0.f, -3.4f, -0.5f) + _vec3(-1,-2,0),//trans
					_vec3(0,0,0) + _vec3(0,0,0),//rotation
					_vec3(0.f,0.f,0.f),//scale
					0.2f,//tilltime
					0.f//actionTime
				});
				clip->source[1].push_back(
					ANIMINFO{
					_vec3(0.f, -3.4f, -0.5f),//trans
					_vec3(0,0,0) + _vec3(0,0,0),//rotation
					_vec3(0.f,0.f,0.f),//scale
					0.3f,//tilltime
					0.2f//actionTime
				});
				clip->source[1].push_back(
					ANIMINFO{
					_vec3(0.f, -3.4f, -0.5f)+_vec3(0,-2,0),//trans
					_vec3(0,0,0) + _vec3(0,0,0),//rotation
					_vec3(0.f,0.f,0.f),//scale
					0.5f,//tilltime
					0.5f//actionTime
				});
				clip->source[1].push_back(
					ANIMINFO{
					_vec3(0.f, -3.4f, -0.5f) + _vec3(1,-2,0),//trans
					_vec3(0,0,0) + _vec3(0,0,0),//rotation
					_vec3(0.f,0.f,0.f),//scale
					0.5f,//tilltime
					1.0f//actionTime
				});
				clip->source[1].push_back(
					ANIMINFO{
					_vec3(0.f, -3.4f, -0.5f) + _vec3(0,-2,0),//trans
					_vec3(0,0,0) + _vec3(0,0,0),//rotation
					_vec3(0.f,0.f,0.f),//scale
					0.5f,//tilltime
					1.5f//actionTime
				});
			}
			//LeftBack
			{
				clip->source[2].push_back(
					ANIMINFO{
					_vec3(2.1f, -2.8f, 0.2f) + _vec3(-1,-2,0),//trans
					_vec3(0,0,0) + _vec3(0,0,0),//rotation
					_vec3(0.f,0.f,0.f),//scale
					0.2f,//tilltime
					0.f//actionTime
				});
				clip->source[2].push_back(
					ANIMINFO{
					_vec3(2.1f, -2.8f, 0.2f) + _vec3(0,-2,0),//trans
					_vec3(0,0,0) + _vec3(0,0,0),//rotation
					_vec3(0.f,0.f,0.f),//scale
					0.5f,//tilltime
					0.5f//actionTime
				});
				clip->source[2].push_back(
					ANIMINFO{
					_vec3(2.1f, -2.8f, 0.2f) + _vec3(1,-2,0),//trans
					_vec3(0,0,0) + _vec3(0,0,0),//rotation
					_vec3(0.f,0.f,0.f),//scale
					0.5f,//tilltime
					1.0f//actionTime
				});
				clip->source[2].push_back(
					ANIMINFO{
					_vec3(2.1f, -2.8f, 0.2f) + _vec3(0,-2,0),//trans
					_vec3(0,0,0) + _vec3(0,0,0),//rotation
					_vec3(0.f,0.f,0.f),//scale
					0.5f,//tilltime
					1.5f//actionTime
				});
			}
			//RightBack
			{
				clip->source[3].push_back(
					ANIMINFO{
					_vec3(4.1f, -2.8f, -0.2f) + _vec3(-1,-2,0),//trans
					_vec3(0,0,0) + _vec3(0,0,0),//rotation
					_vec3(0.f,0.f,0.f),//scale
					0.2f,//tilltime
					0.f//actionTime
				});
				clip->source[3].push_back(
					ANIMINFO{
					_vec3(4.1f, -2.8f, -0.2f) + _vec3(0,-2,0),//trans
					_vec3(0,0,0) + _vec3(0,0,0),//rotation
					_vec3(0.f,0.f,0.f),//scale
					0.5f,//tilltime
					0.5f//actionTime
				});
				clip->source[3].push_back(
					ANIMINFO{
					_vec3(4.1f, -2.8f,-0.2f) + _vec3(1,-2,0),//trans
					_vec3(0,0,0) + _vec3(0,0,0),//rotation
					_vec3(0.f,0.f,0.f),//scale
					0.5f,//tilltime
					1.f//actionTime
				});
				clip->source[3].push_back(
					ANIMINFO{
					_vec3(4.1f, -2.8f,-0.2f) + _vec3(0,-2,0),//trans
					_vec3(0,0,0) + _vec3(0,0,0),//rotation
					_vec3(0.f,0.f,0.f),//scale
					0.5f,//tilltime
					1.5f//actionTime
				});
			}
			//BackBody
			{
				clip->source[4].push_back(
					ANIMINFO{
					_vec3(3.f, 0.3f, 0.1f) + _vec3(0,1,0),//trans
					_vec3(0,0,0) + _vec3(D3DXToRadian(1),0,0),//rotation
					_vec3(1.f,1.f,1.f),//scale
					1.f,//tilltime
					0.f//actionTime
				});

				clip->source[4].push_back(
					ANIMINFO{
					_vec3(3.f, 0.3f, 0.1f) + _vec3(0,-1,0),//trans
					_vec3(0,0,0) + _vec3(0,0,0),//rotation
					_vec3(0.f,0.f,0.f),//scale
					1.f,//tilltime
					1.0f//actionTime
				});
			}
			//FrontBody
			{
				clip->source[5].push_back(
					ANIMINFO{
					_vec3(0.f, 0.f, 0.f) + _vec3(0,0,0),//trans
					_vec3(0,0,0) + _vec3(D3DXToRadian(1),0,0),//rotation
					_vec3(1.f,1.f,1.f),//scale
					1.0f,//tilltime
					0.f//actionTime
				});
				clip->source[5].push_back(
					ANIMINFO{
					_vec3(0.f, 0.f, 0.f) + _vec3(0,-0.5f,0),//trans
					_vec3(0,0,0) + _vec3(0,0,0),//rotation
					_vec3(0.f,0.f,0.f),//scale
					1.f,//tilltime
					1.f//actionTime
				});
			}
			//joint
			{
				clip->source[6].push_back(
					ANIMINFO{
					_vec3(-1.0f, 0.0f, 0.3f) + _vec3(0,0,0),//trans
					_vec3(0,0,0) + _vec3(0,0,0),//rotation
					_vec3(0.f,0.f,0.f),//scale
					0.2f,//tilltime
					0.f//actionTime
				});
			}
			//joint
			{
				clip->source[7].push_back(
					ANIMINFO{
					_vec3(-1.0f, 0.0f, -0.5f) + _vec3(0,0,0),//trans
					_vec3(0,0,0) + _vec3(0,0,0),//rotation
					_vec3(0.f,0.f,0.f),//scale
					0.2f,//tilltime
					0.f//actionTime
				});
			}
			//joint
			{
				clip->source[8].push_back(
					ANIMINFO{
					_vec3(1.0f, 0.0f, 0.3f) + _vec3(0,0,0),//trans
					_vec3(0,0,0) + _vec3(0,0,0),//rotation
					_vec3(0.f,0.f,0.f),//scale
					0.2f,//tilltime
					0.f//actionTime
				});
			}
			//joint
			{
				clip->source[9].push_back(
					ANIMINFO{
					_vec3(1.0f, 0.0f, -0.5f) + _vec3(0,0,0),//trans
					_vec3(0,0,0) + _vec3(0,0,0),//rotation
					_vec3(0.f,0.f,0.f),//scale
					0.2f,//tilltime
					0.f//actionTime
				});
			}
		}
		m_pAnimation_Body->AddClip(L"Idle", clip);

		clip = new AnimClip();
		{
			clip->parts.push_back(m_pTransform->GetChild(1)->GetChild(0));
			clip->parts.push_back(m_pTransform->GetChild(1)->GetChild(1));
			clip->parts.push_back(m_pTransform->GetChild(1)->GetChild(2));
			clip->parts.push_back(m_pTransform->GetChild(1)->GetChild(3));
			clip->parts.push_back(m_pTransform->GetChild(1));
			clip->parts.push_back(m_pTransform->GetChild(0));
			clip->parts.push_back(m_pTransform->GetChild(1)->GetChild(5));
			clip->parts.push_back(m_pTransform->GetChild(1)->GetChild(6));
			clip->source.resize(8);
			clip->TotalTime = 2.9f;
			clip->Useloop = true;
			//LeftFront
			{
				clip->source[0].push_back(
					ANIMINFO{
					_vec3(-2.1f, -3.4f, 0.3f) + _vec3(-5,3,0),//trans
					_vec3(0,0,0) + _vec3(0,0,0),//rotation
					_vec3(0.f,0.f,0.f),//scale
					0.5f,//tilltime
					0.f//actionTime
				});

				clip->source[0].push_back(
					ANIMINFO{
					_vec3(-2.1f, -3.4f, 0.3f),//trans
					_vec3(0,0,0) + _vec3(0,0,0),//rotation
					_vec3(0.f,0.f,0.f),//scale
					1.5f,//tilltime
					0.5f//actionTime
				});

				clip->source[0].push_back(
					ANIMINFO{
					_vec3(-2.1f, -3.4f, 0.3f) + _vec3(1,-4,0),//trans
					_vec3(0,0,0) + _vec3(0,0,0),//rotation
					_vec3(0.f,0.f,0.f),//scale
					0.5f,//tilltime
					2.f//actionTime
				});
			}
			//Rightfront
			{
				clip->source[1].push_back(
					ANIMINFO{
					_vec3(0.f, -3.4f, -0.5f) + _vec3(-5,3,0),//trans
					_vec3(0,0,0) + _vec3(0,0,0),//rotation
					_vec3(0.f,0.f,0.f),//scale
					0.5f,//tilltime
					0.f//actionTime
				});
				clip->source[1].push_back(
					ANIMINFO{
					_vec3(0.f, -3.4f, -0.5f),//trans
					_vec3(0,0,0) + _vec3(0,0,0),//rotation
					_vec3(0.f,0.f,0.f),//scale
					1.5f,//tilltime
					0.5f//actionTime
				});
				clip->source[1].push_back(
					ANIMINFO{
					_vec3(0.f, -3.4f, -0.5f) + _vec3(1,-4,0),//trans
					_vec3(0,0,0) + _vec3(0,0,0),//rotation
					_vec3(0.f,0.f,0.f),//scale
					0.5f,//tilltime
					2.0f//actionTime
				});
			}
			//LeftBack
			{
				clip->source[2].push_back(
					ANIMINFO{
					_vec3(2.1f, -2.8f, 0.2f) + _vec3(-2,-6,0),//trans
					_vec3(0,0,0) + _vec3(0,0,0),//rotation
					_vec3(0.f,0.f,0.f),//scale
					0.2f,//tilltime
					0.f//actionTime
				});
				clip->source[2].push_back(
					ANIMINFO{
					_vec3(2.1f, -2.8f, 0.2f),//trans
					_vec3(0,0,0) + _vec3(0,0,0),//rotation
					_vec3(0.f,0.f,0.f),//scale
					0.3f,//tilltime
					0.2f//actionTime
				});
				clip->source[2].push_back(
					ANIMINFO{
					_vec3(2.1f, -2.8f, 0.2f),//trans
					_vec3(0,0,0) + _vec3(0,0,0),//rotation
					_vec3(0.f,0.f,0.f),//scale
					1.5f,//tilltime
					0.5f//actionTime
				});
				clip->source[2].push_back(
					ANIMINFO{
					_vec3(2.1f, -2.8f, 0.2f) + _vec3(3,5,0),//trans
					_vec3(0,0,0) + _vec3(0,0,0),//rotation
					_vec3(0.f,0.f,0.f),//scale
					0.5f,//tilltime
					2.0f//actionTime
				});
			}
			//RightBack
			{
				clip->source[3].push_back(
					ANIMINFO{
					_vec3(4.1f, -2.8f, -0.2f) + _vec3(2,-6,0),//trans
					_vec3(0,0,0) + _vec3(0,0,0),//rotation
					_vec3(0.f,0.f,0.f),//scale
					0.2f,//tilltime
					0.f//actionTime
				});
				clip->source[3].push_back(
					ANIMINFO{
					_vec3(4.1f, -2.8f, -0.2f),//trans
					_vec3(0,0,0) + _vec3(0,0,0),//rotation
					_vec3(0.f,0.f,0.f),//scale
					0.3f,//tilltime
					0.2f//actionTime
				});
				clip->source[3].push_back(
					ANIMINFO{
					_vec3(4.1f, -2.8f, -0.2f),//trans
					_vec3(0,0,0) + _vec3(0,0,0),//rotation
					_vec3(0.f,0.f,0.f),//scale
					1.5f,//tilltime
					0.5f//actionTime
				});
				clip->source[3].push_back(
					ANIMINFO{
					_vec3(4.1f, -2.8f,- 0.2f) + _vec3(3,5,0),//trans
					_vec3(0,0,0) + _vec3(0,0,0),//rotation
					_vec3(0.f,0.f,0.f),//scale
					0.5f,//tilltime
					2.f//actionTime
				});
			}
			//BackBody
			{
				clip->source[4].push_back(
					ANIMINFO{
					_vec3(3.f, 0.3f, 0.1f) + _vec3(0,-3,0),//trans
					_vec3(0,0,0) + _vec3(0,0,0),//rotation
					_vec3(0.f,0.f,0.f),//scale
					0.2f,//tilltime
					0.f//actionTime
				});

				clip->source[4].push_back(
					ANIMINFO{
					_vec3(3.f, 0.3f, 0.1f),//trans
					_vec3(0,0,0) + _vec3(0,0,0),//rotation
					_vec3(0.f,0.f,0.f),//scale
					1.5f,//tilltime
					0.5f//actionTime
				});

				clip->source[4].push_back(
					ANIMINFO{
					_vec3(3.f, 0.3f, 0.1f) + _vec3(0,1,0),//trans
					_vec3(0,0,0) + _vec3(0,0,0),//rotation
					_vec3(0.f,0.f,0.f),//scale
					0.5f,//tilltime
					2.0f//actionTime
				});
			}
			//FrontBody
			{
				clip->source[5].push_back(
					ANIMINFO{
					_vec3(0.f, 0.f, 0.f) + _vec3(0,2,0),//trans
					_vec3(0,0,0) + _vec3(0,0,0),//rotation
					_vec3(0.f,0.f,0.f),//scale
					0.5f,//tilltime
					0.f//actionTime
				});
				clip->source[5].push_back(
					ANIMINFO{
					_vec3(0.f, 0.f, 0.f),//trans
					_vec3(0,0,0) + _vec3(0,0,0),//rotation
					_vec3(0.f,0.f,0.f),//scale
					1.5f,//tilltime
					0.5f//actionTime
				});
				clip->source[5].push_back(
					ANIMINFO{
					_vec3(0.f, 0.f, 0.f) + _vec3(0,-2,0),//trans
					_vec3(0,0,0) + _vec3(0,0,0),//rotation
					_vec3(0.f,0.f,0.f),//scale
					0.5f,//tilltime
					2.f//actionTime
				});
			}
			//joint
			{
				clip->source[6].push_back(
					ANIMINFO{
					_vec3(-1.0f, 0.0f, 0.3f) + _vec3(0,0,0),//trans
					_vec3(0,0,0) + _vec3(0,0,0),//rotation
					_vec3(0.f,0.f,0.f),//scale
					2.f,//tilltime
					0.f//actionTime
				});
			}
			//joint
			{
				clip->source[7].push_back(
					ANIMINFO{
					_vec3(-1.0f, 0.0f, -0.5f) + _vec3(0,0,0),//trans
					_vec3(0,0,0) + _vec3(0,0,0),//rotation
					_vec3(0.f,0.f,0.f),//scale
					2.f,//tilltime
					0.f//actionTime
				});
			}

		}
		m_pAnimation_Body->AddClip(L"Jump", clip);

		clip = new AnimClip();
		{
			clip->parts.push_back(m_pTransform->GetChild(1)->GetChild(0)); //손
			clip->parts.push_back(m_pTransform->GetChild(1)->GetChild(1)); //손
			clip->parts.push_back(m_pTransform->GetChild(1)->GetChild(2)); //발
			clip->parts.push_back(m_pTransform->GetChild(1)->GetChild(3)); //발
			clip->parts.push_back(m_pTransform->GetChild(1)); //몸통
			clip->parts.push_back(m_pTransform->GetChild(0)); //머리
			clip->parts.push_back(m_pTransform->GetChild(1)->GetChild(5)); //팔 조인트
			clip->parts.push_back(m_pTransform->GetChild(1)->GetChild(6)); //팔 조인트
			clip->parts.push_back(m_pTransform->GetChild(1)->GetChild(7)); //팔 조인트
			clip->parts.push_back(m_pTransform->GetChild(1)->GetChild(8)); //팔 조인트
			clip->source.resize(10);
			clip->TotalTime = 0.5f;
			clip->Useloop = true;
			//LeftFront
			{
				//위에서 입력한 0번 항목
				clip->source[0].push_back(
					ANIMINFO{
						// 원래 포지션			+ 추가될 포지션
					_vec3(0,0,0) + _vec3(-2,0,-0.4f),//trans
						//원래 회전값	+ 추가될 회전값
					_vec3(0,0,0) + _vec3(0,0,0),//rotation
						//원래 스케일(0,0,0)일시 미적용
					_vec3(0.f,0.f,0.f) + _vec3(0,0,0),//scale
					0.5f,//tilltime 애니메이션 재생 시간(0.5초동안 재생해라.)
					0.f//actionTime 애니메이션 시작 시간(0.0초면 재생을 시작해라.)
				});
			}

			//Rightfront
			{
				clip->source[1].push_back(
					ANIMINFO{
					_vec3(0,0,0) + _vec3(2,0,-0.4f),//trans
					_vec3(0,0,0) + _vec3(0,0,0),//rotation
					_vec3(0.f,0.f,0.f),//scale
					0.5f,//tilltime
					0.f//actionTime
				});
			}
			//LeftBack
			{
				clip->source[2].push_back(
					ANIMINFO{
					_vec3(0,0,0) + _vec3(-2,-3,-0.1f),//trans
					_vec3(0,0,0) + _vec3(0,0,0),//rotation
					_vec3(0.f,0.f,0.f),//scale
					0.5f,//tilltime
					0.f//actionTime
				});
			}
			//RightBack
			{
				clip->source[3].push_back(
					ANIMINFO{
					_vec3(0,0,0) + _vec3(2,-3,-0.1f),//trans
					_vec3(0,0,0) + _vec3(0,0,0),//rotation
					_vec3(0.f,0.f,0.f),//scale
					0.5f,//tilltime
					0.f//actionTime
				});
			}
			//BackBody
			{
				clip->source[4].push_back(
					ANIMINFO{
					_vec3(0,0,0) + _vec3(0,0.01f,+0.2f),//trans
					_vec3(0,0,0),
					_vec3(0,0,0),
					0.5f,//tilltime
					0.f//actionTime
				});
			}
			//FrontBody
			{
				clip->source[5].push_back(
					ANIMINFO{
					_vec3(0.f, 0.f, 0.f) + _vec3(0,3,0),//trans
					_vec3(0,0,0) + _vec3(0,0,0),//rotation
					_vec3(0,0,0),
					0.5f,//tilltime
					0.f//actionTime
				});
			}
			//joint
			{
				clip->source[6].push_back(
					ANIMINFO{
					_vec3(0.f, 0.f, 0.f) + _vec3(-1,0,0),//trans
					_vec3(0,0,0) + _vec3(0,0,0),//rotation
					_vec3(0.f,0.f,0.f),//scale
					0.2f,//tilltime
					0.f//actionTime
				});
			}
			//joint
			{
				clip->source[7].push_back(
					ANIMINFO{
					_vec3(0.f, 0.f, 0.f) + _vec3(1,0,0),//trans
					_vec3(0,0,0) + _vec3(0,0,0),//rotation
					_vec3(0.f,0.f,0.f),//scale
					0.2f,//tilltime
					0.f//actionTime
				});
			}
			//joint
			{
				clip->source[8].push_back(
					ANIMINFO{
					_vec3(0.f, 0.f, 0.f) + _vec3(-1,-3,0),//trans
					_vec3(0,0,0) + _vec3(0,0,0),//rotation
					_vec3(0.f,0.f,0.f),//scale
					0.2f,//tilltime
					0.f//actionTime
				});
			}
			//joint
			{
				clip->source[9].push_back(
					ANIMINFO{
					_vec3(0.f, 0.f, 0.f) + _vec3(1,-3,0),//trans
					_vec3(0,0,0) + _vec3(0,0,0),//rotation
					_vec3(0.f,0.f,0.f),//scale
					0.2f,//tilltime
					0.f//actionTime
				});
			}

		}
		m_pAnimation_Body->AddClip(L"Stump", clip);

		clip = new AnimClip();
		{
			clip->parts.push_back(m_pTransform->GetChild(0));
			clip->source.resize(1);
			clip->TotalTime = 0.7f;
			clip->Useloop = true;
			{
				for (int i = 0; i < 10; i++)
				{
					_float angle = GetRandomFloat(-30, 30);
					_float scale = GetRandomFloat(1.2f, 1.6f);
					clip->source[0].push_back(
						ANIMINFO{
						_vec3(0.f, 0.f, 0.0f) + _vec3(GetRandomFloat(-1,1),GetRandomFloat(-1,1),GetRandomFloat(-1,0)),//trans
						_vec3(0,0,0) + _vec3(0,0,D3DXToRadian(angle)),//rotation
						_vec3(scale,scale,scale),//scale
						0.1f,//tilltime
						0.1f * i//actionTime
					});
				}
			}
		}
		m_pAnimation_Body->AddClip(L"Scream", clip);

		clip = new AnimClip();
		{
			clip->parts.push_back(m_pTransform->GetChild(1)->GetChild(0));
			clip->parts.push_back(m_pTransform->GetChild(1)->GetChild(1));
			clip->parts.push_back(m_pTransform->GetChild(1)->GetChild(2));
			clip->parts.push_back(m_pTransform->GetChild(1)->GetChild(3));
			clip->parts.push_back(m_pTransform->GetChild(1));
			clip->parts.push_back(m_pTransform->GetChild(0));
			clip->parts.push_back(m_pTransform->GetChild(1)->GetChild(5));
			clip->parts.push_back(m_pTransform->GetChild(1)->GetChild(6));
			clip->source.resize(8);
			clip->TotalTime = 2.0f;
			clip->Useloop = false;
			//LeftFront
			{
				clip->source[0].push_back(
					ANIMINFO{
					_vec3(-2.1f, -3.4f, 0.3f) + _vec3(-4,70,0),//trans
					_vec3(0,0,0) + _vec3(0,0,0),//rotation
					_vec3(0.f,0.f,0.f),//scale
					2.f,//tilltime
					0.f//actionTime
				});
			}

			//Rightfront
			{
				clip->source[1].push_back(
					ANIMINFO{
					_vec3(0.f, -3.4f, -0.5f) + _vec3(4,70,0),//trans
					_vec3(0,0,0) + _vec3(0,0,0),//rotation
					_vec3(0.f,0.f,0.f),//scale
					2.f,//tilltime
					0.f//actionTime
				});
			}
			//LeftBack
			{
				clip->source[2].push_back(
					ANIMINFO{
					_vec3(2.1f, -2.8f, 0.2f) + _vec3(-4,-3,0),//trans
					_vec3(0,0,0) + _vec3(0,0,0),//rotation
					_vec3(0.f,0.f,0.f),//scale
					0.5f,//tilltime
					0.f//actionTime
				});
			}
			//RightBack
			{
				clip->source[3].push_back(
					ANIMINFO{
					_vec3(4.1f, -2.8f, -0.2f) + _vec3(-2,-3,0),//trans
					_vec3(0,0,0) + _vec3(0,0,0),//rotation
					_vec3(1.f,1.f,1.f),//scale
					0.5f,//tilltime
					0.f//actionTime
				});
			}
			//BackBody
			{
				clip->source[4].push_back(
					ANIMINFO{
					_vec3(3.f, 0.3f, 0.1f) + _vec3(0,-3,0),//trans
					_vec3(0,0,0) + _vec3(0,0,0),//rotation
					_vec3(1.f,1.f,1.f),//scale
					2.f,//tilltime
					0.f//actionTime
				});
			}
			//FrontBody
			{
				clip->source[5].push_back(
					ANIMINFO{
					_vec3(0.f, 0.f, 0.f) + _vec3(3,4,-0.5f),//trans
					_vec3(0,0,0) + _vec3(0,0,0),//rotation
					_vec3(1.f,1.f,1.f),//scale
					2.f,//tilltime
					0.f//actionTime
				});
			}
			//joint
			{
				clip->source[6].push_back(
					ANIMINFO{
					_vec3(-1.0f, 0.0f, 0.3f) + _vec3(-4,0,0),//trans
					_vec3(0,0,0) + _vec3(0,0,0),//rotation
					_vec3(0.f,0.f,0.f),//scale
					2.f,//tilltime
					0.f//actionTime
				});
			}
			//joint
			{
				clip->source[7].push_back(
					ANIMINFO{
					_vec3(-1.0f, 0.0f, -0.5f) + _vec3(4,0,0),//trans
					_vec3(0,0,0) + _vec3(0,0,0),//rotation
					_vec3(0.f,0.f,0.f),//scale
					2.f,//tilltime
					0.f//actionTime
				});
			}

		}
		m_pAnimation_Body->AddClip(L"Punch", clip);
		m_pAnimation_Body->SetAnimation(L"Idle");

		FAILED_CHECK_RETURN(Find_PlayerBoth(), -1);
		m_bInit = true;
	}
	
	CheckZFloor();
	DoFlip();
	m_dwActionTime -= fTimeDelta;
	m_dwRestTime -= fTimeDelta;
	(this->*funcAction[m_eCurrentState][m_iCurrentActionIdx])(fTimeDelta);
	Engine::Add_RenderGroup(RENDER_ALPHA, this);

	m_pTransform->SwapYZ();

	return 0;
}

_int CBoss2::Update_Too(const _float & fTimeDelta)
{
	return 0;
}

_int CBoss2::Update_Top(const _float & fTimeDelta)
{
	return 0;
}

void CBoss2::LateUpdate_GameObject(void)
{
	__super::LateUpdate_GameObject();
}

void CBoss2::SwapTrigger()
{
}

void CBoss2::OnCollisionEnter(const Collision * collision)
{
	//?…ì´???¿ìœ¼ë©?ì¶©ê²© ??ì£¼ê²Ÿ??
	if (dynamic_cast<CCube*>(collision->otherObj)&&collision->_dir == DIR_DOWN)
	{
		m_bIsOnGround = true;
		m_pAnimation_Body->SetAnimation(L"Idle");
		dynamic_cast<CBoss2Foot*>(m_pTransform->GetChild(1)->GetChild(0)->m_pGameObject)->SetAnim(L"Idle");
		dynamic_cast<CBoss2Foot*>(m_pTransform->GetChild(1)->GetChild(1)->m_pGameObject)->SetAnim(L"Idle");
		dynamic_cast<CBoss2Foot*>(m_pTransform->GetChild(1)->GetChild(2)->m_pGameObject)->SetAnim(L"Idle");
		dynamic_cast<CBoss2Foot*>(m_pTransform->GetChild(1)->GetChild(3)->m_pGameObject)->SetAnim(L"Idle");
		dynamic_cast<CStage1Camera*>(Engine::Get_GameObject(L"Layer_Environment", L"Camera"))->Start_Camera_Shake(0.4f, 40.0f, SHAKE_ALL);
	}

	if (dynamic_cast<CSpike*>(collision->otherObj))
	{
		//하얀색으로 반짝이게


		//2초동안 rest
		m_dwRestTime = 2.0f;

		//2초동안 무적임.
	}
		
	__super::OnCollisionEnter(collision);
}

void CBoss2::OnCollisionStay(const Collision * collision)
{
	__super::OnCollisionStay(collision);
}

void CBoss2::OnCollisionExit(const Collision * collision)
{
	if (dynamic_cast<CCube*>(collision->otherObj))
	{
		m_bIsOnGround = false;
	}
	__super::OnCollisionExit(collision);
}

HRESULT CBoss2::Add_Component(void)
{
	CComponent*		pComponent = nullptr;

	pComponent = m_pRigid = dynamic_cast<CRigidbody*>(Engine::Clone_Proto(L"Rigidbody", this));
	NULL_CHECK_RETURN(m_pRigid, E_FAIL);
	m_vecComponent[ID_DYNAMIC].push_back({ L"Rigidbody", pComponent });

	pComponent = m_pCollider = dynamic_cast<CCollider*>(Engine::Clone_Proto(L"Collider", this));
	NULL_CHECK_RETURN(m_pCollider, E_FAIL);
	m_vecComponent[ID_DYNAMIC].push_back({ L"Collider", pComponent });

	pComponent = m_pAnimation_Face = dynamic_cast<CAnimation*>(Engine::Clone_Proto(L"Animation", this));
	NULL_CHECK_RETURN(m_pAnimation_Face, E_FAIL);
	m_vecComponent[ID_DYNAMIC].push_back({ L"Animation", pComponent });

	pComponent = m_pAnimation_Body = dynamic_cast<CAnimation*>(Engine::Clone_Proto(L"Animation", this));
	NULL_CHECK_RETURN(m_pAnimation_Body, E_FAIL);
	m_vecComponent[ID_DYNAMIC].push_back({ L"Animation", pComponent });

	return S_OK;
}

HRESULT CBoss2::Find_PlayerBoth()
{
	m_pPlayer01_trans = dynamic_cast<CTransform*>(Engine::Get_Component(L"Layer_GameLogic", L"Toodee", L"Transform", ID_DYNAMIC));
	NULL_CHECK_RETURN(m_pPlayer01_trans, -1);
	m_pPlayer02_trans = dynamic_cast<CTransform*>(Engine::Get_Component(L"Layer_GameLogic", L"Topdee", L"Transform", ID_DYNAMIC));
	NULL_CHECK_RETURN(m_pPlayer02_trans, -1);
	return S_OK;
}

void CBoss2::CheckZFloor()
{
	if (m_pTransform->m_vInfo[INFO_POS].z > 11)
	{
		m_pTransform->m_vInfo[INFO_POS].z = 10;
		m_pRigid->m_Velocity.z = 0;
		dynamic_cast<CStage1Camera*>(Engine::Get_GameObject(L"Layer_Environment", L"Camera"))->Start_Camera_Shake(0.4f, 40.0f, SHAKE_ALL);
		//?´ê±° ?´ë–»ê²?êº¼ì¤„ê±°ìž„?
		m_bIsOnGround = true;
	}
}

void CBoss2::Do_Jump_Ready(const _float& fTimeDelta)
{
	if (m_iJumpPosidx == 0)
		if (rand() % 2 == 0)
			m_iJumpPosidx = m_iJumpPosidx;
		else
			m_iJumpPosidx++;
	else if (m_iJumpPosidx == 1)
		if (rand() % 3 == 0)
			m_iJumpPosidx--;
		else if (rand() % 3 == 1)
			m_iJumpPosidx = m_iJumpPosidx;
		else
			m_iJumpPosidx++;
	else
		if (rand() % 2 == 0)
			m_iJumpPosidx--;
		else
			m_iJumpPosidx = m_iJumpPosidx;

	m_bFlip_Y = m_pTransform->m_vInfo[INFO_POS].x <= m_fJumpPos[m_iJumpPosidx].x;

	m_bIsOnGround = false;
	m_dwActionTime = 0.5f;
	m_pAnimation_Body->SetAnimation(L"Jump");
	m_iCurrentActionIdx++;
	
}

void CBoss2::Do_Jump_01(const _float& fTimeDelta)
{
	_vec3 _dir, originlen;
	originlen = _vec3(m_fJumpPos[m_iJumpPosidx] - m_pTransform->m_vInfo[INFO_POS]);
	D3DXVec3Normalize(&_dir, &originlen);

	dynamic_cast<CBoss2Foot*>(m_pTransform->GetChild(1)->GetChild(0)->m_pGameObject)->SetAnim(L"Paper");
	dynamic_cast<CBoss2Foot*>(m_pTransform->GetChild(1)->GetChild(1)->m_pGameObject)->SetAnim(L"Paper");
	dynamic_cast<CBoss2Foot*>(m_pTransform->GetChild(1)->GetChild(2)->m_pGameObject)->SetAnim(L"Paper");
	dynamic_cast<CBoss2Foot*>(m_pTransform->GetChild(1)->GetChild(3)->m_pGameObject)->SetAnim(L"Paper");

	m_pTransform->m_vInfo[INFO_POS] += _dir * (0.5f-m_dwActionTime);
	if (D3DXVec3Length(&originlen) <1.f)
	{
		//?¤ìŒ ?‰ë™?¼ë¡œ ?±ã„±
		CheckIsLastActionIdx();
		m_dwRestTime = 1;
	}
}

void CBoss2::Do_Jump_02(const _float& fTimeDelta)
{
	//ê·¸ëƒ¥ ?„ëž˜ë¡?addforceì¤„ê±°??
	m_pRigid->AddForce(_vec3(0, -1, 0), 130, IMPULSE, fTimeDelta);

	dynamic_cast<CBoss2Foot*>(m_pTransform->GetChild(1)->GetChild(0)->m_pGameObject)->SetAnim(L"Jump");
	dynamic_cast<CBoss2Foot*>(m_pTransform->GetChild(1)->GetChild(1)->m_pGameObject)->SetAnim(L"Jump");
	dynamic_cast<CBoss2Foot*>(m_pTransform->GetChild(1)->GetChild(2)->m_pGameObject)->SetAnim(L"Jump");
	dynamic_cast<CBoss2Foot*>(m_pTransform->GetChild(1)->GetChild(3)->m_pGameObject)->SetAnim(L"Jump");

	CheckIsLastActionIdx();
	m_dwRestTime = 1;
}

void CBoss2::Do_Hurray(const _float & fTimeDelta)
{
	//재생되던 애니메이션을 stump로 바꾼다.
	m_pAnimation_Body->SetAnimation(L"Punch");
	//애니메이션 종료까지 대기
	CheckIsLastActionIdx();
	dynamic_cast<CBoss2Foot*>(m_pTransform->GetChild(1)->GetChild(0)->m_pGameObject)->SetAnim(L"Paper");
	dynamic_cast<CBoss2Foot*>(m_pTransform->GetChild(1)->GetChild(1)->m_pGameObject)->SetAnim(L"Paper");
	dynamic_cast<CBoss2Foot*>(m_pTransform->GetChild(1)->GetChild(2)->m_pGameObject)->SetAnim(L"Paper");
	dynamic_cast<CBoss2Foot*>(m_pTransform->GetChild(1)->GetChild(3)->m_pGameObject)->SetAnim(L"Paper");
	m_dwRestTime = 2.0f;
}

void CBoss2::Do_SummonFist(const _float & fTimeDelta)
{
	//손을 소환함.
	for (size_t i = 0; i < sizeof(m_fJumpPos)/sizeof(_vec3); i++)
	{
		if (i != m_iJumpPosidx)
		{
			FAILED_CHECK_RETURN(CManagement::GetInstance()->Get_Layer(L"Layer_GameLogic")->
				Add_GameObject(L"Boss2Hand", CBoss2Hand::Create(m_pGraphicDev, _vec3(m_fJumpPos[i].x, 50.f, 10.f))));
		}
	}
	//손 들어갈떄까지 대기
	CheckIsLastActionIdx();
	dynamic_cast<CBoss2Foot*>(m_pTransform->GetChild(1)->GetChild(0)->m_pGameObject)->SetAnim(L"Idle");
	dynamic_cast<CBoss2Foot*>(m_pTransform->GetChild(1)->GetChild(1)->m_pGameObject)->SetAnim(L"Idle");
	dynamic_cast<CBoss2Foot*>(m_pTransform->GetChild(1)->GetChild(2)->m_pGameObject)->SetAnim(L"Idle");
	dynamic_cast<CBoss2Foot*>(m_pTransform->GetChild(1)->GetChild(3)->m_pGameObject)->SetAnim(L"Idle");
	m_dwRestTime = 4.0f;
}

void CBoss2::Do_Scream(const _float & fTimeDelta)
{
	m_pAnimation_Body->SetAnimation(L"Scream");
	m_pAnimation_Face->SetAnimation(L"Scream");
	
	dynamic_cast<CStage1Camera*>(Engine::Get_GameObject(L"Layer_Environment", L"Camera"))->Start_Camera_Shake(4.0f, 40.0f, SHAKE_ALL);

	m_dwRestTime = 2.0f;
	CheckIsLastActionIdx();
}

void CBoss2::Do_ScreamEnd(const _float & fTimeDelta)
{
	m_pAnimation_Body->SetAnimation(L"Idle");
	m_pAnimation_Face->SetAnimation(L"Idle");
	m_dwRestTime = 1.0f;
	CheckIsLastActionIdx();
}

//?„ìž¬ ?¤í…Œ?´íŠ¸ê°€ ê°™ì?ê²??„ë‹ˆ?¼ë©´ true ë°?ë³€ê²?
void CBoss2::SetPartten()
{
	//100 ?ˆìª½???œë¤ ?œìˆ˜ ?ì„±
	int ranIdx = 0;

	while (true)
	{
		ranIdx = (rand() + 1) % 100;
		switch (ranIdx)
		{
		case 0: //30% idle
			if (m_eCurrentState != B2_THROW)
			{
				m_ePreState = m_eCurrentState;
				m_eCurrentState = B2_THROW;
				m_iCurrentActionIdx = 0;
				return;
			}
			break;
		case 100: //40% jump   
			m_ePreState = m_eCurrentState;
			m_eCurrentState = B2_JUMPING;
			m_iCurrentActionIdx = 0;
			return;
			break;
		case 70: //10% scream
			if (m_eCurrentState != B2_SCREAM)
			{
				m_ePreState = m_eCurrentState;
				m_eCurrentState = B2_SCREAM;
				m_iCurrentActionIdx = 0;
				return;
			}
			break;
		case 80: // 10% punch
			if (m_eCurrentState != B2_PUNCH)
			{
				m_ePreState = m_eCurrentState;
				m_eCurrentState = B2_PUNCH;
				m_iCurrentActionIdx = 0;
				return;
			}
			break;
		case 90: // 10% stump
			if (m_eCurrentState != B2_STUMP)
			{
				m_ePreState = m_eCurrentState;
				m_eCurrentState = B2_STUMP;
				m_iCurrentActionIdx = 0;
				return;
			}
			break;
		}
	}
}

void CBoss2::ReadyPartten()
{
	//ì¢…ë£Œê¹Œì? ?•ìž¥
	funcAction.reserve(B2_END);
	
	BOSS2_STATE_FUNC func; //idle
	func.push_back(&CBoss2::Do_Jump_Ready);
	func.push_back(&CBoss2::Do_Rest);
	func.push_back(&CBoss2::Do_Jump_01);
	func.push_back(&CBoss2::Do_Rest);
	func.push_back(&CBoss2::Do_Jump_02);
	func.push_back(&CBoss2::Do_Rest);
	func.push_back(&CBoss2::Do_ResetVelocity);
	func.push_back(&CBoss2::Do_Idle);
	func.push_back(&CBoss2::Do_Rest);
	funcAction.push_back(func);
	func.clear();

	func.push_back(&CBoss2::Do_Jump_Ready);
	func.push_back(&CBoss2::Do_Rest);
	func.push_back(&CBoss2::Do_Jump_01);
	func.push_back(&CBoss2::Do_Rest);
	func.push_back(&CBoss2::Do_Jump_02);
	func.push_back(&CBoss2::Do_Rest);
	func.push_back(&CBoss2::Do_ResetVelocity);
	func.push_back(&CBoss2::Do_Idle);
	func.push_back(&CBoss2::Do_Rest);
	funcAction.push_back(func);
	func.clear();

	func.push_back(&CBoss2::Do_Scream);
	func.push_back(&CBoss2::Do_Rest);
	func.push_back(&CBoss2::Do_ScreamEnd);
	func.push_back(&CBoss2::Do_Rest);
	funcAction.push_back(func);
	func.clear();

	func.push_back(&CBoss2::Do_Hurray);
	func.push_back(&CBoss2::Do_Rest);
	func.push_back(&CBoss2::Do_SummonFist);
	func.push_back(&CBoss2::Do_Rest);
	func.push_back(&CBoss2::Do_Idle);
	func.push_back(&CBoss2::Do_Rest);
	funcAction.push_back(func);
	func.clear();

	func.push_back(&CBoss2::Do_Stump_Ready);
	func.push_back(&CBoss2::Do_Chase_Player);
	func.push_back(&CBoss2::Do_LittleUp_Turn);
	func.push_back(&CBoss2::Do_Rest);
	func.push_back(&CBoss2::Do_Stump_02);
	func.push_back(&CBoss2::Do_Turn_Minus);
	func.push_back(&CBoss2::Do_ResetVelocity);
	func.push_back(&CBoss2::Do_ResetRot);
	funcAction.push_back(func);

	/* //펀치
	func.push_back(&CBoss2::Do_Hurray);
	func.push_back(&CBoss2::Do_Rest);
	func.push_back(&CBoss2::Do_SummonFist);
	func.push_back(&CBoss2::Do_Rest);
	func.push_back(&CBoss2::Do_Idle);
	func.push_back(&CBoss2::Do_Rest);
	funcAction.push_back(func);
	func.clear();

	// 점프
	func.push_back(&CBoss2::Do_Jump_Ready);
	func.push_back(&CBoss2::Do_Rest);
	func.push_back(&CBoss2::Do_Jump_01);
	func.push_back(&CBoss2::Do_Rest);
	func.push_back(&CBoss2::Do_Jump_02);
	func.push_back(&CBoss2::Do_Rest);
	func.push_back(&CBoss2::Do_ResetVelocity);
	funcAction.push_back(func);
	func.clear();
	
	//찍기
	func.push_back(&CBoss2::Do_Stump_Ready);
	func.push_back(&CBoss2::Do_Chase_Player);
	func.push_back(&CBoss2::Do_LittleUp_Turn);
	func.push_back(&CBoss2::Do_Rest);
	func.push_back(&CBoss2::Do_Stump_02);
	func.push_back(&CBoss2::Do_Turn_Minus);
	func.push_back(&CBoss2::Do_ResetVelocity);
	funcAction.push_back(func);
	func.clear();
	
	//소리지르기
	func.push_back(&CBoss2::Do_Scream);
	func.push_back(&CBoss2::Do_Rest);
	func.push_back(&CBoss2::Do_ScreamEnd);
	func.push_back(&CBoss2::Do_Rest);
	funcAction.push_back(func);
	func.clear();
	*/
}

void CBoss2::Do_Rest(const _float& fTimeDelta)
{
	if (m_dwRestTime < 0)
		CheckIsLastActionIdx();
}

void CBoss2::Do_Rest_grivity(const _float& fTimeDelta)
{
	if (g_Is2D)
	{
		m_pRigid->m_bUseGrivaty = true;
	}
	else
	{
		m_pRigid->m_bUseGrivaty = false;
		m_pRigid->m_Velocity.y = 0;
	}

	if (m_dwRestTime < 0)
	{
		CheckIsLastActionIdx();
		m_pRigid->m_bUseGrivaty = false;
		m_pRigid->m_Velocity.y = 0;
	}
}

void CBoss2::DoFlip()
{
	//스텀프 상태가 아니면
	if (m_eCurrentState != B2_STUMP)
	{
		if (m_bFlip_Y)
			m_pTransform->m_vAngle.y = Lerp(m_pTransform->m_vAngle.y, D3DXToRadian(180), 0.1f);
		else
			m_pTransform->m_vAngle.y = Lerp(m_pTransform->m_vAngle.y, 0, 0.1f);
	}
	
}

void CBoss2::Do_Stump_Ready(const _float & fTimeDelta)
{
	m_pAnimation_Body->SetAnimation(L"Stump");
	m_dwActionTime = 3;
	m_bIsOnGround = false;
	m_pTransform->m_vAngle = _vec3(0, 0, 0);
	CheckIsLastActionIdx();
}

void CBoss2::Do_Chase_Player(const _float & fTimeDelta)
{
	if (g_Is2D)
	{
		float _x =Lerp(m_pTransform->m_vInfo[INFO_POS].x, m_pPlayer01_trans->m_vInfo[INFO_POS].x, 0.1f);
		m_pTransform->m_vInfo[INFO_POS].x = _x;
		float _y = Lerp(m_pTransform->m_vInfo[INFO_POS].y,25, 0.1f);
		m_pTransform->m_vInfo[INFO_POS].y = _y;
		float _z = Lerp(m_pTransform->m_vInfo[INFO_POS].z, 10.f, 0.1f);
		m_pTransform->m_vInfo[INFO_POS].z = _z;
	}
	else 
	{
		float _x = Lerp(m_pTransform->m_vInfo[INFO_POS].x, m_pPlayer02_trans->m_vInfo[INFO_POS].x, 0.1f);
		m_pTransform->m_vInfo[INFO_POS].x = _x;
		//?‘ë””ë©?y???°ë¼ê°€ì¤˜ì•¼??
		float _y = Lerp(m_pTransform->m_vInfo[INFO_POS].y, m_pPlayer02_trans->m_vInfo[INFO_POS].y, 0.1f);
		m_pTransform->m_vInfo[INFO_POS].y = _y;
		float _z = Lerp(m_pTransform->m_vInfo[INFO_POS].z, 4.f, 0.1f);
		m_pTransform->m_vInfo[INFO_POS].z = _z;
	}
	if (m_dwActionTime < 0)
		CheckIsLastActionIdx();
}

void CBoss2::Do_LittleUp_Turn(const _float & fTimeDelta)
{
	if(g_Is2D)
		m_pRigid->AddTorque(_vec3(0, 1, 0), 100.f, IMPULSE, fTimeDelta);
	else
		m_pRigid->AddTorque(_vec3(1, 0, 0), 100.f, IMPULSE, fTimeDelta);
	CheckIsLastActionIdx();
}

void CBoss2::Do_Stump_02(const _float & fTimeDelta)
{
	//?¬ë””ë©??„ëž˜ë¡?
	if (g_Is2D)
		m_pRigid->AddForce(_vec3(0, -1, 0), 100.f, IMPULSE, fTimeDelta);
	//?‘ë””ë©?zë¡?
	else 
		m_pRigid->AddForce(_vec3(0, 0, 1), 40.f, IMPULSE, fTimeDelta);
	CheckIsLastActionIdx();
	m_dwRestTime = 3;
}

void CBoss2::Do_Turn_Minus(const _float & fTimeDelta)
{
	if (m_dwRestTime < 0)
	{
		m_pRigid->m_bUseGrivaty = false;
		m_pRigid->m_Velocity.y = 0;
		CheckIsLastActionIdx();
	}
		
	if(m_bIsOnGround)
		m_pRigid->m_AngularVelocity *= 0.9f;
	if (g_Is2D)
	{
		m_pRigid->m_bUseGrivaty = true;
	}
	else
	{
		m_pRigid->m_bUseGrivaty = false;
		m_pRigid->m_Velocity.y = 0;
	}
}

void CBoss2::CheckIsLastActionIdx()
{
	if (funcAction[m_eCurrentState].size() <= (m_iCurrentActionIdx+1))
		SetPartten();
	else
		m_iCurrentActionIdx++; 
}

CBoss2 * CBoss2::Create(LPDIRECT3DDEVICE9 pGraphicDev, _vec3 & vPos)
{
	CBoss2*		pInstance = new CBoss2(pGraphicDev);

	if (FAILED(pInstance->Ready_GameObject(vPos)))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CBoss2::Free(void)
{
	__super::Free();
}
