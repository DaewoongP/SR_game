#include "stdafx.h"
#include "Stage8.h"
#include "AbstractFactory.h"
#include "StageCamera.h"
#include "StageBG.h"
#include "Toodee.h"
#include "Topdee.h"
#include "ImguiMgr.h"
#include "ImguiStage.h"
#include "ImguiUnit.h"

#include "Boss3.h"
#include "Boss3Hand.h"
#include "Key.h"
#include "KeyCube.h"
#include "Spike.h"
#include "InstallCube.h"
#include "ImguiBG.h"
#include "Boss3Camera.h"
#include "HpUI.h"
#include "Fade.h"
#include "BossDeadCamera.h"

CStage8::CStage8(LPDIRECT3DDEVICE9 pGraphicDev)
	:CScene(pGraphicDev),
	m_iPreBossHp(3),
	m_fDist(0.f),
	m_bLerp(false), m_bLerpInit(true)
{
}

CStage8::~CStage8()
{
}

HRESULT CStage8::Ready_Scene(void)
{
	m_eLoadingID = LOADING_STAGE8;
	m_pFade = CFade::Create(m_pGraphicDev, false);
	FAILED_CHECK_RETURN(Ready_Layer_Environment(L"Layer_Environment"), E_FAIL);
	FAILED_CHECK_RETURN(Ready_Layer_GameLogic(L"Layer_GameLogic"), E_FAIL);
	FAILED_CHECK_RETURN(Ready_Layer_UI(L"Layer_UI"), E_FAIL);
	CImguiMgr::GetInstance()->Get_Stage()->LoadCube(7, this);
	CImguiMgr::GetInstance()->Get_Stage()->LoadGrid(7, this);
	CImguiMgr::GetInstance()->Get_Unit()->LoadMapObject(7, this);
	CImguiMgr::GetInstance()->Get_Unit()->LoadMonster(7, this);
	CImguiMgr::GetInstance()->Get_BG()->LoadBG(7, this);
	m_pGraphicDev->SetRenderState(D3DRS_LIGHTING, FALSE);
	Engine::StopSound(SOUND_BGM);
	Engine::PlayBGM(L"1.wav", 0.35f);
	return S_OK;
}

_int CStage8::Update_Scene(const _float & fTimeDelta)
{
	if (m_iPreBossHp == 2 && m_bUICheck == true)
	{
		Get_GameObject(L"Layer_UI", L"HpUI")->Set_Dead();
		m_bUICheck = false;
	}
	if (m_iPreBossHp == 1 && m_bUICheck == true)
	{
		Get_GameObject(L"Layer_UI", L"HpUI")->Set_Dead();
		m_bUICheck = false;

	}
	if (m_iPreBossHp == 0 && m_bUICheck == true)
	{
		Get_GameObject(L"Layer_UI", L"HpUI")->Set_Dead();
		m_bUICheck = false;

	}

	if (nullptr != Engine::Get_GameObject(L"Layer_GameLogic", L"Boss3"))
		PatternSet(fTimeDelta);
	
	if (m_bLerp)
		Player_Reset(fTimeDelta);

	return __super::Update_Scene(fTimeDelta);
}

void CStage8::LateUpdate_Scene(void)
{
	__super::LateUpdate_Scene();
}

void CStage8::Render_Scene(void)
{
}

HRESULT CStage8::Ready_Layer_Environment(const _tchar * pLayerTag)
{
	CLayer*		pLayer = CLayer::Create();
	NULL_CHECK_RETURN(pLayer, E_FAIL);

	CGameObject*		pGameObject = nullptr;

	FAILED_CHECK_RETURN(FACTORY<CStage1Camera>::Create(L"Camera", pLayer), E_FAIL);
	FAILED_CHECK_RETURN(FACTORY<CBoss2DeadCamera>::Create(L"BossDeadCamera", pLayer), E_FAIL);
	FAILED_CHECK_RETURN(FACTORY<CBoss3Camera>::Create(L"Boss3Camera", pLayer), E_FAIL);

	FAILED_CHECK_RETURN(FACTORY<CStage1BG>::Create(L"StageBG", pLayer), E_FAIL);

	m_uMapLayer.insert({ pLayerTag, pLayer });

	return S_OK;
}

HRESULT CStage8::Ready_Layer_GameLogic(const _tchar * pLayerTag)
{
	CLayer*		pLayer = CLayer::Create();
	NULL_CHECK_RETURN(pLayer, E_FAIL);

	CGameObject*		pGameObject = nullptr;

	FAILED_CHECK_RETURN(FACTORY<CToodee>::Create(L"Toodee", pLayer, _vec3(4.f, 4.f, 10.f)), E_FAIL);
	FAILED_CHECK_RETURN(FACTORY<CTopdee>::Create(L"Topdee", pLayer, _vec3(58.f, 10.f, 11.f)), E_FAIL);

	FAILED_CHECK_RETURN(FACTORY<CBoss3>::Create(L"Boss3", pLayer, _vec3(30.f, 16.f, 10.f)), E_FAIL);
	m_iHp = pLayer->Get_GameObject(L"Boss3")->Get_Hp();

	for (int i = 0; i < CUBEY; i++)
	{
		for (int j = 0; j < CUBEX; j++)
		{
			//맨 윗줄
			if (i == 0)
				FAILED_CHECK_RETURN(FACTORY<CCube>::Create(L"MapCube", pLayer, _vec3{ (_float)j * 2,(_float)i * 2,10.f }, 4), E_FAIL);
			//사이 첫줄
			if (i == CUBEY - 1)
				FAILED_CHECK_RETURN(FACTORY<CCube>::Create(L"MapCube", pLayer, _vec3{ (_float)j * 2,(_float)i * 2,10.f }, 4), E_FAIL);
			//사이 마지막줄
			if (j == 0)
				FAILED_CHECK_RETURN(FACTORY<CCube>::Create(L"MapCube", pLayer, _vec3{ (_float)j * 2,(_float)i * 2,10.f }, 4), E_FAIL);
			//맨 아랫줄
			if (j == CUBEX - 1)
				FAILED_CHECK_RETURN(FACTORY<CCube>::Create(L"MapCube", pLayer, _vec3{ (_float)j * 2,(_float)i * 2,10.f }, 4), E_FAIL);
		}
	}
	m_uMapLayer.insert({ pLayerTag, pLayer });
	return S_OK;
}

HRESULT CStage8::Ready_Layer_UI(const _tchar * pLayerTag)
{
	CLayer*		pLayer = CLayer::Create();
	NULL_CHECK_RETURN(pLayer, E_FAIL);

	CGameObject*		pGameObject = nullptr;
	for (int i = 0; i < m_iHp; ++i)
	{
		pGameObject = CHpUI::Create(m_pGraphicDev, m_iHp, i);
		NULL_CHECK_RETURN(pGameObject, E_FAIL);
		FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"HpUI", pGameObject), E_FAIL);
	}
	m_uMapLayer.insert({ pLayerTag, pLayer });
	return S_OK;
}

void CStage8::PatternSet(const _float & fTimeDelta)
{
	CLayer*		pLayer = Engine::Get_Layer(L"Layer_GameLogic");
	NULL_CHECK_RETURN(pLayer, );

	_int iBossHp = 0;

	if (nullptr != Engine::Get_GameObject(L"Layer_GameLogic", L"Boss3"))
		iBossHp = dynamic_cast<CBoss3*>(Engine::Get_GameObject(L"Layer_GameLogic", L"Boss3"))->Get_Boss3Hp();

	// 보스 체력이 2가 되면
	if (2 == iBossHp && 3 == m_iPreBossHp)
	{
		for (int i = 0; i < 30; ++i)
		{
			FAILED_CHECK_RETURN(FACTORY<CSpike>::Create(L"Spike", pLayer, _vec3((2.f * i + 2.f), 2.f, 10.f)), );
		}
		pLayer->Delete_Tag(L"Key");

		FAILED_CHECK_RETURN(FACTORY<CKey>::Create(L"Key", pLayer, _vec3(6.f, 30.f, 10.f)), );
		FAILED_CHECK_RETURN(FACTORY<CKey>::Create(L"Key", pLayer, _vec3(32.f, 30.f, 10.f)), );
		FAILED_CHECK_RETURN(FACTORY<CKey>::Create(L"Key", pLayer, _vec3(58.f, 30.f, 10.f)), );

		FAILED_CHECK_RETURN(FACTORY<CKeyCube>::Create(L"KeyCube", pLayer, _vec3(58.f, 18.f, 10.f)), );
		FAILED_CHECK_RETURN(FACTORY<CKeyCube>::Create(L"KeyCube", pLayer, _vec3(2.f, 6.f, 10.f)), );
		FAILED_CHECK_RETURN(FACTORY<CKeyCube>::Create(L"KeyCube", pLayer, _vec3(4.f, 6.f, 10.f)), );
		FAILED_CHECK_RETURN(FACTORY<CKeyCube>::Create(L"KeyCube", pLayer, _vec3(6.f, 6.f, 10.f)), );

		dynamic_cast<CBoss3*>(Engine::Get_GameObject(L"Layer_GameLogic", L"Boss3"))->Set_Damaged();
		dynamic_cast<CBoss3Hand*>(Engine::Get_GameObject(L"Layer_GameLogic", L"Boss3Left"))->Set_Damaged();
		dynamic_cast<CBoss3Hand*>(Engine::Get_GameObject(L"Layer_GameLogic", L"Boss3Right"))->Set_Damaged();
		dynamic_cast<CBoss3*>(Engine::Get_GameObject(L"Layer_GameLogic", L"Boss3"))->Set_Lerp();
		dynamic_cast<CBoss3Hand*>(Engine::Get_GameObject(L"Layer_GameLogic", L"Boss3Left"))->Set_Lerp();
		dynamic_cast<CBoss3Hand*>(Engine::Get_GameObject(L"Layer_GameLogic", L"Boss3Right"))->Set_Lerp();
		m_bLerp = true;
		m_bLerpInit = true;
		m_bUICheck = true;
		m_iPreBossHp = 2;
	}

	// 보스 체력이 1이 되면
	if (1 == iBossHp && 2 == m_iPreBossHp)
	{
		pLayer->Delete_Tag(L"Key");

		FAILED_CHECK_RETURN(FACTORY<CKey>::Create(L"Key", pLayer, _vec3(6.f, 30.f, 10.f)), );
		FAILED_CHECK_RETURN(FACTORY<CKey>::Create(L"Key", pLayer, _vec3(32.f, 30.f, 10.f)), );
		FAILED_CHECK_RETURN(FACTORY<CKey>::Create(L"Key", pLayer, _vec3(58.f, 30.f, 10.f)), );

		FAILED_CHECK_RETURN(FACTORY<CKeyCube>::Create(L"KeyCube", pLayer, _vec3(58.f, 18.f, 10.f)), );
		FAILED_CHECK_RETURN(FACTORY<CKeyCube>::Create(L"KeyCube", pLayer, _vec3(2.f, 6.f, 10.f)), );
		FAILED_CHECK_RETURN(FACTORY<CKeyCube>::Create(L"KeyCube", pLayer, _vec3(4.f, 6.f, 10.f)), );
		FAILED_CHECK_RETURN(FACTORY<CKeyCube>::Create(L"KeyCube", pLayer, _vec3(6.f, 6.f, 10.f)), );

		dynamic_cast<CBoss3*>(Engine::Get_GameObject(L"Layer_GameLogic", L"Boss3"))->Set_Damaged();
		dynamic_cast<CBoss3Hand*>(Engine::Get_GameObject(L"Layer_GameLogic", L"Boss3Left"))->Set_Damaged();
		dynamic_cast<CBoss3Hand*>(Engine::Get_GameObject(L"Layer_GameLogic", L"Boss3Right"))->Set_Damaged();
		dynamic_cast<CBoss3*>(Engine::Get_GameObject(L"Layer_GameLogic", L"Boss3"))->Set_Lerp();
		dynamic_cast<CBoss3Hand*>(Engine::Get_GameObject(L"Layer_GameLogic", L"Boss3Left"))->Set_Lerp();
		dynamic_cast<CBoss3Hand*>(Engine::Get_GameObject(L"Layer_GameLogic", L"Boss3Right"))->Set_Lerp();
		m_bLerp = true;
		m_bLerpInit = true;
		m_bUICheck = true;
		m_iPreBossHp = 1;
	}
	if(1==iBossHp==0&&1==m_iPreBossHp)
		m_bUICheck = true;

}

void CStage8::Player_Reset(const _float & fTimeDelta)
{
	_vec3 vLerp, vGoal;
	vGoal = _vec3{ 6.f, 10.f, 10.f };

	if (m_bLerpInit)
	{
		m_vPos = Engine::Get_GameObject(L"Layer_GameLogic", L"Toodee")->m_pTransform->m_vInfo[INFO_POS];
		m_bLerpInit = false;
	}		

	D3DXVec3Lerp(&vLerp, &m_vPos, &vGoal, m_fDist);
	Engine::Get_GameObject(L"Layer_GameLogic", L"Toodee")->m_pTransform->m_vInfo[INFO_POS].x = vLerp.x;
	Engine::Get_GameObject(L"Layer_GameLogic", L"Toodee")->m_pTransform->m_vInfo[INFO_POS].y = vLerp.y;

	m_fDist += 0.005f;

	if (1.f <= m_fDist)
	{
		m_bLerp = false;
		m_fDist = 0.f;
	}
}

CStage8 * CStage8::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CStage8 *	pInstance = new CStage8(pGraphicDev);

	if (FAILED(pInstance->Ready_Scene()))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CStage8::Free(void)
{
	__super::Free();
}
