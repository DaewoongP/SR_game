#include "stdafx.h"
#include "GiantHand.h"
#include "AbstractFactory.h"
#include "Boss1Parts.h"
#include "GravityCube.h"
#include "ThirdCamera.h"
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
	m_pCollider->Set_BoundingBox({1,35,35});
	m_pCollider->Set_BoundOffset(_vec3(0,-8,+0.5f));
	m_pTransform->m_vInfo[INFO_POS] = vPos;
	m_pTransform->m_vScale = _vec3(30, 30, 30);
	m_pTransform->m_vAngle = _vec3(D3DXToRadian(-90), D3DXToRadian(90), 0);
	m_vSummonPos = vPos;
	m_fweight = 0;
	m_eState = GH_IDLE;
	m_pTransform->m_bIsStatic = true;
	m_bCollision = false;
	return S_OK;
}

_int CGiantHand::Update_GameObject(const _float & fTimeDelta)
{
	if (m_bDead)
	{
		return -1;
	}

	if (m_bInit)
	{
		//실행해주는 코드
		CLayer* pStageLayer = dynamic_cast<CLayer*>(Engine::Get_Layer(L"Layer_GameLogic"));
		if (pStageLayer == nullptr)
			return 0;
		for (int i = 0; i < 7; i++)
		{
			FAILED_CHECK_RETURN(FACTORY<CBoss1Parts>::Create(L"Boss1Parts", pStageLayer, _vec3(0, 0, i*0.4f), m_pTransform, L"Boss1_Pattern03", i, false), E_FAIL);
			m_PartsVec.push_back(m_pTransform->GetChild(i));
		}
		m_bInit = false;
	}
	Engine::Add_RenderGroup(RENDER_NONALPHA, this);

	switch (m_eState)
	{
	case GH_IDLE:
		//아무것도 안할거임
		break;
	case GH_STUMP:
		Do_Stump(fTimeDelta);
		break;
	case GH_UP:
		Do_Up(fTimeDelta);
		break;
	case GH_END:
		m_fweight = 0;
		m_bCollision = false;
		m_bStop = false;
		break;
	}
	__super::Update_GameObject(fTimeDelta);
	return 0;
}

void CGiantHand::LateUpdate_GameObject(void)
{
	__super::LateUpdate_GameObject();
}

void CGiantHand::Render_GameObject(void)
{
	m_pBoxParticle->Update_Particle();
	__super::Render_GameObject();
}

void CGiantHand::OnCollisionEnter(const Collision * collision)
{
	if (!lstrcmp(collision->otherObj->m_pTag, L"GravityCube")
		&& !m_bStop)
	{
		_int pos_z = -8.5f;
		m_pTransform->m_vInfo[INFO_POS].z =
			Lerp(m_vSummonPos.z, pos_z, m_fweight);

		dynamic_cast<CGravityCube*>(collision->otherObj)->m_bIsStone = true;
		m_bStop = true;
	}
	dynamic_cast<CThirdCamera*>(Engine::Get_GameObject(L"Layer_Environment", L"Camera"))->Start_Camera_Shake(0.4f, 40.0f, SHAKE_ALL);

	CLayer* pStageLayer = dynamic_cast<CLayer*>(Engine::Get_Layer(L"Layer_GameLogic"));
	auto iter_begin = pStageLayer->GetMapObject()->begin();
	auto iter_end = pStageLayer->GetMapObject()->end();
	for (; iter_begin != iter_end; iter_begin++)
	{
		if (dynamic_cast<CCube*>(iter_begin->second) != nullptr&&dynamic_cast<CCube*>(iter_begin->second)->Get_Update())
		{
			float dist = D3DXVec3Length(&(m_pTransform->m_vInfo[INFO_POS] - iter_begin->second->m_pTransform->m_vInfo[INFO_POS]));
			//거리에 비례해 힘을 주겠습니다.
			_float duration = 32.f / dist;
			_float power = 5000.0f / dist;
			if (duration < 0.1f)duration = 0.1f;
			if (power < 1.f)power = 1.f;
			dynamic_cast<CCube*>(iter_begin->second)->Set_EarthQuake(duration, power, SHAKE_Z);
		}
	}
	//잠깐동안 카메라를 뒤로 뺀다.
	
	Set_Particle();
}

void CGiantHand::OnCollisionStay(const Collision * collision)
{
}

void CGiantHand::OnCollisionExit(const Collision * collision)
{
	if (!lstrcmp(collision->otherObj->m_pTag, L"GravityCube"))
	{
		m_bCollision = false;
	}
}

void CGiantHand::Do_Stump(const _float & fTimeDelta)
{
	if (m_bStop)
	{
		StopSound(SOUND_EFFECT_ENEMY);
		PlaySound_Effect(L"83.wav", SOUND_EFFECT_ENEMY, 1.f);
		return;
	}

	if (m_fweight < 1)
	{
		
		m_fweight += fTimeDelta * 3.f;
		_float camoff = dynamic_cast<CThirdCamera*>(Engine::Get_GameObject(L"Layer_Environment", L"Camera"))->GetOffset();
		dynamic_cast<CThirdCamera*>(Engine::Get_GameObject(L"Layer_Environment", L"Camera"))->SetOffset(camoff + 0.1f);
	}
		
	if (m_fweight >= 1)
	{
		
		m_fweight = 1;
	}
	//weight가 1 미만인 경우 collider에 검출되면
	_int pos_z = -7;
	
	m_pTransform->m_vInfo[INFO_POS].z =
		Lerp(m_vSummonPos.z, pos_z, m_fweight);
}

void CGiantHand::Do_Up(const _float & fTimeDelta)
{
	m_pTransform->m_vInfo[INFO_POS].z =
		Lerp(m_pTransform->m_vInfo[INFO_POS].z, -30, fTimeDelta*0.6f);
	_float camoff = dynamic_cast<CThirdCamera*>(Engine::Get_GameObject(L"Layer_Environment", L"Camera"))->GetOffset();
	dynamic_cast<CThirdCamera*>(Engine::Get_GameObject(L"Layer_Environment", L"Camera"))->SetOffset(Lerp(camoff,1, fTimeDelta*0.6f));
}

void CGiantHand::Set_Particle()
{
	_vec3 vPos = m_pTransform->m_vInfo[INFO_POS];
	BoundingBox box;
	box._min = _vec3(vPos.x - 4.f, 2.f, 10.f);
	box._max = _vec3(vPos.x + 4.f, 32.f, 10.f);
	m_pBoxParticle->Set_BoundingBox(box);
	m_pBoxParticle->Start_Particle();
}

HRESULT CGiantHand::Add_Component(void)
{
	CComponent*		pComponent = nullptr;
	pComponent = m_pCollider = dynamic_cast<CCollider*>(Engine::Clone_Proto(L"Collider", this));
	NULL_CHECK_RETURN(m_pCollider, E_FAIL);
	m_vecComponent[ID_DYNAMIC].push_back({ L"Collider", pComponent });

	pComponent = m_pBoxParticle = dynamic_cast<CBoxParticle*>(Engine::Clone_Proto(L"BoxParticle", this));
	NULL_CHECK_RETURN(m_pBoxParticle, E_FAIL);
	m_vecComponent[ID_DYNAMIC].push_back({ L"BoxParticle", pComponent });

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
