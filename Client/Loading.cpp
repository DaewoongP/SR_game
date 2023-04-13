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
	//Sound 로딩오래걸릴경우 여기를 끄면됨
	/*Set_String(L"Sound/Bgm Loading..........");
	FAILED_CHECK_RETURN(Engine::Ready_Sound(), E_FAIL);
	Engine::PlayBGM(L"9.wav", 0.5f);*/
	Set_String(L"Sound/Effect Loading..........");
	


	Set_String(L"Texture Loading....");
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Stage_Texture", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Resource/Texture/Export_Textures/Sprites/theme3BgSpr/theme3BgSpr_0.png")), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"None_Texture", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Resource/Texture/None/Alpha0.png")), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Player_Texture", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Resource/Texture/Export_Textures/Sprites/toodeeSpr/toodeeSpr_%d.png", 73)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Terrain_Texture", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Resource/Texture/Terrain/Grass_%d.tga", 2)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Tile_Texture", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Resource/Texture/Terrain/TileFloor.png")), E_FAIL);
	// 검정색 타일(빈 공간 묘사 용)
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Stage1_Tile_Texture", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Resource/Texture/Terrain/TileBlack.png")), E_FAIL);
	//2D돼지
	//앞
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Pig_Texture", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Resource/Texture/Export_Textures/Sprites/pigSpr/correct-pigSpr-%d.png", 9)), E_FAIL);
	//뒤
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Back_Pig_Texture", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Resource/Texture/Export_Textures/Sprites/pigSpr/Back_pigSpr_%d.png", 9)), E_FAIL);
	//Key
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"KeyBox_Texture", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Resource/Texture/Export_Textures/Sprites/keySpr/keySpr_%d.png", 11)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Key_Texture", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Resource/Texture/Export_Textures/Sprites/keySpr/keySpr_%d.png",21)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Spike_Texture", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Resource/Texture/Export_Textures/Sprites/spikesSpr/spikesSpr_%d.png", 13)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Fireball_Texture", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Resource/Texture/Export_Textures/Sprites/b1FireballSpr/b1FireballSpr_%d.png", 9)), E_FAIL);
	
	//박쥐
	//앞
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Bat_Front_Texture", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Resource/Texture/Export_Textures/Sprites/batSpr/bat_Front%d.png", 6)), E_FAIL);
	//뒤
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Bat_Back_Texture", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Resource/Texture/Export_Textures/Sprites/batSpr/bat_Back%d.png", 6)), E_FAIL);

	// imgui 디폴트 몬스터
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Default_Monster_Texture", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Resource/Texture/Export_Textures/Sprites/boss1Spr/boss1Spr_0.png")), E_FAIL);
	// imgui 디폴트 맵 오브젝트
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Default_MapObject_Texture", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Resource/Texture/Export_Textures/Sprites/planetsSpr/planetsSpr_0.png")), E_FAIL);
	// 포탈
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Portal_Texture", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Resource/Texture/Export_Textures/Sprites/portalSpr/NewportalSpr/portalSpr_%d.png", 11)), E_FAIL);
	// 포탈 진입 애니메이션
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Swallow_Portal_Texture", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Resource/Texture/Export_Textures/Sprites/portalSpr/SwallowportalSpr/spriteportal_%d.png", 17)), E_FAIL);

	//구름
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Cloud_Texture", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Resource/Texture/Export_Textures/Sprites/cloudSpr/Cloud_%d.png", 6)), E_FAIL);
	//번개
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Lightning_Texture", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Resource/Texture/Export_Textures/Sprites/lightningSpr/lightningSpr_%d.png", 8)), E_FAIL);
	// 스위치 큐브
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Switch_Texture", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Resource/Texture/Export_Textures/Sprites/buttonSpr/buttonSprNew/buttonSpr_%d.png", 2)), E_FAIL);

	Set_String(L"Buffer Loading.......");

	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"TriCol", CTriCol::Create(m_pGraphicDev)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"RcCol", CRcCol::Create(m_pGraphicDev)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"TerrainTex", CTerrainTex::Create(m_pGraphicDev)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Collider", CCollider::Create(m_pGraphicDev, true)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Rigidbody", CRigidbody::Create(m_pGraphicDev)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Shadow", CShadow::Create(m_pGraphicDev)), E_FAIL);

	Set_String(L"CubeTex Loading..........");

	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"CubeTex", CCubeTex::Create(m_pGraphicDev)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"SkyBox_Texture", CTexture::Create(m_pGraphicDev, TEX_CUBE, L"../Resource/Texture/SkyBox/burger%d.dds", 4)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Move_Cube", CTexture::Create(m_pGraphicDev, TEX_CUBE, L"../Resource/Texture/SkyBox/NormalBox.dds")), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Texture_Cube", CTexture::Create(m_pGraphicDev, TEX_CUBE, L"../Resource/Texture/SkyBox/Texture.dds")), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"KeyBox_Cube", CTexture::Create(m_pGraphicDev, TEX_CUBE, L"../Resource/Texture/SkyBox/KeyBox.dds")), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Stage1_Cube", CTexture::Create(m_pGraphicDev, TEX_CUBE, L"../Resource/Texture/SkyBox/Stage1_%d.dds", 2)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"CrackBlock_Cube", CTexture::Create(m_pGraphicDev, TEX_CUBE, L"../Resource/Texture/SkyBox/CrackBlock.dds")), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"SwitchCube_Cube", CTexture::Create(m_pGraphicDev, TEX_CUBE, L"../Resource/Texture/SkyBox/SwitchCube.dds")), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Portal_Cube", CTexture::Create(m_pGraphicDev, TEX_CUBE, L"../Resource/Texture/SkyBox/PortalCube.dds")), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Stage3_Boss_Cube", CTexture::Create(m_pGraphicDev, TEX_CUBE, L"../Resource/Texture/SkyBox/Stage3Boss.dds")), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Stage3_Boss_Hand_Cube", CTexture::Create(m_pGraphicDev, TEX_CUBE, L"../Resource/Texture/SkyBox/Boss3Hand%d.dds", 2)), E_FAIL);
	
	Set_String(L"Particle Loading..........");

	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"BlockExp", CBlockExp::Create(m_pGraphicDev,
		L"../Resource/Texture/Export_Textures/Sprites/blockExpSpr/blockExpSpr_%d.png", 
		9, 1.f, 1, false)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"CSparkParticle", CSparkParticle::Create(m_pGraphicDev,
		L"../Resource/Texture/Export_Textures/Sprites/sparkSpr/SparkSpr_0%d.png",
		10, 1.f, 2, false)), E_FAIL);

	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"CircularParticle", CCircularParticle::Create(m_pGraphicDev,
		L"../Resource/Texture/Export_Textures/Sprites/particleSpr/particleSpr_0.png", 1,
		1.f, 10, false, 1.f, 10.f)), E_FAIL);

	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"JumpParticle", CJumpParticle::Create(m_pGraphicDev,
		L"../Resource/Texture/Export_Textures/Sprites/particleSpr/particleSpr_0.png", 1,
		1.f, 15, false)), E_FAIL);

	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"LandingParticle", CLandingParticle::Create(m_pGraphicDev,
		L"../Resource/Texture/Export_Textures/Sprites/particleSpr/particleSpr_0.png", 1,
		1.f, 10, false)), E_FAIL);

	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Line", CLine::Create(m_pGraphicDev)), E_FAIL);
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
