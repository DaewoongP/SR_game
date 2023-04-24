#include "stdafx.h"
#include "Boss1Hand.h"
#include "AbstractFactory.h"
#include "Boss1Parts.h"
#include "..\Engine\CircularParticle.h"

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
	m_pTransform->m_vAngle = _vec3(D3DXToRadian(90), D3DXToRadian(90), 0);
	m_vToWard = vToWard;
	return S_OK;
}

_int CBoss1Hand::Update_GameObject(const _float & fTimeDelta)
{
	if (m_pCircularParticle->IsDead())
		return OBJ_DEAD;
	if (m_bInit)
	{
		//�������ִ� �ڵ�
		CLayer* pStageLayer = dynamic_cast<CLayer*>(Engine::Get_Layer(L"Layer_GameLogic"));
		NULL_CHECK_RETURN(pStageLayer, E_FAIL);
		for (int i = 0; i < 8; i++)
		{
			FAILED_CHECK_RETURN(FACTORY<CBoss1Parts>::Create(L"Boss1Parts", pStageLayer, _vec3(0, 0, i*0.4f), m_pTransform, L"Boss1_Pattern02", i, false), E_FAIL);
			m_PartsVec.push_back(m_pTransform->GetChild(i));
		}
		m_bInit = false;
	}
	
	_vec3 out; //��ǥ��
	_vec3 cur= m_pTransform->m_vInfo[INFO_POS]; //���� ��ġ
	_float len = D3DXVec3Length(&(cur - m_vToWard)) / 4; //������
	GetVectorSlerp(&out, &cur,&m_vToWard,&_vec3(0, 1, 0), len,fTimeDelta);
	
	_vec3 dir, lookvec;
	D3DXVec3Normalize(&dir,&(out - cur));
	D3DXVec3Normalize(&lookvec, &m_pTransform->m_vInfo[INFO_LOOK]);
	_float dot = D3DXVec3Dot(&lookvec, &dir);
	_float radian = acosf(dot);
	m_pTransform->Rotation(ROT_Z, D3DXToRadian(-radian));
	
	if (D3DXVec3Length(&(out - m_vToWard)) < 12.f && !m_pCircularParticle->IsRendering())
	{
		BoundingBox box;
		_vec3 vInfo = cur;
		vInfo.z = 10.f;
		box.Offset(vInfo);
		m_pCircularParticle->Set_Size(2.f);
		m_pCircularParticle->Set_Options(2.f, 25.f);
		m_pCircularParticle->Set_SizeLifeTime(1.f);
		m_pCircularParticle->Set_BoundingBox(box);
		m_pCircularParticle->Start_Particle();
	}
	
	m_pTransform->m_vInfo[INFO_POS] = out;
	if (m_pCircularParticle->IsRendering())
	{
		// �ȵ�θ޴ٷ�
		m_pTransform->m_vInfo[INFO_POS].z += 30.f;
	}
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
	if (m_pCircularParticle->IsRendering())
		m_pCircularParticle->Update_Particle();
	__super::Render_GameObject();
}

HRESULT CBoss1Hand::Add_Component(void)
{
	CComponent*		pComponent = nullptr;

	pComponent = m_pCollider = dynamic_cast<CCollider*>(Engine::Clone_Proto(L"Collider", this));
	NULL_CHECK_RETURN(m_pCollider, E_FAIL);
	m_vecComponent[ID_DYNAMIC].push_back({ L"Collider", pComponent });

	pComponent = m_pCircularParticle = dynamic_cast<CCircularParticle*>(Engine::Clone_Proto(L"Boss2LandParticle", this));
	NULL_CHECK_RETURN(m_pCircularParticle, E_FAIL);
	m_vecComponent[ID_DYNAMIC].push_back({ L"Boss2LandParticle", pComponent });

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
