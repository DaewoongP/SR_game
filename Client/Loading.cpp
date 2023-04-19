#include "stdafx.h"
#include "Loading.h"

#include "Export_Function.h"
#include "ImguiMgr.h"
#include "imgui.h"
#include "PreStage.h"
#include "Logo.h"
#include "Stage1.h"
#include "Stage2.h"
#include "Stage3.h"
#include "Stage4.h"
#include "FinalStage1.h"
#include "BackGroundToolScene.h"

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
	case LOADING_LOGO:
		iFlag = pLoading->Loading_ForLogo();
		break;
	case LOADING_STAGE1:
		iFlag = pLoading->Loading_ForStage1();
		break;
	case LOADING_STAGE2:
		iFlag = pLoading->Loading_ForStage2();
		break;
	case LOADING_STAGE3:
		iFlag = pLoading->Loading_ForStage3();
		break;
	case LOADING_STAGE4:
		iFlag = pLoading->Loading_ForStage4();
		break;
	case LOADING_FINAL1:
		iFlag = pLoading->Loading_ForFinal1();
		break;
	case LOADING_BGTOOL:
		iFlag = pLoading->Loading_ForBGTool();
		break;
	}
	
	LeaveCriticalSection(&(pLoading->Get_Crt()));

	_endthreadex(0);

	return iFlag;
}

HRESULT CLoading::Ready_Loading(LOADINGID eID)
{
	InitializeCriticalSection(&m_Crt);

	m_hThread = (HANDLE)_beginthreadex(nullptr, 0, Thread_Main, this, 0, nullptr);

	m_eID = eID;

	return S_OK;
}

_uint CLoading::Loading_ForLogo(void)
{
	m_iLoadingTexImgNum = 0;
	//Sound 로딩오래걸릴경우 여기를 끄면됨
	Set_String(L"Sound/Bgm Loading..........");
	//FAILED_CHECK_RETURN(Engine::Ready_Sound(), E_FAIL);
	m_iLoadingTexImgNum = 1;
	Engine::PlayBGM(L"9.wav", 0.5f);
	Set_String(L"Sound/Effect Loading..........");
	//FAILED_CHECK_RETURN(Engine::Ready_Sound_Effect(), E_FAIL);
	m_iLoadingTexImgNum = 2;

	Set_String(L"Texture Loading....");
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Stage_Texture", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Resource/Texture/Export_Textures/Sprites/theme3BgSpr/theme3BgSpr_0.png")), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"None_Texture", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Resource/Texture/None/Alpha0.png")), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Player_Texture", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Resource/Texture/Export_Textures/Sprites/toodeeSpr/toodeeSpr_%d.png", 73)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Terrain_Texture", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Resource/Texture/Terrain/Grass_%d.tga", 2)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Tile_Texture", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Resource/Texture/Terrain/TileFloor.png")), E_FAIL);
	m_iLoadingTexImgNum = 3;
	// 검정색 타일(빈 공간 묘사 용)
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Stage1_Tile_Texture", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Resource/Texture/Terrain/TileBlack.png")), E_FAIL);
	//2D돼지
	//앞
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Pig_Texture", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Resource/Texture/Export_Textures/Sprites/pigSpr/correct-pigSpr-%d.png", 9)), E_FAIL);
	//뒤
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Back_Pig_Texture", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Resource/Texture/Export_Textures/Sprites/pigSpr/Back_pigSpr_%d.png", 9)), E_FAIL);
	//Key
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"KeyBox_Texture", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Resource/Texture/Export_Textures/Sprites/keySpr/keySpr_%d.png", 11)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Key_Texture", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Resource/Texture/Export_Textures/Sprites/keySpr/keySpr_%d.png", 21)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Spike_Texture", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Resource/Texture/Export_Textures/Sprites/spikesSpr/spikesSpr_%d.png", 13)), E_FAIL);
	m_iLoadingTexImgNum = 4;
	// 보스 총알 생성
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Fireball_Create_Texture", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Resource/Texture/Export_Textures/Sprites/b1FireballStaticSpr/b1FireballStaticSpr_0.png")), E_FAIL);
	// 보스 총알 발사
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Fireball_Texture", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Resource/Texture/Export_Textures/Sprites/b1FireballSpr/b1FireballSpr_%d.png", 9)), E_FAIL);
	m_iLoadingTexImgNum = 5;
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
	m_iLoadingTexImgNum = 6;
	//구름
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Cloud_Texture", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Resource/Texture/Export_Textures/Sprites/cloudSpr/Cloud_%d.png", 6)), E_FAIL);
	// 번개구름
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"LightningCloud_Texture", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Resource/Texture/Export_Textures/Sprites/lightningcloudSpr/cloudSpr_%d.png", 6)), E_FAIL);
	//번개
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Lightning_Texture", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Resource/Texture/Export_Textures/Sprites/lightningSpr/lightningSpr_%d.png", 9)), E_FAIL);
	// 스위치 큐브
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Switch_Texture", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Resource/Texture/Export_Textures/Sprites/buttonSpr/buttonSpr_%d.png", 2)), E_FAIL);
	m_iLoadingTexImgNum = 7;
	//탑디 머리통
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Topdee_Head", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Resource/Texture/Export_Textures/Sprites/topdeeSpinningHead/topdeeSpinningHead_%d.png", 36)), E_FAIL);
	//몸통
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Topdee_Body", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Resource/Texture/Export_Textures/Sprites/topdeeBody/topdeeSpr31_%d.png", 5)), E_FAIL);
	//팔다리용
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Topdee_Arm", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Resource/Texture/Export_Textures/Sprites/topdeeArm/topdeePalmSpr_%d.png", 2)), E_FAIL);

	//투키
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Tookee_Head", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Resource/Texture/Export_Textures/Sprites/tookeeSpinningHeadSpr/tookeeSpinningHeadSpr_%d.png", 12)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Tookee_Leg", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Resource/Texture/Export_Textures/Sprites/tookeeLegsSpr/tookeeLegsSpr_%d.png", 4)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Tookee_Arm", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Resource/Texture/Export_Textures/Sprites/tookeeHandsSpr/tookeeHandsSpr_%d.png", 2)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Tookee_Body", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Resource/Texture/Export_Textures/Sprites/tookeeTopSpr/tookeeTopSpr_0.png")), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Tookee_Too", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Resource/Texture/Export_Textures/Sprites/tookeeSpr/tookeeSpr_%d.png", 53)), E_FAIL);
	m_iLoadingTexImgNum = 8;

	//배경
	
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"BackCloud", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Resource/Texture/Export_Textures/Sprites/theme1CloudsSpr/theme1CloudsSpr_0.png")), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Map_Deco", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Resource/Texture/Export_Textures/Sprites/theme1DecorDownSpr/theme1DecorDownSpr_%d.png",3)), E_FAIL);



	//Boss2 머리털
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Boss2_Head", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Resource/Texture/Export_Textures/Sprites/boss2HeadSpr/boss2HeadSpr_%d.png", 6)), E_FAIL);
	//Boss2 공격용 팔
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Boss2_Hand", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Resource/Texture/Export_Textures/Sprites/boss2LimbSpr/Boss2_Hand.png")), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Boss2_Eye", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Resource/Texture/Export_Textures/Sprites/boss2EyeSpr/boss2EyeSpr_%d.png", 2)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Boss2_EyeBrow", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Resource/Texture/Export_Textures/Sprites/boss2EyebrowSpr/boss2EyebrowSpr_%d.png", 2)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Boss2_Face", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Resource/Texture/Export_Textures/Sprites/boss2FaceSpr/boss2FaceSpr_%d.png", 17)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Boss2_Jaw", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Resource/Texture/Export_Textures/Sprites/boss2JawSpr/boss2JawSpr_%d.png", 17)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Boss2_Nose", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Resource/Texture/Export_Textures/Sprites/boss2NoseSpr/boss2NoseSpr_%d.png", 9)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Boss2_Foot", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Resource/Texture/Export_Textures/Sprites/boss2HandSpr/boss2HandSpr_%d.png", 13)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Boss2_Chain", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Resource/Texture/Export_Textures/Sprites/boss2LimbSpr/boss2LimbSpr_%d.png", 2)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Boss2_Stone", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Resource/Texture/Export_Textures/Sprites/theme2pigStatueSpr/theme2pigStatueSpr_0.png")), E_FAIL);
	m_iLoadingTexImgNum = 9;
	// 3보스 눈썹 + 눈알
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Boss3_Eye", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Resource/Texture/Export_Textures/Sprites/boss3EyeSpr/boss3EyeSpr_%d.png", 3)), E_FAIL);
	// 3보스 입
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Boss3_Mouth", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Resource/Texture/Export_Textures/Sprites/boss3MouthSpr/boss3MouthSpr_%d.png", 3)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Boss3_Mouth_Shoot", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Resource/Texture/Export_Textures/Sprites/boss3MouthSpr/CompleteMouth/Lip%d.png", 6)), E_FAIL);
	// 3보스 사슬
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Boss3_HandPart", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Resource/Texture/Export_Textures/Sprites/boss3HandPartSpr/boss3HandPartSpr_%d.png", 6)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Boss3_HandPart_Shadow", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Resource/Texture/Export_Textures/Sprites/boss3HandPartSpr/boss3HandPart_ShadowSpr_%d.png", 3)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Boss3_Spark_Animation", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Resource/Texture/Export_Textures/Sprites/boss3ElectrictySpr/boss3ElectrictySpr_%d.png", 5)), E_FAIL);

	// 레이저 터렛
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Laser_Turret", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Resource/Texture/Export_Textures/Sprites/turretFireSpr/turretFireSpr_%d.png", 2)), E_FAIL);
	// 레이저
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Laser", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Resource/Texture/Export_Textures/Sprites/turretFireSpr/turretFireLaserSpr_0.png")), E_FAIL);

	Set_String(L"Buffer Loading.......");

	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"TriCol", CTriCol::Create(m_pGraphicDev)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"RcCol", CRcCol::Create(m_pGraphicDev)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"TerrainTex", CTerrainTex::Create(m_pGraphicDev)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Collider", CCollider::Create(m_pGraphicDev, true)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Rigidbody", CRigidbody::Create(m_pGraphicDev)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Shadow", CShadow::Create(m_pGraphicDev)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Animation", CAnimation::Create(m_pGraphicDev)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"BulletPool", CBulletPool::Create(m_pGraphicDev)), E_FAIL);

	m_iLoadingTexImgNum = 10;
	Set_String(L"CubeTex Loading..........");

	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"CubeTex", CCubeTex::Create(m_pGraphicDev)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"SkyBox_Texture", CTexture::Create(m_pGraphicDev, TEX_CUBE, L"../Resource/Texture/SkyBox/burger%d.dds", 4)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Move_Cube", CTexture::Create(m_pGraphicDev, TEX_CUBE, L"../Resource/Texture/SkyBox/NormalBox.dds")), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Texture_Cube", CTexture::Create(m_pGraphicDev, TEX_CUBE, L"../Resource/Texture/SkyBox/Texture.dds")), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"KeyBox_Cube", CTexture::Create(m_pGraphicDev, TEX_CUBE, L"../Resource/Texture/SkyBox/KeyBox.dds")), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Stage_Cube", CTexture::Create(m_pGraphicDev, TEX_CUBE, L"../Resource/Texture/SkyBox/Stage_%d.dds", 7)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"CrackBlock_Cube", CTexture::Create(m_pGraphicDev, TEX_CUBE, L"../Resource/Texture/SkyBox/CrackBlock.dds")), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"SwitchCube_Cube", CTexture::Create(m_pGraphicDev, TEX_CUBE, L"../Resource/Texture/SkyBox/SwitchCube.dds")), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Portal_Cube", CTexture::Create(m_pGraphicDev, TEX_CUBE, L"../Resource/Texture/SkyBox/PortalCube.dds")), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Gravity_Cube", CTexture::Create(m_pGraphicDev, TEX_CUBE, L"../Resource/Texture/SkyBox/GravBox.dds")), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Stage3_Boss_Cube", CTexture::Create(m_pGraphicDev, TEX_CUBE, L"../Resource/Texture/SkyBox/Stage3Boss.dds")), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Stage3_Boss_Hand_Cube", CTexture::Create(m_pGraphicDev, TEX_CUBE, L"../Resource/Texture/SkyBox/Boss3Hand%d.dds", 2)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Stage3_Boss_Hand_Blank_Cube", CTexture::Create(m_pGraphicDev, TEX_CUBE, L"../Resource/Texture/SkyBox/None.dds")), E_FAIL);
	m_iLoadingTexImgNum = 11;
	Set_String(L"Particle Loading..........");

	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"BlockExp", CTexParticle::Create(m_pGraphicDev,
		L"../Resource/Texture/Export_Textures/Sprites/blockExpSpr/blockExpSpr_%d.png",
		9, 0.8f, 1, false)), E_FAIL);

	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"ShineParticle", CTexParticle::Create(m_pGraphicDev,
		L"../Resource/Texture/Export_Textures/Sprites/shineBiggerSpr/shineBiggerSpr_0.png", 1,
		10.f, 1, false)), E_FAIL);

	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"BossScream", CTexParticle::Create(m_pGraphicDev,
		L"../Resource/Texture/Export_Textures/Sprites/bossScream/ScreamParticle1.png", 1,
		1.f, 1, false)), E_FAIL);

	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"SparkParticle", CSparkParticle::Create(m_pGraphicDev,
		L"../Resource/Texture/Export_Textures/Sprites/sparkSpr/SparkSpr_0%d.png",
		10, 1.f, 2, false)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"WalkParticle", CSparkParticle::Create(m_pGraphicDev,
		L"../Resource/Texture/Export_Textures/Sprites/particleSpr/particleSpr_0.png",
		1, 0.8f, 2, false)), E_FAIL);

	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"CircularParticle", CCircularParticle::Create(m_pGraphicDev,
		L"../Resource/Texture/Export_Textures/Sprites/particleSpr/particleSpr_0.png", 1,
		0.7f, 10, false, 1.f, 10.f)), E_FAIL);
	// 2보스 손찍기 파티클
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Boss2LandParticle", CCircularParticle::Create(m_pGraphicDev,
		L"../Resource/Texture/Export_Textures/Sprites/particleSpr/particleSpr_0.png", 1,
		10.f, 20, false, 1.f, 20.f)), E_FAIL);
	// 2보스 땅찍기원형파티클
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"CircleParticle", CCircleParticle::Create(m_pGraphicDev,
		L"../Resource/Texture/Export_Textures/Sprites/blockExpSpr/blockExpSpr_1.png", 1,
		1.f, 200, false, 10.f)), E_FAIL);
	// 2보스 점프 파티클
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Boss2JumpParticle", CJumpParticle::Create(m_pGraphicDev,
		L"../Resource/Texture/Export_Textures/Sprites/particleSpr/particleSpr_0.png", 1,
		4.f, 40, false)), E_FAIL);
	// 3보스 불덩이 소환 파티클
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"FireParticle", CCircularParticle::Create(m_pGraphicDev,
		L"../Resource/Texture/Export_Textures/Sprites/b1FireballStaticSpr/b1FireballStaticSpr_0.png", 1,
		1.f, 5, false, 1.f, 10.f)), E_FAIL);
	// 3보스 전기 파티클
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"SparkCircularParticle", CCircularParticle::Create(m_pGraphicDev,
		L"../Resource/Texture/Export_Textures/Sprites/boss3SparkSpr/boss3SparkSpr_%d.png", 5,
		2.f, 20, false, 1.f, 10.f)), E_FAIL);
	// 3보스 파란전기 파티클
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"ElectrictCircularParticle", CCircularParticle::Create(m_pGraphicDev,
		L"../Resource/Texture/Export_Textures/Sprites/boss3ElectrictySpr/boss3ElectrictySpr_%d.png", 5,
		2.f, 20, false, 1.f, 10.f)), E_FAIL);

	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"RazorColParticle", CCircleParticle::Create(m_pGraphicDev,
		L"../Resource/Texture/Export_Textures/Sprites/particleSpr/particleSpr_0.png", 1,
		0.5f, 10, false, 1.f)), E_FAIL);

	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"JumpParticle", CJumpParticle::Create(m_pGraphicDev,
		L"../Resource/Texture/Export_Textures/Sprites/particleSpr/particleSpr_0.png", 1,
		1.f, 15, false)), E_FAIL);

	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"LandingParticle", CLandingParticle::Create(m_pGraphicDev,
		L"../Resource/Texture/Export_Textures/Sprites/particleSpr/particleSpr_0.png", 1,
		1.f, 4, false)), E_FAIL);

	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"SlerpParticle", CSlerpParticle::Create(m_pGraphicDev,
		L"../Resource/Texture/Export_Textures/Sprites/sparkSpr/SparkSpr_0%d.png", 10,
		1.f, 5, false)), E_FAIL);

	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"LeafParticle0", CLeafParticle::Create(m_pGraphicDev,
		L"../Resource/Texture/Export_Textures/Sprites/theme2LeafSpr/theme2LeafSpr_0.png", 1,
		1.f, 4, true)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"LeafParticle1", CLeafParticle::Create(m_pGraphicDev,
		L"../Resource/Texture/Export_Textures/Sprites/theme2LeafSpr/theme2LeafSpr_1.png", 1,
		1.f, 4, true)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"LeafParticle2", CLeafParticle::Create(m_pGraphicDev,
		L"../Resource/Texture/Export_Textures/Sprites/theme2LeafSpr/theme2LeafSpr_2.png", 1,
		1.f, 4, true)), E_FAIL);

	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"RainParticle", CRainParticle::Create(m_pGraphicDev,
		L"../Resource/Texture/Export_Textures/Sprites/RainSpr/RainSpr_0.png", 1,
		0.7f, 15, true)), E_FAIL);

	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Line", CLine::Create(m_pGraphicDev)), E_FAIL);

	Set_String(L"Logo Loading..........");
	m_pScene = CLogo::Create(m_pGraphicDev);
	dynamic_cast<CPreStage*>(Engine::Get_Scene())->Set_Scene(m_pScene);
	m_bFinish = true;
	m_iLoadingTexImgNum = 12;
	Set_String(L"Loading Complete!!!!!!!!");

	return 0;
}

_uint CLoading::Loading_ForStage1(void)
{
	Set_String(L"Stage Loading..........");
	m_pScene = CStage1::Create(m_pGraphicDev);
	dynamic_cast<CPreStage*>(Engine::Get_Scene())->Set_Scene(m_pScene);
	m_bFinish = true;
	m_iLoadingTexImgNum = 13;
	Set_String(L"Loading1 Complete!!!!!!!!");
	return 0;
}

_uint CLoading::Loading_ForStage2(void)
{
	Set_String(L"Stage Loading..........");

	m_pScene = CStage2::Create(m_pGraphicDev);
	dynamic_cast<CPreStage*>(Engine::Get_Scene())->Set_Scene(m_pScene);

	m_bFinish = true;
	m_iLoadingTexImgNum = 13;
	Set_String(L"Loading2 Complete!!!!!!!!");
	return 0;
}

_uint CLoading::Loading_ForStage3(void)
{
	Set_String(L"Stage Loading..........");

	m_pScene = CStage3::Create(m_pGraphicDev);
	dynamic_cast<CPreStage*>(Engine::Get_Scene())->Set_Scene(m_pScene);

	m_bFinish = true;
	m_iLoadingTexImgNum = 13;
	Set_String(L"Loading3 Complete!!!!!!!!");
	return 0;
}

_uint CLoading::Loading_ForStage4(void)
{
	Set_String(L"Stage Loading..........");

	m_pScene = CStage4::Create(m_pGraphicDev);
	dynamic_cast<CPreStage*>(Engine::Get_Scene())->Set_Scene(m_pScene);

	m_bFinish = true;
	m_iLoadingTexImgNum = 13;
	Set_String(L"Loading4 Complete!!!!!!!!");
	return 0;
}

_uint CLoading::Loading_ForFinal1(void)
{
	Set_String(L"Stage Loading..........");

	m_pScene = CFinalStage1::Create(m_pGraphicDev);
	dynamic_cast<CPreStage*>(Engine::Get_Scene())->Set_Scene(m_pScene);

	m_bFinish = true;
	m_iLoadingTexImgNum = 13;
	Set_String(L"Loading Final Complete!!!!!!!!");
	return 0;
}

_uint CLoading::Loading_ForBGTool(void)
{
	Set_String(L"Stage Loading..........");

	m_pScene = CBackGroundToolScene::Create(m_pGraphicDev);
	dynamic_cast<CPreStage*>(Engine::Get_Scene())->Set_Scene(m_pScene);

	m_bFinish = true;
	m_iLoadingTexImgNum = 13;
	Set_String(L"Loading BGTool Complete!!!!!!!!");
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
