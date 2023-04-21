#include "stdafx.h"
#include "GiantHand.h"
#include "AbstractFactory.h"
#include "Boss1Parts.h"

CGiantHand::CGiantHand(LPDIRECT3DDEVICE9 pGraphicDev) :CGameObject(pGraphicDev)
{
	m_bInit = true;
}

CGiantHand::~CGiantHand()
{
}

HRESULT CGiantHand::Ready_GameObject(_vec3 & vPos)
{
	//왕손은 보스가 만드는게 아니라 보스랑 개별 패턴임. 쉽게 말해서 맵 기믹 z가 -7.f이 되면 지면과 닿은거임.
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);
	m_pCollider->Set_BoundingBox({2,35,35});
	m_pCollider->Set_BoundOffset(_vec3(0,-8,+0.5f));
	m_pTransform->m_vInfo[INFO_POS] = vPos;
	m_pTransform->m_vScale = _vec3(30, 30, 30);
	m_pTransform->m_vAngle = _vec3(D3DXToRadian(-90), D3DXToRadian(90), 0);
	
	//테스트용으로 찍기를 만듬.
	//if()
	

	
	return S_OK;
}

_int CGiantHand::Update_GameObject(const _float & fTimeDelta)
{
	if (m_bInit)
	{
		//실행해주는 코드
		CLayer* pStageLayer = dynamic_cast<CLayer*>(Engine::Get_Layer(L"Layer_GameLogic"));
		NULL_CHECK_RETURN(pStageLayer, E_FAIL);
		for (int i = 0; i < 7; i++)
			FAILED_CHECK_RETURN(FACTORY<CBoss1Parts>::Create(L"Boss1Parts", pStageLayer, _vec3(0, 0, i*0.4f), m_pTransform, L"Boss1_Pattern03", i, false), E_FAIL);
		m_bInit = false;
	}
	Engine::Add_RenderGroup(RENDER_NONALPHA, this);

	//z가 지면의 위치거나
	//move가능한 큐브와 충돌했을 경우 멈춥니다.
	//if(m_pTransform->m_vInfo[INFO_POS].z == 11)

	__super::Update_GameObject(fTimeDelta);
	return 0;
}

void CGiantHand::LateUpdate_GameObject(void)
{
	__super::LateUpdate_GameObject();
}

void CGiantHand::Render_GameObject(void)
{
	__super::Render_GameObject();
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransform->Get_WorldMatrixPointer());
}

HRESULT CGiantHand::Add_Component(void)
{
	CComponent*		pComponent = nullptr;
	pComponent = m_pCollider = dynamic_cast<CCollider*>(Engine::Clone_Proto(L"Collider", this));
	NULL_CHECK_RETURN(m_pCollider, E_FAIL);
	m_vecComponent[ID_DYNAMIC].push_back({ L"Collider", pComponent });
	return S_OK;
}

CGiantHand * CGiantHand::Create(LPDIRECT3DDEVICE9 pGraphicDev, _vec3 & vPos)
{
	CGiantHand*		pInstance = new CGiantHand(pGraphicDev);

	if (FAILED(pInstance->Ready_GameObject(vPos)))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CGiantHand::Free(void)
{
	__super::Free();
}
