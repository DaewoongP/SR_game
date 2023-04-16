#include "stdafx.h"
#include "AbstractFactory.h"
#include "EarthQuakeSystem.h"


CEarthQuakeSystem::CEarthQuakeSystem(LPDIRECT3DDEVICE9 pGraphicDev)
	:CGameObject(pGraphicDev)
{
	m_QuakeTimer = 1;
}

CEarthQuakeSystem::~CEarthQuakeSystem()
{
}

HRESULT CEarthQuakeSystem::Ready_GameObject(_vec3 & vPos)
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);
	m_QuakeTimer = 1;

	
	//4방향으로 나아갈 퀘이크 소환
	_vec3 dir[4] = { {1,0,0},{-1,0,0},{0,1,0},{0,-1,0} };
	for (int i = 0; i < 4; i++)
	{
		CreateQuake(_vec3(0,0,0), dir[i]);
	}

	return S_OK;
}

_int CEarthQuakeSystem::Update_GameObject(const _float & fTimeDelta)
{
	m_dwLifeTime -= fTimeDelta;
	//죽을 때면 죽으면서 새로운 퀘이크 소환해주세요.
	if (m_dwLifeTime < 0)
		SummonNewQuake();
	
	__super::Update_GameObject(fTimeDelta);
	
	if (m_EarthQuake.size() == 0)
		m_bDead = true;

	return 0;
}

void CEarthQuakeSystem::Render_GameObject(void)
{
}

void CEarthQuakeSystem::SwapTrigger()
{
}

HRESULT CEarthQuakeSystem::Add_Component(void)
{
	return S_OK;
}

void CEarthQuakeSystem::SummonNewQuake()
{
	if (m_EarthQuake.size() != 0)
	{
		for (int i = 0; i < m_EarthQuake.size(); i++)
		{
			_vec3 _dir[3];
			_vec3 temp = m_EarthQuake[i]->GetDir();
			if (temp.x != 0)
			{
				_dir[0] = temp + _vec3(0, -1, 0);
				_dir[1] = temp + _vec3(0, 0, 0);
				_dir[2] = temp + _vec3(0, 1, 0);
			}
			else 
			{
				_dir[0] = temp + _vec3(-1, 0, 0);
				_dir[1] = temp + _vec3(0, 0, 0);
				_dir[2] = temp + _vec3(1, 0, 0);
			}
			 
			for (int j = 0; j < m_EarthQuake.size(); j++)
			{
				//3방향 체크
				//약진하려는 방향으로 이동할수있다면? == 방향에 존재하는 퀘이크가 없다면?
				if (m_EarthQuake[i]->m_pTransform->m_vInfo[INFO_POS] + _dir[0])
					CreateQuake(m_EarthQuake[i]->m_pTransform->m_vInfo[INFO_POS] + _dir[0],_dir[0]);
				if (m_EarthQuake[i]->m_pTransform->m_vInfo[INFO_POS] + _dir[1])
					CreateQuake(m_EarthQuake[i]->m_pTransform->m_vInfo[INFO_POS] + _dir[1],_dir[1]);
				if (m_EarthQuake[i]->m_pTransform->m_vInfo[INFO_POS] + _dir[2])
					CreateQuake(m_EarthQuake[i]->m_pTransform->m_vInfo[INFO_POS] + _dir[2],_dir[2]);

				m_EarthQuake[i]->m_bDead = true;
			}
		}
	}
	m_dwLifeTime = 1;
}

void CEarthQuakeSystem::CreateQuake(_vec3 vPos,_vec3 dir)
{
	CLayer* pStageLayer = dynamic_cast<CLayer*>(Engine::Get_Layer(L"Layer_GameLogic"));

	CGameObject* pGameObject = CEarthQuake::Create(m_pGraphicDev, vPos);
	pGameObject->Sort_Component();
	dynamic_cast<CEarthQuake*>(pGameObject)->SetDir(dir);

	m_EarthQuake.push_back(dynamic_cast<CEarthQuake*>(pGameObject));
}

CEarthQuakeSystem * CEarthQuakeSystem::Create(LPDIRECT3DDEVICE9 pGraphicDev, _vec3 & vPos)
{
	CEarthQuakeSystem*		pInstance = new CEarthQuakeSystem(pGraphicDev);

	if (FAILED(pInstance->Ready_GameObject(vPos)))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CEarthQuakeSystem::Free(void)
{
	__super::Free();
}
