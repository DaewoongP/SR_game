#include "stdafx.h"
#include "MainApp.h"

#include "Scene.h"
#include "Logo.h"

#include"ImguiMgr.h"
#include "PreStage.h"

CMainApp::CMainApp()
	: m_pDeviceClass(nullptr), m_pManagementClass(nullptr), m_pGraphicDev(nullptr)
{
}

CMainApp::~CMainApp()
{
}
HRESULT CMainApp::Ready_MainApp(void)
{
//#ifdef _DEBUG
//
//	if (::AllocConsole() == TRUE)
//	{
//		FILE* nfp[3];
//		freopen_s(nfp + 0, "CONOUT$", "rb", stdin);
//		freopen_s(nfp + 1, "CONOUT$", "wb", stdout);
//		freopen_s(nfp + 2, "CONOUT$", "wb", stderr);
//		std::ios::sync_with_stdio();
//	}
//
//#endif // _DEBUG

	srand((unsigned int)time(NULL));
	FAILED_CHECK_RETURN(Ready_DefaultSetting(&m_pGraphicDev), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Create_Management(&m_pManagementClass), E_FAIL);
	m_pManagementClass->AddRef();
	Ready_Proto();
	CScene*	pScene = CPreStage::Create(m_pGraphicDev, LOADING_LOGO);
	m_pManagementClass->Set_Scene(pScene);
	m_pCImguiMgr->GetInstance()->Ready_Imgui(m_pGraphicDev);

	Engine::Ready_Frame(L"1Sec", 1.f);
	Engine::Ready_Frame(L"0.001Sec", 1000.f);
	return S_OK;
}

int CMainApp::Update_MainApp(const _float & fTimeDelta)
{
	Engine::Update_DInput();

	_long	dwMouse = 0;
	m_pManagementClass->Update_Management(fTimeDelta);

	return 0;
}

void CMainApp::LateUpdate_MainApp(void)
{
	m_pManagementClass->LateUpdate_Management();
}

void CMainApp::Render_MainApp(void)
{
	Engine::Render_Begin(D3DXCOLOR(0.f, 0.f, 0.f, 1.f));

	m_pManagementClass->Render_Management(m_pGraphicDev);
	m_pCImguiMgr->GetInstance()->Update_Imgui(m_pGraphicDev);

	Engine::Render_End();
}

HRESULT CMainApp::Ready_Proto()
{
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"RcTex", CRcTex::Create(m_pGraphicDev)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"RcGradation", CRcGradation::Create(m_pGraphicDev)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Title_Texture", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Resource/Texture/Export_Textures/Sprites/menuLogoSpr/menuLogoSpr.png")), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Spark_Texture", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Resource/Texture/Export_Textures/Sprites/sparkSpr/SparkSpr_0%d.png", 10)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"MenuCube_Texture", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Resource/Texture/Export_Textures/Sprites/menuCubeSpr/menuCube.png")), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Select_Texture", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Resource/Texture/Export_Textures/Sprites/menuLogoSpr/menuLogoSpr_%d.png", 11)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Loading", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Resource/Texture/Loading/Loading_%d.png", 13)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Transform", CTransform::Create(m_pGraphicDev)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"ChapterTex", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Resource/Texture/Export_Textures/Sprites/chapterImagesSpr/chapterImagesSpr_%d.png", 7)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"ChapterCloudTex", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Resource/Texture/Export_Textures/Sprites/chapterImagesSpr/LevelPortalSpr0%d.png", 10)), E_FAIL);
	return S_OK;
}

HRESULT CMainApp::Ready_DefaultSetting(LPDIRECT3DDEVICE9 * ppGraphicDev)
{
	FAILED_CHECK_RETURN(Engine::Ready_GraphicDev(g_hWnd, MODE_WIN, WINCX, WINCY, &m_pDeviceClass), E_FAIL);
	m_pDeviceClass->AddRef();

	(*ppGraphicDev) = m_pDeviceClass->Get_GraphicDev();
	(*ppGraphicDev)->AddRef();

	FAILED_CHECK_RETURN(Engine::Ready_Font((*ppGraphicDev), L"Font_Default", L"바탕", 15, 20, FW_HEAVY), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Font((*ppGraphicDev), L"Font_Jinji", L"궁서", 15, 20, FW_HEAVY), E_FAIL);

	// dinput
	FAILED_CHECK_RETURN(Engine::Ready_DInput(g_hInst, g_hWnd), E_FAIL);

	return S_OK;
}

CMainApp * CMainApp::Create(void)
{
	CMainApp *	pInstance = new CMainApp;

	if (FAILED(pInstance->Ready_MainApp()))
	{
		delete pInstance;
		pInstance = nullptr;
	}

	return pInstance;
}

void CMainApp::Free(void)
{
	m_pCImguiMgr->DestroyInstance();

	FreeConsole();
	Safe_Release(m_pGraphicDev);

	Safe_Release(m_pManagementClass);
	Safe_Release(m_pDeviceClass);
	
	Engine::Release_Utility();
	Engine::Release_System();
}
