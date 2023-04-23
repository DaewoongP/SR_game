#include "stdafx.h"
#include "BulletPool.h"

#include "Export_Function.h"
//#include "../Client/Laser.h"

CBulletPool::CBulletPool(LPDIRECT3DDEVICE9 pGraphicDev)
	:CComponent(pGraphicDev),
	m_bCreate(true)
{
	// ������ �������ۿ� ����. bullet Ŭ���� ���� �Ѿ˵� ���� ��� ó�� �ʿ�
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
	// ���� 1�� �Ѿ� ����
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

	// ���� ��������� ���� �Ѿ� �� ���� �տ� �ִ� �Ѿ��� ã��
	auto iter = find_if(m_vecBulletPool.begin(), m_vecBulletPool.end(), [](auto& iter)->_bool {
		return iter.second == false;
	});

	// ������ ����
	if (iter == m_vecBulletPool.end())
		return nullptr;

	// ������ �ش� �Ѿ��� second�� true�� �ٲ� (������̶�� �ǹ�)
	/*iter->second = true;
	dynamic_cast<CLaser*>(iter->first)->Set_Use(true);*/

	// ��ġ ����
	iter->first->m_pTransform->m_vInfo[INFO_POS] = vPos;

	// �� �Ѿ��� ����
	return iter->first;
}

void CBulletPool::UnUse_Bullet()
{
	// ������ �����ϰ� second�� false(��Ȱ��ȭ)�� �ٲ�
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
