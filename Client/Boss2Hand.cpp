#include "stdafx.h"
#include "Boss2Hand.h"
#include "Export_Function.h"
#include "StageCamera.h"
#include "Cube.h"
#include "Toodee.h"
#include "Tookee.h"
#include "Topdee.h"
CBoss2Hand::CBoss2Hand(LPDIRECT3DDEVICE9 pGraphicDev)
	: CGameObject(pGraphicDev)
{
}


CBoss2Hand::~CBoss2Hand()
{
}

HRESULT CBoss2Hand::Ready_GameObject(_vec3 & vPos)
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	//지정된 위쪽에서 태어나겠지
	m_pTransform->m_vScale = _vec3(9, 20, 5);
	m_pTransform->m_vInfo[INFO_POS] = vPos;
	m_pTransform->m_bIsStatic = false;

	m_fStartY = vPos.y;

	m_pCollider->Set_BoundingBox(_vec3(9,5,3));
	m_pCollider->Set_BoundOffset(_vec3(0.0f, -(m_pTransform->m_vScale.y - m_pCollider->Get_BoundSize().y*0.5f), -1.0f));
	m_pCollider->m_bIsTrigger = true;

	m_pRigid->m_bUseGrivaty = false;
	//충돌 전부 OFF
	//m_pCollider->m_bIsTrigger = true;

	m_state = B2H_APPEAR;
	return S_OK;
}

_int CBoss2Hand::Update_GameObject(const _float & fTimeDelta)
{
	if (m_bDead)
		return 1;

	DoActionSwitch(fTimeDelta);

	__super::Update_GameObject(fTimeDelta);
	Engine::Add_RenderGroup(RENDER_ALPHA, this);

	//DoActionSwitch(fTimeDelta);
	return 0;
}

_int CBoss2Hand::Update_Too(const _float & fTimeDelta)
{
	return _int();
}

_int CBoss2Hand::Update_Top(const _float & fTimeDelta)
{
	return _int();
}

void CBoss2Hand::LateUpdate_GameObject(void)
{
	__super::LateUpdate_GameObject();
}

void CBoss2Hand::Render_GameObject(void)
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransform->Get_WorldMatrixPointer());
	m_pTextureCom->Set_Texture(0);
	m_pShadow->Render_Shadow(m_pBufferCom);

	m_pBufferCom->Render_Buffer();
	__super::Render_GameObject();

	m_pFistparticle->Update_Particle();
}

void CBoss2Hand::OnCollisionEnter(const Collision * collision)
{
	TOOKEEDIE;
	TOODEEDIE;
	TOPDEEDIE;
}

void CBoss2Hand::OnCollisionStay(const Collision * collision)
{
}

void CBoss2Hand::OnCollisionExit(const Collision * collision)
{
}

void CBoss2Hand::SwapTrigger()
{
}

HRESULT CBoss2Hand::Add_Component(void)
{
	CComponent*		pComponent = nullptr;

	pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(Engine::Clone_Proto(L"RcTex", this));
	NULL_CHECK_RETURN(m_pBufferCom, E_FAIL);
	m_vecComponent[ID_STATIC].push_back({ L"RcTex", pComponent });

	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Boss2_Hand", this));
	NULL_CHECK_RETURN(m_pTextureCom, E_FAIL);
	m_vecComponent[ID_STATIC].push_back({ L"Texture", pComponent });

	pComponent = m_pRigid = dynamic_cast<CRigidbody*>(Engine::Clone_Proto(L"Rigidbody", this));
	NULL_CHECK_RETURN(m_pRigid, E_FAIL);
	m_vecComponent[ID_DYNAMIC].push_back({ L"Rigidbody", pComponent });

	pComponent = m_pCollider = dynamic_cast<CCollider*>(Engine::Clone_Proto(L"Collider", this));
	NULL_CHECK_RETURN(m_pCollider, E_FAIL);
	m_vecComponent[ID_DYNAMIC].push_back({ L"Collider", pComponent });

	pComponent = m_pFistparticle = dynamic_cast<CCircularParticle*>(Engine::Clone_Proto(L"Boss2LandParticle", this));
	NULL_CHECK_RETURN(m_pFistparticle, E_FAIL);
	m_vecComponent[ID_STATIC].push_back({ L"Boss2LandParticle", pComponent });

	pComponent = m_pShadow = dynamic_cast<CShadow*>(Engine::Clone_Proto(L"Shadow", this));
	NULL_CHECK_RETURN(m_pShadow, E_FAIL);
	m_vecComponent[ID_STATIC].push_back({ L"Shadow", pComponent });

	return S_OK;
}

void CBoss2Hand::DoActionSwitch(const _float & fTimeDelta)
{
	switch (m_state)
	{
	case B2H_APPEAR:
		D0_Appear(fTimeDelta);
		break;
	case B2H_READY_LITTLEUP:
		D0_Ready_LittleUp(fTimeDelta);
		break;
	case B2H_LITTLEUP:
		D0_LittleUp(fTimeDelta);
		break;
	case B2H_DROP:
		D0_Drop(fTimeDelta);
		break;
	case B2H_READY_DISAPPEAR:
		D0_Ready_DisAppear(fTimeDelta);
		break;
	case B2H_DISAPPEAR:
		D0_DisAppear(fTimeDelta);
		break;
	case B2H_DIE:
		break;
	}
}

void CBoss2Hand::D0_Appear(const _float & fTimeDelta)
{
	if (m_dwActionTimer < 1.0f)
	{
		m_dwActionTimer += fTimeDelta;

		if (1.0f <= m_dwActionTimer)
		{
			m_dwActionTimer = 1.0f;
		}

		m_pTransform->m_vInfo[INFO_POS].y = Lerp(m_fStartY, B2H_APPEAR_YPOS, m_dwActionTimer);
	}
	else
	{
		Next_State();
		Set_BoundOffSetZ(1.0f);
	}
}

void CBoss2Hand::D0_Ready_LittleUp(const _float & fTimeDelta)
{
	if (m_dwActionTimer < 1.0f)
	{
		m_dwActionTimer += fTimeDelta;
	}
	else
	{
		Next_State();
	}
}

void CBoss2Hand::D0_LittleUp(const _float & fTimeDelta)
{
	if (m_dwActionTimer < 1.0f)
	{
		m_dwActionTimer += fTimeDelta * 10.0f;

		if (1.0f <= m_dwActionTimer)
		{
			m_dwActionTimer = 1.0f;
		}

		m_pTransform->m_vInfo[INFO_POS].y = Lerp(B2H_APPEAR_YPOS, B2H_APPEAR_YPOS + 5, m_dwActionTimer);

	}
	else
	{
		Next_State();
	}
}

void CBoss2Hand::D0_Drop(const _float & fTimeDelta)
{
	if (m_dwActionTimer < 1.0f)
	{
		m_dwActionTimer += fTimeDelta * 4.0f;

		if (1.0f <= m_dwActionTimer)
		{
			m_dwActionTimer = 1.0f;
		}

		m_pTransform->m_vInfo[INFO_POS].y = Lerp(B2H_APPEAR_YPOS + 5, B2H_DROP_YPOS, m_dwActionTimer);

	}
	else
	{

		BoundingBox box;
		_vec3 vInfo = m_pTransform->m_vInfo[INFO_POS];
		vInfo.y -= B2H_DROP_YPOS - 14;
		vInfo.z += 0.2f;
		box.Offset(vInfo);
		m_pFistparticle->Set_Size(8.f);
		m_pFistparticle->Set_Options(2.5f, 20.f);
		m_pFistparticle->Set_BoundingBox(box);
		m_pFistparticle->Start_Particle();

		Next_State();
		StopSound(SOUND_EFFECT_ENEMY);
		PlaySound_Effect(L"77.wav", SOUND_EFFECT_ENEMY, 1.f);
		
		dynamic_cast<CStage1Camera*>(Engine::Get_GameObject(L"Layer_Environment", L"Camera"))->Start_Camera_Shake(0.8f, 100.0f, SHAKE_Y);
	}
}

void CBoss2Hand::D0_Ready_DisAppear(const _float & fTimeDelta)
{
	if (m_dwActionTimer < 1.0f)
	{
		m_dwActionTimer += fTimeDelta;
	}
	else
	{
		Next_State();
		Set_BoundOffSetZ(-1.0f);
	}
}

void CBoss2Hand::D0_DisAppear(const _float & fTimeDelta)
{
	if (m_dwActionTimer < 1.0f)
	{
		m_dwActionTimer += fTimeDelta;

		if (1.0f <= m_dwActionTimer)
		{
			m_dwActionTimer = 1.0f;
		}

		m_pTransform->m_vInfo[INFO_POS].y = Lerp(B2H_DROP_YPOS, m_fStartY, m_dwActionTimer);
	}
	else
	{
		Next_State();
		m_bDead = true;
	}
}

CBoss2Hand * CBoss2Hand::Create(LPDIRECT3DDEVICE9 pGraphicDev, _vec3 & vPos)
{
	CBoss2Hand*		pInstance = new CBoss2Hand(pGraphicDev);

	if (FAILED(pInstance->Ready_GameObject(vPos)))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CBoss2Hand::Free(void)
{
	__super::Free();
}
