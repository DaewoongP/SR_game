#include "stdafx.h"
#include "Boss1HeadSub.h"
#include "AbstractFactory.h"
#include "Boss1Parts.h"
#include "Thirddee.h"
CBoss1HeadSub::CBoss1HeadSub(LPDIRECT3DDEVICE9 pGraphicDev) :CGameObject(pGraphicDev)
{
	m_bInit = true;
}

CBoss1HeadSub::~CBoss1HeadSub()
{
}

HRESULT CBoss1HeadSub::Ready_GameObject(_vec3 & vPos,_vec3 & vtoward)
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);
	m_pTransform->m_vInfo[INFO_POS] = vPos;
	m_pTransform->m_vScale = _vec3(5, 5, 5);
	m_pTransform->m_vAngle = _vec3(D3DXToRadian(-90), D3DXToRadian(90), 0);
	m_pCollider->Set_BoundingBox({ 2,6,6 }, { 0,0,2 });
	m_vMoveDir = vtoward;
	m_fSpeed = -0.7f;
	return S_OK;
}

_int CBoss1HeadSub::Update_GameObject(const _float & fTimeDelta)
{
	if (m_bDead)
		return -1;

	if (m_bInit)
	{
		//실행해주는 코드
		CLayer* pStageLayer = dynamic_cast<CLayer*>(Engine::Get_Layer(L"Layer_GameLogic"));
		NULL_CHECK_RETURN(pStageLayer, E_FAIL);
		for (int i = 0; i < 8; i++)
		{
			FAILED_CHECK_RETURN(FACTORY<CBoss1Parts>::Create(L"Boss1Parts", pStageLayer, _vec3(0, 0, i*0.4f), m_pTransform, L"Boss1_Pattern01", i, false), E_FAIL);
			m_PartsVec.push_back(m_pTransform->GetChild(i));
		}
		m_bInit = false;
	}

	m_pTransform->m_vInfo[INFO_POS] += m_vMoveDir*m_fSpeed;

	Engine::Add_RenderGroup(RENDER_NONALPHA, this);
	__super::Update_GameObject(fTimeDelta);
	return 0;
}

void CBoss1HeadSub::LateUpdate_GameObject(void)
{
	__super::LateUpdate_GameObject();
}

void CBoss1HeadSub::Render_GameObject(void)
{
	__super::Render_GameObject();
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransform->Get_WorldMatrixPointer());
}

void CBoss1HeadSub::OnCollisionEnter(const Collision * collision)
{
	THIRDDEEDIE;
}

HRESULT CBoss1HeadSub::Add_Component(void)
{
	CComponent*		pComponent = nullptr;
	pComponent = m_pCollider = dynamic_cast<CCollider*>(Engine::Clone_Proto(L"Collider", this));
	NULL_CHECK_RETURN(m_pCollider, E_FAIL);
	m_vecComponent[ID_DYNAMIC].push_back({ L"Collider", pComponent });
	return S_OK;
}

CBoss1HeadSub * CBoss1HeadSub::Create(LPDIRECT3DDEVICE9 pGraphicDev, _vec3 & vPos, _vec3 & vtoward)
{
	CBoss1HeadSub*		pInstance = new CBoss1HeadSub(pGraphicDev);

	if (FAILED(pInstance->Ready_GameObject(vPos, vtoward)))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CBoss1HeadSub::Free(void)
{
	for (int i = 0; i < m_PartsVec.size(); i++)
		m_PartsVec[i]->m_pGameObject->Set_Dead();
	__super::Free();
}
