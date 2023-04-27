#include "stdafx.h"
#include "FinalStoneCube.h"
#include "..\Engine\AbstractFactory.h"
#include "Cube.h"
#include "Boss3.h"
#include "StageCamera.h"

CFinalStoneCube::CFinalStoneCube(LPDIRECT3DDEVICE9 pGraphicDev)
	:CGameObject(pGraphicDev), m_bTrigger(false), m_iBossSpawn(0), m_bStart(false), m_bEnd(false)
{
}

CFinalStoneCube::~CFinalStoneCube()
{
}

HRESULT CFinalStoneCube::Ready_GameObject(_vec3 & vPos, CLayer* pLayer)
{
	m_pGameLogicLayer = pLayer;
	vPos.y = -50.f;
	m_vInitPos = vPos;
	Ready_MapCubes();
	sort(m_vecCube.begin(), m_vecCube.end(), [&](auto& src, auto& dest)->_bool {
		return D3DXVec3Length(&(src.second - vPos)) > D3DXVec3Length(&(dest.second - vPos));
	});
	m_fLerp = 0.f;
	for (size_t i = 0; i < m_vecCube.size(); ++i)
		m_vecLerp.push_back( i / 300.f);
	m_fSpeed = 0.5f;
	
	m_pLandingParticle = dynamic_cast<CCircularParticle*>(Engine::Clone_Proto(L"CircularParticle", this));
	NULL_CHECK_RETURN(m_pLandingParticle, E_FAIL);
	m_vecComponent[ID_STATIC].push_back({ L"CircularParticle", m_pLandingParticle });

	Engine::Ready_Frame(L"0.1Sec", 10.f);
	return S_OK;
}

_int CFinalStoneCube::Update_GameObject(const _float & fTimeDelta)
{
	if (m_bTrigger)
		return 0;
	Add_RenderGroup(RENDER_ALPHA, this);
	if (m_bEnd)
		if (End_Pattern(fTimeDelta))
			return OBJ_DEAD;
		else
			return 0;
	g_Is2D = false;
	for (size_t i = 0; i < m_vecCube.size(); ++i)
	{
		Cube_Lerp(fTimeDelta, i);
	}

	if (m_iBossSpawn > 0)
		m_iBossSpawn = 0;
	else
		Make_Boss3();

	if (nullptr != m_pBoss && false == m_bStart)
	{
		m_fLerp += fTimeDelta;
		if (m_fLerp >= 1.f)
		{
			m_fLerp = 1.f;
			dynamic_cast<CStage1Camera*>(Engine::Get_GameObject(L"Layer_Environment", L"Camera"))->Start_Camera_Shake(0.8f, 60.0f, SHAKE_ALL, false);
			Make_Particle();
			m_bStart = true;
		}
		D3DXVec3Lerp(&m_pBoss->m_pTransform->m_vInfo[INFO_POS], &_vec3(30.f, 16.f, -20.f), &_vec3(30.f, 16.f, 8.f), m_fLerp);
	}
	
	if (m_bStart && false == m_bEnd)
	{
		if (Start_BossPattern(fTimeDelta))
		{
			for (size_t i = 0; i < m_vecCube.size(); ++i)
				m_vecLerp[i] = GetRandomFloat(2.5f, 5.f);
			m_bEnd = true;
		}			
	}
	
	__super::Update_GameObject(fTimeDelta);
	return 0;
}

void CFinalStoneCube::Render_GameObject()
{
	m_pLandingParticle->Update_Particle();
	__super::Render_GameObject();
}

HRESULT CFinalStoneCube::Ready_MapCubes()
{
	CCube* cube = nullptr;
	_vec3 vPos;
	for (size_t i = 0; i < 8; ++i)
	{
		for (size_t j = 0; j < (i + 2) * 2; ++j)
		{
			vPos = _vec3((28 - 2.f * i) + 2.f * j, 34 - 2.f * i, LERPCUBEZ);
			cube = CCube::Create(m_pGraphicDev, vPos, 6);
			m_pGameLogicLayer->Add_GameObject(L"MapCube", cube);
			m_vecCube.push_back({ cube, vPos });

			vPos = _vec3((28 - 2.f * i) + 2.f * j, 0 + 2.f * i, LERPCUBEZ);
			cube = CCube::Create(m_pGraphicDev, vPos, 6);
			m_pGameLogicLayer->Add_GameObject(L"MapCube", cube);
			m_vecCube.push_back({ cube, vPos });
		}
	}
	for (size_t i = 0; i < 18; ++i)
	{
		vPos = _vec3(14.f + 2 * i, 18.f, LERPCUBEZ);
		cube = CCube::Create(m_pGraphicDev, vPos, 6);
		m_pGameLogicLayer->Add_GameObject(L"MapCube", cube);
		m_vecCube.push_back({ cube, vPos });
	}
	for (size_t i = 0; i < 18; ++i)
	{
		vPos = _vec3(14.f + 2 * i, 16.f, LERPCUBEZ);
		cube = CCube::Create(m_pGraphicDev, vPos, 6);
		m_pGameLogicLayer->Add_GameObject(L"MapCube", cube);
		m_vecCube.push_back({ cube, vPos });
	}

	for (int i = 0; i < CUBEY; i++)
	{
		for (int j = 0; j < CUBEX; j++)
		{
			//맨 윗줄
			if (i == 0)
			{
				vPos = _vec3(j * 2.f, i * 2.f, LERPCUBEZ - 2.f);
				cube = CCube::Create(m_pGraphicDev, vPos, 6);
				m_pGameLogicLayer->Add_GameObject(L"MapCube", cube);
				m_vecCube.push_back({ cube, vPos });
			}
			//사이 첫줄
			if (i == CUBEY - 1)
			{
				vPos = _vec3(j * 2.f, i * 2.f, LERPCUBEZ - 2.f);
				cube = CCube::Create(m_pGraphicDev, vPos, 6);
				m_pGameLogicLayer->Add_GameObject(L"MapCube", cube);
				m_vecCube.push_back({ cube, vPos });
			}
			//사이 마지막줄
			if (j == 0)
			{
				vPos = _vec3(j * 2.f, i * 2.f, LERPCUBEZ - 2.f);
				cube = CCube::Create(m_pGraphicDev, vPos, 6);
				m_pGameLogicLayer->Add_GameObject(L"MapCube", cube);
				m_vecCube.push_back({ cube, vPos });
			}
			//맨 아랫줄
			if (j == CUBEX - 1)
			{
				vPos = _vec3(j * 2.f, i * 2.f, LERPCUBEZ - 2.f);
				cube = CCube::Create(m_pGraphicDev, vPos, 6);
				m_pGameLogicLayer->Add_GameObject(L"MapCube", cube);
				m_vecCube.push_back({ cube, vPos });
			}
		}
	}
	return S_OK;
}

void CFinalStoneCube::Cube_Lerp(const _float& fTimeDelta, _int iIndex)
{
	m_vecLerp[iIndex] += fTimeDelta * m_fSpeed;
	if (m_vecLerp[iIndex] >= 1.f)
		m_vecLerp[iIndex] = 1.f;
	else
		m_iBossSpawn += 1;
	D3DXVec3Lerp(&m_vecCube[iIndex].first->m_pTransform->m_vInfo[INFO_POS],
		&m_vecCube[iIndex].second,
		&(m_vecCube[iIndex].second + _vec3(0.f, 0.f, 12.f - LERPCUBEZ)), m_vecLerp[iIndex]);
}

void CFinalStoneCube::Make_Boss3()
{
	if (nullptr == m_pBoss)
	{
		m_pBoss = CBoss3::Create(m_pGraphicDev, _vec3(30.f, 16.f, -20.f), true);
		m_pGameLogicLayer->Add_GameObject(L"Boss3", m_pBoss);
		m_pBoss->CreateParts(m_pGameLogicLayer);
	}
}

_bool CFinalStoneCube::Start_BossPattern(const _float& fTimeDelta)
{
	if (m_pBoss->IsPatternEnd())
		return true;
	m_pBoss->SpeedUp_TopTime(fTimeDelta);
	m_pBoss->FollowPlayer(fTimeDelta);
	return false;
}

_bool CFinalStoneCube::End_Pattern(const _float & fTimeDelta)
{
	_vec3 vDir = _vec3(0,0,1);
	m_fTime += fTimeDelta;
	for (size_t i = 0; i < m_vecCube.size(); ++i)
	{
		if (m_fTime > m_vecLerp[i])
		{
			dynamic_cast<CStage1Camera*>(Engine::Get_GameObject(L"Layer_Environment", L"Camera"))->Start_Camera_Shake(1.f, 40.0f, SHAKE_ALL);
			m_vecCube[i].first->m_pTransform->m_vInfo[INFO_POS] += vDir * fTimeDelta * 50.f;
		}
	}
	if (m_pBoss != nullptr && m_fTime > 5.f)
	{
		m_pBoss->Off_Shadow();
		m_pBoss->m_pTransform->m_vInfo[INFO_POS] += vDir * fTimeDelta * 50.f;
		m_pBoss->Move_Hands(vDir, fTimeDelta * 50.f);
	}

	if (m_pBoss != nullptr && m_pBoss->m_pTransform->m_vInfo[INFO_POS].z >= 200.f)
		return true;
	return false;
}

void CFinalStoneCube::Make_Particle()
{
	if (nullptr == m_pBoss)
		return;
	BoundingBox box;
	_vec3 vPos = m_pBoss->m_pTransform->m_vInfo[INFO_POS];
	box.Offset(vPos);
	m_pLandingParticle->Set_Size(2.f);
	m_pLandingParticle->Set_Options(2.f, 25.f);
	m_pLandingParticle->Set_SizeLifeTime(1.f);
	m_pLandingParticle->Set_BoundingBox(box);
	m_pLandingParticle->Start_Particle();
}

CFinalStoneCube * CFinalStoneCube::Create(LPDIRECT3DDEVICE9 pGraphicDev, _vec3 & vPos, CLayer* pLayer)
{
	CFinalStoneCube* pInstance = new CFinalStoneCube(pGraphicDev);
	if (FAILED(pInstance->Ready_GameObject(vPos, pLayer)))
	{
		Safe_Release(pInstance);
		return nullptr;
	}
	return pInstance;
}

void CFinalStoneCube::Free()
{
	__super::Free();
}
