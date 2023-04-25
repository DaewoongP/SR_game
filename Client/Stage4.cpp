#include "stdafx.h"
#include "Stage4.h"
#include "AbstractFactory.h"
#include "StageCamera.h"
#include "StageBG.h"
#include "Cube.h"
#include "Toodee.h"
#include "Topdee.h"
#include "ImguiMgr.h"
#include "ImguiStage.h"
#include "ImguiUnit.h"
#include "Boss3.h"
#include "Boss2.h"
#include "Fade.h"
#include"HpUI.h"
#include"UICamera.h"
#include"ImguiBG.h"

CStage4::CStage4(LPDIRECT3DDEVICE9 pGraphicDev)
	:CScene(pGraphicDev)
{
}


CStage4::~CStage4()
{
}

HRESULT CStage4::Ready_Scene(void)
{
	m_eLoadingID = LOADING_STAGE4;
	m_pFade = CFade::Create(m_pGraphicDev, false);
	FAILED_CHECK_RETURN(Ready_Layer_Environment(L"Layer_Environment"), E_FAIL);
	FAILED_CHECK_RETURN(Ready_Layer_GameLogic(L"Layer_GameLogic"), E_FAIL);
	FAILED_CHECK_RETURN(Ready_Layer_UI(L"Layer_UI"), E_FAIL);

	CImguiMgr::GetInstance()->Get_Stage()->LoadCube(3, this);
	CImguiMgr::GetInstance()->Get_Stage()->LoadGrid(3, this);
	CImguiMgr::GetInstance()->Get_Unit()->LoadMapObject(3, this);
	CImguiMgr::GetInstance()->Get_Unit()->LoadMonster(3, this);
	CImguiMgr::GetInstance()->Get_BG()->LoadBG(3, this);

	m_pGraphicDev->SetRenderState(D3DRS_LIGHTING, FALSE);
	Engine::StopSound(SOUND_BGM);
	Engine::PlayBGM(L"0.wav", 0.4f);
	return S_OK;
}

_int CStage4::Update_Scene(const _float & fTimeDelta)
{
	if (Get_GameObject(L"Layer_GameLogic", L"Boss2")->Get_Damage())
		Get_GameObject(L"Layer_UI", L"HpUI")->Get_Damage();
		
	return __super::Update_Scene(fTimeDelta);
}

void CStage4::LateUpdate_Scene(void)
{
	__super::LateUpdate_Scene();
}

void CStage4::Render_Scene(void)
{
}

HRESULT CStage4::Ready_Layer_Environment(const _tchar * pLayerTag)
{
	CLayer*		pLayer = CLayer::Create();
	NULL_CHECK_RETURN(pLayer, E_FAIL);

	CGameObject*		pGameObject = nullptr;
	
	FAILED_CHECK_RETURN(FACTORY<CStage1Camera>::Create(L"Camera", pLayer), E_FAIL);

	m_uMapLayer.insert({ pLayerTag, pLayer });

	return S_OK;
}

HRESULT CStage4::Ready_Layer_GameLogic(const _tchar * pLayerTag)
{
	CLayer*		pLayer = CLayer::Create();
	NULL_CHECK_RETURN(pLayer, E_FAIL);

	CGameObject*		pGameObject = nullptr;

	FAILED_CHECK_RETURN(FACTORY<CToodee>::Create(L"Toodee", pLayer, _vec3(6.f, 16.f, 10.f)), E_FAIL);
	FAILED_CHECK_RETURN(FACTORY<CTopdee>::Create(L"Topdee", pLayer, _vec3(16.f, 28.f, 11.f)), E_FAIL);

	FAILED_CHECK_RETURN(FACTORY<CBoss2>::Create(L"Boss2", pLayer, _vec3(50.f, 20.f, 10.f)), E_FAIL);
	m_iHp=pLayer->Get_GameObject(L"Boss2")->Get_Hp();
	
	for (int i = 0; i < CUBEY; i++)
	{
		for (int j = 0; j < CUBEX; j++)
		{
			//맨 윗줄
			if (i == 0)
				FAILED_CHECK_RETURN(FACTORY<CCube>::Create(L"MapCube", pLayer, _vec3{ (_float)j * 2,(_float)i * 2,10.f }, 2), E_FAIL);
			//사이 첫줄																									  
			if (i == CUBEY - 1)																							  
				FAILED_CHECK_RETURN(FACTORY<CCube>::Create(L"MapCube", pLayer, _vec3{ (_float)j * 2,(_float)i * 2,10.f }, 2), E_FAIL);
			//사이 마지막줄																								
			if (j == 0)																									  
				FAILED_CHECK_RETURN(FACTORY<CCube>::Create(L"MapCube", pLayer, _vec3{ (_float)j * 2,(_float)i * 2,10.f }, 2), E_FAIL);
			//맨 아랫줄																									  
			if (j == CUBEX - 1)																							  
				FAILED_CHECK_RETURN(FACTORY<CCube>::Create(L"MapCube", pLayer, _vec3{ (_float)j * 2,(_float)i * 2,10.f }, 2), E_FAIL);
		}
	}

	m_uMapLayer.insert({ pLayerTag, pLayer });
	return S_OK;
}

HRESULT CStage4::Ready_Layer_UI(const _tchar * pLayerTag)
{
	CLayer*		pLayer = CLayer::Create();
	NULL_CHECK_RETURN(pLayer, E_FAIL);
	CGameObject* pGameObject = nullptr;

	for (int i = 0; i < m_iHp; ++i)
	{
		pGameObject = CHpUI::Create(m_pGraphicDev, m_iHp,i);
		NULL_CHECK_RETURN(pGameObject, E_FAIL);
		FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"HpUI", pGameObject), E_FAIL);
	}

	m_uMapLayer.insert({ pLayerTag, pLayer });
	return S_OK;
}

CStage4 * CStage4::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CStage4 *	pInstance = new CStage4(pGraphicDev);

	if (FAILED(pInstance->Ready_Scene()))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CStage4::Free(void)
{
	__super::Free();
}
