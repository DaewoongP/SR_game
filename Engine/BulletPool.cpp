#include "stdafx.h"
#include "BulletPool.h"

#include "Export_Function.h"
//#include "../Client/Laser.h"

CBulletPool::CBulletPool(LPDIRECT3DDEVICE9 pGraphicDev)
	:CComponent(pGraphicDev),
	m_bCreate(true)
{
	// 지금은 레이저밖에 못씀. bullet 클래스 만들어서 총알들 전부 상속 처리 필요
	m_vecBulletPool.reserve(20);
}

CBulletPool::~CBulletPool()
{
}

HRESULT CBulletPool::Ready_BulletPool()
{
	return  S_OK;
}

CGameObject* CBulletPool::Use_Bullet(_vec3& vPos, _int eDir)
{
	// 최초 1번 총알 생성
	if (m_bCreate)
	{
		CLayer* pLayer = Get_Layer(L"Layer_GameLogic");
		NULL_CHECK_RETURN(pLayer, nullptr);

		m_pGraphicDev = Get_GraphicDev();
		NULL_CHECK_RETURN(m_pGraphicDev, nullptr);

		for (int i = 0; i < 20; ++i)
		{
			//CGameObject* pGameObject = CLaser::Create(m_pGraphicDev, _vec3{ 0.f, 0.f, 0.f }, eDir);
			//NULL_CHECK_RETURN(pGameObject, );
			//pGameObject->Sort_Component();
			//FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Laser", pGameObject), );

			//m_vecBulletPool.push_back(pair<CGameObject*, _bool>(pGameObject, false));
		}

		m_bCreate = false;
	}

	// 현재 사용중이지 않은 총알 중 제일 앞에 있는 총알을 찾음
	auto iter = find_if(m_vecBulletPool.begin(), m_vecBulletPool.end(), [](auto& iter)->_bool {
		return iter.second == false;
	});

	// 없으면 리턴
	if (iter == m_vecBulletPool.end())
		return nullptr;

	// 있으면 해당 총알의 second를 true로 바꿈 (사용중이라는 의미)
	/*iter->second = true;
	dynamic_cast<CLaser*>(iter->first)->Set_Use(true);*/

	// 위치 설정
	iter->first->m_pTransform->m_vInfo[INFO_POS] = vPos;

	// 그 총알을 리턴
	return iter->first;
}

void CBulletPool::UnUse_Bullet()
{
	// 재사용이 가능하게 second를 false(비활성화)로 바꿈
	for (auto& iter : m_vecBulletPool)
	{
		/*if (!dynamic_cast<CLaser*>(iter.first)->Get_Use())
			iter.second = false;*/
	}
}

CBulletPool * CBulletPool::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CBulletPool *	pInstance = new CBulletPool(pGraphicDev);

	if (FAILED(pInstance->Ready_BulletPool()))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

CComponent * CBulletPool::Clone(void)
{
	return new CBulletPool(*this);
}

void CBulletPool::Free()
{
	__super::Free();
}
