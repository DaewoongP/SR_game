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

	m_hThread = (HANDLE)_beginthreadex(nullptr, 0, Thread_Main, this, 0, nullptr);

	m_eID = eID;

	return S_OK;
}

_uint CLoading::Loading_ForStage(void)
{
	Set_String(L"Texture Loading....");

	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Player_Texture", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Resource/Texture/Export_Textures/Sprites/toodeeSpr/toodeeSpr_%d.png", 68)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Terrain_Texture", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Resource/Texture/Terrain/Grass_%d.tga", 2)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Tile_Texture", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Resource/Texture/Terrain/TileFloor.png")), E_FAIL);
	//2D돼지
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Pig_Texture", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Resource/Texture/Export_Textures/Sprites/pigSpr/correct-pigSpr-%d.png", 9)), E_FAIL);
	//3D돼지
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Pig_Parts_Texture", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Resource/Texture/Export_Textures/Sprites/pigTopSpr/pigTopSpr_%d.png", 6)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Stage1_Tile_Texture", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Resource/Texture/Export_Textures/Sprites/theme1WallSpr/theme1WallSpr_%d.png", 20)), E_FAIL);
	//Key
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Key_Texture", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Resource/Texture/Export_Textures/Sprites/keySpr/keySpr_%d.png",11)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"KeyBox_Texture", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Resource/Texture/Export_Textures/Sprites/keySpr/keySpr_%d.png", 11)), E_FAIL);

	Set_String(L"Buffer Loading.......");

	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"TriCol", CTriCol::Create(m_pGraphicDev)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"RcCol", CRcCol::Create(m_pGraphicDev)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"TerrainTex", CTerrainTex::Create(m_pGraphicDev)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Collider", CCollider::Create(m_pGraphicDev, true)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Rigidbody", CRigidbody::Create(m_pGraphicDev)), E_FAIL);

	Set_String(L"CubeTex Loading..........");

	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"CubeTex", CCubeTex::Create(m_pGraphicDev)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"SkyBox_Texture", CTexture::Create(m_pGraphicDev, TEX_CUBE, L"../Resource/Texture/SkyBox/burger%d.dds", 4)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Texture_Cube", CTexture::Create(m_pGraphicDev, TEX_CUBE, L"../Resource/Texture/SkyBox/Texture.dds")), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Stage1_Cube", CTexture::Create(m_pGraphicDev, TEX_CUBE, L"../Resource/Texture/SkyBox/Stage1.dds")), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"KeyBox_Cube", CTexture::Create(m_pGraphicDev, TEX_CUBE, L"../Resource/Texture/SkyBox/KeyBox.dds")), E_FAIL);

	//파티클이 사라져야하는 boundbox영역은 맵 전역이다.
	/*BoundingBox* box = new BoundingBox(_vec3(-100, -100, -20), _vec3(100, 100, 20));
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"SnowParticle", CSnow::Create(m_pGraphicDev, box, 30)), E_FAIL);*/

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
