#include "stdafx.h"
#include "Loading.h"

#include "Export_Function.h"

CLoading::CLoading(LPDIRECT3DDEVICE9 pGraphicDev)
	: m_pGraphicDev(pGraphicDev)
	, m_bFinish(false)
{
	ZeroMemory(m_szString, sizeof(_tchar) * 128);
	m_pGraphicDev->AddRef();
}


CLoading::~CLoading()
{
}

unsigned int CLoading::Thread_Main(void * pArg)
{
	CLoading*	pLoading = reinterpret_cast<CLoading*>(pArg);

	_uint iFlag = 0;

	EnterCriticalSection(&(pLoading->Get_Crt()));

	switch (pLoading->Get_LoadingID())
	{
	case LOADING_STAGE:
		iFlag = pLoading->Loading_ForStage();
		break;
	
	case LOADING_BOSS:
		break;
	}
	
	LeaveCriticalSection(&(pLoading->Get_Crt()));

	//_endthreadex(0);

	return iFlag;
}

HRESULT CLoading::Ready_Loading(LOADINGID eID)
{
	InitializeCriticalSection(&m_Crt);

	// 1. 보안 속성
	// 2. 스택 메모리의 크기, 0인 경우 쓰레드가 요구하는 크기만큼 할당
	// 3. 쓰레드 함수의 이름(함수 포인터)
	// 4. 쓰레드 함수를 통해 가공하고자 하는 데이터의 주소
	// 5. 쓰레드의 생성 및 실행을 조절하기 위한 FLAG 값, 기본 값으로 0
	// 6. 쓰레드 ID반환
	
	m_hThread = (HANDLE)_beginthreadex(nullptr, 0, Thread_Main, this, 0, nullptr);

	m_eID = eID;

	return S_OK;
}

_uint CLoading::Loading_ForStage(void)
{
	Set_String(L"Texture Loading....");

	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Player_Texture", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Resource/Texture/Player/Sonic.png")), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Terrain_Texture", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Resource/Texture/Terrain/Grass_%d.tga", 2)), E_FAIL);

	Set_String(L"Buffer Loading.......");

	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"TriCol", CTriCol::Create(m_pGraphicDev)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"RcCol", CRcCol::Create(m_pGraphicDev)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"TerrainTex", CTerrainTex::Create(m_pGraphicDev)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Collider", CCollider::Create(m_pGraphicDev, true)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Rigidbody", CRigidbody::Create(m_pGraphicDev)), E_FAIL);

	Set_String(L"Buffer Loading..........");

	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"CubeTex", CCubeTex::Create(m_pGraphicDev)), E_FAIL);
	
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"SkyBox_Texture", CTexture::Create(m_pGraphicDev, TEX_CUBE, L"../Resource/Texture/SkyBox/burger%d.dds", 4)), E_FAIL);



	/*FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Transform", CTransform::Create(m_pGraphicDev)), E_FAIL);*/

	m_bFinish = true;

	Set_String(L"Loading Complete!!!!!!!!");

	return 0;
}

CLoading * CLoading::Create(LPDIRECT3DDEVICE9 pGraphicDev, LOADINGID eID)
{
	CLoading *	pInstance = new CLoading(pGraphicDev);

	if (FAILED(pInstance->Ready_Loading(eID)))
	{
		Safe_Release(pInstance);
		return nullptr;
	}
	return pInstance;
}

void CLoading::Free(void)
{
	WaitForSingleObject(m_hThread, INFINITE);
	CloseHandle(m_hThread);
	DeleteCriticalSection(&m_Crt);

	Safe_Release(m_pGraphicDev);
}
