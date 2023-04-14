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
#include "AbstractFactory.h"

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

	m_eCurrentState = B2_IDLE;
	m_ePreState = B2_END;
	m_bInit = false;
	//나머지 위치에 손 소환
	m_bIsOnGround = false;
	m_fJumpPos[0] = _vec3(10,25,10);
	m_fJumpPos[1] = _vec3(30,25,10);
	m_fJumpPos[2] = _vec3(50,25,10);
	m_iJumpPosidx = 0;//자신 위치
	m_dwRestTime = 0;
	ReadyPartten();

	m_pTransform->m_vScale = { 3.f, 3.f, 3.f };
	m_pCollider->Set_BoundingBox(_vec3(4, 4, 2));
	m_pTransform->m_vInfo[INFO_POS] = vPos;
	m_pRigid->m_bUseGrivaty = false;

	return S_OK;
}

_int CBoss2::Update_GameObject(const _float & fTimeDelta)
{
	__super::Update_GameObject(fTimeDelta);

	//플레이어 가져오기
	if (!m_bInit)
	{
		//눈알 두쪽 생성
		CLayer* pStageLayer = dynamic_cast<CLayer*>(Engine::Get_Layer(L"Layer_GameLogic"));
		NULL_CHECK_RETURN(pStageLayer, E_FAIL);

		
		FAILED_CHECK_RETURN(FACTORY<CBoss2Face>::Create(L"Boss2Face", pStageLayer, _vec3(-1.f, 0.f, -0.2f), m_pTransform), E_FAIL);
		m_pTransform->GetChild(0)->m_vScale = _vec3(2.0f, 1.5f, 2.0f);
		m_pTransform->GetChild(0)->m_vAngle = _vec3(0,0,D3DXToRadian(0.f));
		FAILED_CHECK_RETURN(FACTORY<CBoss2Jaw>::Create(L"Boss2Jaw", pStageLayer, _vec3(-1.f, -1.f, -0.1f), m_pTransform), E_FAIL);
		m_pTransform->GetChild(1)->m_vScale = _vec3(1.5f, 2, 2);
		FAILED_CHECK_RETURN(FACTORY<CBoss2Eye>::Create(L"Boss2Eye", pStageLayer, _vec3(-2.1f, 0.6f, -0.3f), m_pTransform), E_FAIL);
		m_pTransform->GetChild(2)->m_vScale = _vec3(0.7f, 0.7f, 0.7f);
		FAILED_CHECK_RETURN(FACTORY<CBoss2Eye>::Create(L"Boss2Eye", pStageLayer, _vec3(-0.3f, 0.6f, -0.3f), m_pTransform), E_FAIL);
		m_pTransform->GetChild(3)->m_vScale = _vec3(0.7f, 0.7f, 0.7f);
		FAILED_CHECK_RETURN(FACTORY<CBoss2Nose>::Create(L"Boss2EyeNose", pStageLayer, _vec3(-1.6f, 0.2f, -0.7f), m_pTransform), E_FAIL);
		m_pTransform->GetChild(4)->m_vScale = _vec3(0.5f, 0.5f, 0.5f);
		FAILED_CHECK_RETURN(FACTORY<CBoss2EyeBrow>::Create(L"Boss2EyeBrow", pStageLayer, _vec3(-2.1f, 1.4f, -0.5f), m_pTransform), E_FAIL);
		m_pTransform->GetChild(5)->m_vScale = _vec3(0.7f, 0.7f, 0.7f);
		m_pTransform->GetChild(5)->m_vAngle = _vec3(0, 0, D3DXToRadian(-50.f));
		FAILED_CHECK_RETURN(FACTORY<CBoss2EyeBrow>::Create(L"Boss2EyeBrow", pStageLayer, _vec3(-0.3f, 1.4f, -0.5f), m_pTransform), E_FAIL);
		m_pTransform->GetChild(6)->m_vScale = _vec3(0.7f, 0.7f, 0.7f);
		m_pTransform->GetChild(6)->m_vAngle = _vec3(0, 0, D3DXToRadian(0.f));
		FAILED_CHECK_RETURN(FACTORY<CBoss2TailBody>::Create(L"Boss2TailBody", pStageLayer, _vec3(3.0f, 0.5f, 0.0f), m_pTransform), E_FAIL);
		m_pTransform->GetChild(7)->m_vScale = _vec3(4.0f, 4.0f, 2.0f);
		m_pTransform->GetChild(7)->m_vAngle = _vec3(0, 0, D3DXToRadian(0.f));

		FAILED_CHECK_RETURN(Find_PlayerBoth(), -1);
		m_bInit = true;
	}
	
	CheckZFloor();
	m_dwActionTime -= fTimeDelta;
	m_dwRestTime -= fTimeDelta;
	(this->*funcAction[m_eCurrentState][m_iCurrentActionIdx])(fTimeDelta);
	Engine::Add_RenderGroup(RENDER_ALPHA, this);

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

void CBoss2::Render_GameObject(void)
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransform->Get_WorldMatrixPointer());
	m_pTextureCom->Set_Texture(0);
	m_pBufferCom->Render_Buffer();
	__super::Render_GameObject();
}

void CBoss2::Render_Too(void)
{
}

void CBoss2::Render_Top(void)
{
}

void CBoss2::SwapTrigger()
{
}

void CBoss2::OnCollisionEnter(const Collision * collision)
{
	//땅이랑 닿으면 충격 함 주겟음.
	if (dynamic_cast<CCube*>(collision->otherObj))
	{
		m_bIsOnGround = true;
		dynamic_cast<CStage1Camera*>(Engine::Get_GameObject(L"Layer_Environment", L"Camera"))->Start_Camera_Shake(0.4f, 40.0f, SHAKE_ALL);
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

	pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(Engine::Clone_Proto(L"RcTex", this));
	NULL_CHECK_RETURN(m_pBufferCom, E_FAIL);
	m_vecComponent[ID_STATIC].push_back({ L"RcTex", pComponent });

	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Boss2_Head", this));
	NULL_CHECK_RETURN(m_pTextureCom, E_FAIL);
	m_vecComponent[ID_STATIC].push_back({ L"Texture", pComponent });

	pComponent = m_pRigid = dynamic_cast<CRigidbody*>(Engine::Clone_Proto(L"Rigidbody", this));
	NULL_CHECK_RETURN(m_pRigid, E_FAIL);
	m_vecComponent[ID_DYNAMIC].push_back({ L"Rigidbody", pComponent });

	pComponent = m_pCollider = dynamic_cast<CCollider*>(Engine::Clone_Proto(L"Collider", this));
	NULL_CHECK_RETURN(m_pCollider, E_FAIL);
	m_vecComponent[ID_DYNAMIC].push_back({ L"Collider", pComponent });

	pComponent = m_pAnimation = dynamic_cast<CAnimation*>(Engine::Clone_Proto(L"Animation", this));
	NULL_CHECK_RETURN(m_pAnimation, E_FAIL);
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
		//이거 어떻게 꺼줄거임?
		m_bIsOnGround = true;
	}
}

void CBoss2::Do_Jump_Ready(const _float& fTimeDelta)
{
	m_pTransform->m_vAngle = _vec3(0, 0, 0);
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
	m_bIsOnGround = false;
	m_iCurrentActionIdx++;
}

void CBoss2::Do_Jump_01(const _float& fTimeDelta)
{
	_vec3 _dir, originlen;
	originlen = _vec3(m_fJumpPos[m_iJumpPosidx] - m_pTransform->m_vInfo[INFO_POS]);
	D3DXVec3Normalize(&_dir, &originlen);
	m_pTransform->m_vInfo[INFO_POS] += _dir * fTimeDelta * 40;
	if (D3DXVec3Length(&originlen) <1.f)
	{
		//다음 행동으로 ㄱㄱ
		CheckIsLastActionIdx();
		m_dwRestTime = 1;
	}
}

void CBoss2::Do_Jump_02(const _float& fTimeDelta)
{
	//그냥 아래로 addforce줄거임.
	m_pRigid->AddForce(_vec3(0, -1, 0), 130, IMPULSE, fTimeDelta);
	CheckIsLastActionIdx();
	m_dwRestTime = 1;
}

void CBoss2::Do_Hurray(const _float & fTimeDelta)
{
}

void CBoss2::Do_SummonFist(const _float & fTimeDelta)
{
	for (size_t i = 0; i < sizeof(m_fJumpPos)/sizeof(_vec3); i++)
	{
		//====================================
		if (i != m_iJumpPosidx)
		{
			FAILED_CHECK_RETURN(CManagement::GetInstance()->Get_Layer(L"Layer_GameLogic")->
				Add_GameObject(L"Boss2Hand", CBoss2Hand::Create(m_pGraphicDev, _vec3(m_fJumpPos[i].x, 50.f, 10.f))));
		}
		//====================================
	}
}

//현재 스테이트가 같은게 아니라면 true 및 변경
void CBoss2::SetPartten()
{
	//100 안쪽의 랜덤 난수 생성
	int ranIdx = 0;

	while (true)
	{
		ranIdx = (rand() + 1) % 100;
		switch (ranIdx)
		{
		case 0: //30% idle
			if (m_eCurrentState != B2_IDLE)
			{
				m_ePreState = m_eCurrentState;
				m_eCurrentState = B2_IDLE;
				m_iCurrentActionIdx = 0;
				return;
			}
			break;
		case 30: //40% jump
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
	//종료까지 확장
	funcAction.reserve(B2_END);
	
	BOSS2_STATE_FUNC func; //idle
	func.push_back(&CBoss2::Do_Jump_Ready);
	func.push_back(&CBoss2::Do_Rest);
	func.push_back(&CBoss2::Do_Jump_01);
	func.push_back(&CBoss2::Do_Rest);
	func.push_back(&CBoss2::Do_Jump_02);
	func.push_back(&CBoss2::Do_Rest);
	func.push_back(&CBoss2::Do_ResetVelocity);
	funcAction.push_back(func);
	func.clear();

	func.push_back(&CBoss2::Do_Stump_Ready);
	func.push_back(&CBoss2::Do_Chase_Player);
	func.push_back(&CBoss2::Do_LittleUp_Turn);
	func.push_back(&CBoss2::Do_Rest);
	func.push_back(&CBoss2::Do_Stump_02);
	func.push_back(&CBoss2::Do_Turn_Minus);
	func.push_back(&CBoss2::Do_ResetVelocity);
	funcAction.push_back(func);
	func.clear();

	func.push_back(&CBoss2::Do_Stump_Ready);
	func.push_back(&CBoss2::Do_Chase_Player);
	func.push_back(&CBoss2::Do_LittleUp_Turn);
	func.push_back(&CBoss2::Do_Rest);
	func.push_back(&CBoss2::Do_Stump_02);
	func.push_back(&CBoss2::Do_Turn_Minus);
	func.push_back(&CBoss2::Do_ResetVelocity);
	funcAction.push_back(func);
	func.clear();

	//펀치
	func.push_back(&CBoss2::Do_Stump_Ready);
	func.push_back(&CBoss2::Do_Chase_Player);
	func.push_back(&CBoss2::Do_LittleUp_Turn);
	func.push_back(&CBoss2::Do_Rest);
	func.push_back(&CBoss2::Do_Stump_02);
	func.push_back(&CBoss2::Do_Turn_Minus);
	func.push_back(&CBoss2::Do_ResetVelocity);
	funcAction.push_back(func);
	func.clear();

	func.push_back(&CBoss2::Do_Stump_Ready);
	func.push_back(&CBoss2::Do_Chase_Player);
	func.push_back(&CBoss2::Do_LittleUp_Turn);
	func.push_back(&CBoss2::Do_Rest);
	func.push_back(&CBoss2::Do_Stump_02);
	func.push_back(&CBoss2::Do_Turn_Minus);
	func.push_back(&CBoss2::Do_ResetVelocity);
	funcAction.push_back(func);
	func.clear();
}

void CBoss2::Do_Rest(const _float& fTimeDelta)
{
	if (m_dwRestTime < 0)
		CheckIsLastActionIdx();
}

void CBoss2::Do_Stump_Ready(const _float & fTimeDelta)
{
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
		float _y = Lerp(m_pTransform->m_vInfo[INFO_POS].y,30, 0.1f);
		m_pTransform->m_vInfo[INFO_POS].y = _y;
		float _z = Lerp(m_pTransform->m_vInfo[INFO_POS].z, 10.f, 0.1f);
		m_pTransform->m_vInfo[INFO_POS].z = _z;
	}
	else 
	{
		float _x = Lerp(m_pTransform->m_vInfo[INFO_POS].x, m_pPlayer02_trans->m_vInfo[INFO_POS].x, 0.1f);
		m_pTransform->m_vInfo[INFO_POS].x = _x;
		//탑디면 y도 따라가줘야함
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
		m_pRigid->AddTorque(_vec3(0, 0, 1), 100.f, IMPULSE, fTimeDelta);
	CheckIsLastActionIdx();
}

void CBoss2::Do_Stump_02(const _float & fTimeDelta)
{
	//투디면 아래로
	if (g_Is2D)
	{
		m_pRigid->AddForce(_vec3(0, -1, 0), 100.f, IMPULSE, fTimeDelta);
	}
	//탑디면 z로
	else 
		m_pRigid->AddForce(_vec3(0, 0, 1), 40.f, IMPULSE, fTimeDelta);
	CheckIsLastActionIdx();
	m_dwRestTime = 3;
}

void CBoss2::Do_Turn_Minus(const _float & fTimeDelta)
{
	if (m_dwRestTime < 0)
		CheckIsLastActionIdx();
	if(m_bIsOnGround)
		m_pRigid->m_AngularVelocity *= 0.9f;
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
