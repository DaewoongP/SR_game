#include "stdafx.h"
#include "Boss1Head.h"
#include "AbstractFactory.h"
#include "Boss1Parts.h"
#include "Boss1HeadSub.h"

CBoss1Head::CBoss1Head(LPDIRECT3DDEVICE9 pGraphicDev) :CGameObject(pGraphicDev)
{
	m_bInit = true;
}

CBoss1Head::~CBoss1Head()
{
}

HRESULT CBoss1Head::Ready_GameObject(_vec3 & vPos, _vec3 vToWard)
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	m_fMovepos[0] = 30;
	m_fMovepos[1] = 15;
	m_fMovepos[2] = 00;

	m_pTransform->m_vInfo[INFO_POS] = vPos;
	m_pTransform->m_vScale = _vec3(5, 5, 5);
	m_pTransform->m_vAngle = _vec3(D3DXToRadian(-90), D3DXToRadian(90), 0);
	m_fSpeed = 4;
	return S_OK;
}

_int CBoss1Head::Update_GameObject(const _float & fTimeDelta)
{
	if (m_bDead)
		return -1;
	if (m_bInit)
	{
		CLayer* pStageLayer = dynamic_cast<CLayer*>(Engine::Get_Layer(L"Layer_GameLogic"));
		NULL_CHECK_RETURN(pStageLayer, E_FAIL);
		for (int i = 0; i < 8; i++)
		{
			FAILED_CHECK_RETURN(FACTORY<CBoss1Parts>::Create(L"Boss1Parts", pStageLayer, _vec3(0, 0, i*0.4f), m_pTransform, L"Boss1_Pattern01", i, false), E_FAIL);
			m_PartsVec.push_back(m_pTransform->GetChild(i));
		}
		m_bInit = false;
	}

	ShootHead(fTimeDelta);

	Engine::Add_RenderGroup(RENDER_NONALPHA, this);
	__super::Update_GameObject(fTimeDelta);
	return 0;
}

void CBoss1Head::LateUpdate_GameObject(void)
{
	__super::LateUpdate_GameObject();
}

void CBoss1Head::Render_GameObject(void)
{
	__super::Render_GameObject();
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransform->Get_WorldMatrixPointer());
}

_bool CBoss1Head::isMoveDone(const _float & fTimeDelta)
{
	m_pTransform->m_vInfo[INFO_POS].y = Lerp(m_pTransform->m_vInfo[INFO_POS].y, m_fMovepos[m_iShootCount%3], fTimeDelta *m_fSpeed);
	return fabsf(m_pTransform->m_vInfo[INFO_POS].y - m_fMovepos[m_iShootCount%3])<1.f;
}

void CBoss1Head::ShootHead(const _float & fTimeDelta)
{
	if (m_iShootCount > 5)
	{
		for (int i = 0; i < m_PartsVec.size(); i++)
			m_PartsVec[i]->m_pGameObject->Set_Dead();
		m_bDead = true;
	}
		

	if (isMoveDone(fTimeDelta))
	{
		//이동 방향과 생성 위치를 알려주는 CREATE
		CLayer* pStageLayer = dynamic_cast<CLayer*>(Engine::Get_Layer(L"Layer_GameLogic"));
		if (pStageLayer != nullptr)
			FACTORY<CBoss1HeadSub>::Create(L"Boss1Hand", pStageLayer, m_pTransform->m_vInfo[INFO_POS], _vec3(1,0,0));
		m_iShootCount++;
	}
}

HRESULT CBoss1Head::Add_Component(void)
{
	CComponent*		pComponent = nullptr;
	pComponent = m_pCollider = dynamic_cast<CCollider*>(Engine::Clone_Proto(L"Collider", this));
	NULL_CHECK_RETURN(m_pCollider, E_FAIL);
	m_vecComponent[ID_DYNAMIC].push_back({ L"Collider", pComponent });
	return S_OK;
}

CBoss1Head * CBoss1Head::Create(LPDIRECT3DDEVICE9 pGraphicDev, _vec3& vPos, _vec3 & vtoward)
{
	CBoss1Head*		pInstance = new CBoss1Head(pGraphicDev);

	if (FAILED(pInstance->Ready_GameObject(vPos, vtoward)))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CBoss1Head::Free(void)
{
	__super::Free();
}
