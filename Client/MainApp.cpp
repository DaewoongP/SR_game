#include "stdafx.h"
#include "MainApp.h"

#include "Scene.h"
#include "Logo.h"

// ÁÖ¼® Å×½ºÆ®¿ë
CMainApp::CMainApp()
	: m_pDeviceClass(nullptr), m_pManagementClass(nullptr), m_pGraphicDev(nullptr)
{
}


CMainApp::~CMainApp()
{
}

HRESULT CMainApp::Ready_MainApp(void)
{
#ifdef _DEBUG

	/*if (::AllocConsole() == TRUE)
	{
		FILE* nfp[3];
		freopen_s(nfp + 0, "CONOUT$", "rb", stdin);
		freopen_s(nfp + 1, "CONOUT$", "wb", stdout);
		freopen_s(nfp + 2, "CONOUT$", "wb", stderr);
		std::ios::sync_with_stdio();
	}*/

#endif // _DEBUG

	srand((unsigned int)time(NULL));
	FAILED_CHECK_RETURN(Ready_DefaultSetting(&m_pGraphicDev), E_FAIL);
	FAILED_CHECK_RETURN(Set_Scene(m_pGraphicDev, &m_pManagementClass), E_FAIL);


	return S_OK;
}

int CMainApp::Update_MainApp(const _float & fTimeDelta)
{
	Engine::Update_DInput();

	_long	dwMouse = 0;

	/*if (dwMouse = Engine::Get_DIMouseMove(DIMS_Z))
	{
		int a = 0;
	}*/


	m_pManagementClass->Update_Management(fTimeDelta);

	return 0;
}

void CMainApp::LateUpdate_MainApp(void)
{
	m_pManagementClass->LateUpdate_Management();
}

void CMainApp::Render_MainApp(void)
{
	Engine::Render_Begin(D3DXCOLOR(0.5f, 0.5f, 0.5f, 1.f));
	
	m_pManagementClass->Render_Management(m_pGraphicDev);

	Engine::Render_End();
}

HRESULT CMainApp::Ready_DefaultSetting(LPDIRECT3DDEVICE9 * ppGraphicDev)
{
	FAILED_CHECK_RETURN(Engine::Ready_GraphicDev(g_hWnd, MODE_WIN, WINCX, WINCY, &m_pDeviceClass), E_FAIL);
	m_pDeviceClass->AddRef();

	(*ppGraphicDev) = m_pDeviceClass->Get_GraphicDev();
	(*ppGraphicDev)->AddRef();

	FAILED_CHECK_RETURN(Engine::Ready_Font((*ppGraphicDev), L"Font_Default", L"¹ÙÅÁ", 15, 20, FW_HEAVY), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Font((*ppGraphicDev), L"Font_Jinji", L"±Ã¼­", 15, 20, FW_HEAVY), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Font((*ppGraphicDev), L"Font_Ganji", L"°ß°íµñ", 15, 20, FW_HEAVY), E_FAIL);

	// dinput
	FAILED_CHECK_RETURN(Engine::Ready_DInput(g_hInst, g_hWnd), E_FAIL);

	(*ppGraphicDev)->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
	(*ppGraphicDev)->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);

	return S_OK;
}

HRESULT CMainApp::Set_Scene(LPDIRECT3DDEVICE9 pGraphicDev, CManagement ** ppManagement)
{
	CScene*		pScene = nullptr;
	pScene = CLogo::Create(pGraphicDev);
	NULL_CHECK_RETURN(pScene, E_FAIL);

	FAILED_CHECK_RETURN(Engine::Create_Management(ppManagement), E_FAIL);
	(*ppManagement)->AddRef();

	FAILED_CHECK_RETURN((*ppManagement)->Set_Scene(pScene), E_FAIL);

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
	FreeConsole();
	Safe_Release(m_pGraphicDev);

	Safe_Release(m_pManagementClass);
	Safe_Release(m_pDeviceClass);

	Engine::Release_Utility();
	Engine::Release_System();
}
