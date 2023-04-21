#include "stdafx.h"
#include "Boss1Hand.h"
#include "AbstractFactory.h"
#include "Boss1Parts.h"

CBoss1Hand::CBoss1Hand(LPDIRECT3DDEVICE9 pGraphicDev):CGameObject(pGraphicDev)
{
	m_bInit = true;
}

CBoss1Hand::~CBoss1Hand()
{
}

HRESULT CBoss1Hand::Ready_GameObject(_vec3 & vPos,_vec3 vToWard)
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);
	m_pTransform->m_vInfo[INFO_POS] = vPos;
	m_pTransform->m_vScale = _vec3(5,5,5);
	m_pTransform->m_vAngle = _vec3(D3DXToRadian(-90), D3DXToRadian(90), 0);
	m_vToWard = vToWard;
	return S_OK;
}

_int CBoss1Hand::Update_GameObject(const _float & fTimeDelta)
{
	if (m_bInit)
	{
		//실행해주는 코드
		CLayer* pStageLayer = dynamic_cast<CLayer*>(Engine::Get_Layer(L"Layer_GameLogic"));
		NULL_CHECK_RETURN(pStageLayer, E_FAIL);
		for (int i = 0; i < 8; i++)
			FAILED_CHECK_RETURN(FACTORY<CBoss1Parts>::Create(L"Boss1Parts", pStageLayer, _vec3(0, 0, i*0.4f), m_pTransform, L"Boss1_Pattern02", i, false), E_FAIL);
		m_bInit = false;
	}
	
	_vec3 out; //목표점
	_vec3 cur= m_pTransform->m_vInfo[INFO_POS]; //현재 위치
	_float len = D3DXVec3Length(&(cur - m_vToWard)) / 2; //반지름
	GetVectorSlerp(&out, &cur,&m_vToWard,&_vec3(0, 1, 0), len,fTimeDelta);
	
	_vec3 dir, upvec;
	D3DXVec3Normalize(&dir,&(out - cur));
	D3DXVec3Normalize(&upvec, &m_pTransform->m_vInfo[INFO_LOOK]);
	float dot = D3DXVec3Dot(&upvec, &dir);
	float radian = acosf(dot);

	if (out.z > cur.z)
		radian = 2 * D3DX_PI - radian;

	m_pTransform->m_vAngle.z = radian;
	m_pTransform->m_vInfo[INFO_POS] = out;
	Engine::Add_RenderGroup(RENDER_NONALPHA, this);
	__super::Update_GameObject(fTimeDelta);
	return 0;
}

void CBoss1Hand::LateUpdate_GameObject(void)
{
	__super::LateUpdate_GameObject();
}

void CBoss1Hand::Render_GameObject(void)
{
	__super::Render_GameObject();
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransform->Get_WorldMatrixPointer());
}

HRESULT CBoss1Hand::Add_Component(void)
{
	CComponent*		pComponent = nullptr;
	pComponent = m_pCollider = dynamic_cast<CCollider*>(Engine::Clone_Proto(L"Collider", this));
	NULL_CHECK_RETURN(m_pCollider, E_FAIL);
	m_vecComponent[ID_DYNAMIC].push_back({ L"Collider", pComponent });
	return S_OK;
}

CBoss1Hand * CBoss1Hand::Create(LPDIRECT3DDEVICE9 pGraphicDev, _vec3 & vPos, _vec3 & vtoward)
{
	CBoss1Hand*		pInstance = new CBoss1Hand(pGraphicDev);

	if (FAILED(pInstance->Ready_GameObject(vPos, vtoward)))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CBoss1Hand::Free(void)
{
	__super::Free();
}
