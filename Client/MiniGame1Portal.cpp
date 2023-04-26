#include "stdafx.h"
#include "MiniGame1Portal.h"
#include "AbstractFactory.h"
#include "Export_Function.h"
#include "SwallowPortal.h"
#include "Toodee.h"
#include "Topdee.h"

CMiniGame1Portal::CMiniGame1Portal(LPDIRECT3DDEVICE9 pGraphicDev)
	:CGameObject(pGraphicDev)
{
}

CMiniGame1Portal::~CMiniGame1Portal()
{
}

HRESULT CMiniGame1Portal::Ready_GameObject(_vec3& vPos)
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	m_pTransform->m_vScale = { 2.f, 2.f, 1.f };
	m_pTransform->m_vInfo[INFO_POS] = vPos;
	m_pTransform->m_bIsStatic = false;
	m_pCollider->Set_Group(COL_ENV);
	m_pCollider->m_bIsTrigger = true;

	return S_OK;
}

_int CMiniGame1Portal::Update_GameObject(const _float & fTimeDelta)
{
	if (m_bDead)
		return OBJ_DEAD;

	__super::Update_GameObject(fTimeDelta);

	Engine::Add_RenderGroup(RENDER_ALPHA, this);
	m_pTextureCom->Update_Anim(fTimeDelta);

	//debug
	if (Engine::Get_DIKeyState(DIK_F7) == Engine::KEYDOWN)
	{
		CLayer* pStageLayer = dynamic_cast<CLayer*>(Engine::Get_Layer(L"Layer_GameLogic"));
		FACTORY<CSwallowPortal>::Create(L"SwallowPortal", pStageLayer, m_pTransform->m_vInfo[INFO_POS]);
	}
		
	return 0;
}

void CMiniGame1Portal::LateUpdate_GameObject(void)
{
	__super::LateUpdate_GameObject();
}

void CMiniGame1Portal::Render_GameObject(void)
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransform->Get_WorldMatrixPointer());

	m_pTextureCom->Set_Texture(0);
	m_pShadow->Render_Shadow(m_pBufferCom);

	m_pBufferCom->Render_Buffer();

	__super::Render_GameObject();
}

void CMiniGame1Portal::OnCollisionEnter(const Collision * collision)
{
	CLayer* pStageLayer = dynamic_cast<CLayer*>(Engine::Get_Layer(L"Layer_GameLogic"));
	NULL_CHECK_RETURN(pStageLayer, );

	if (!lstrcmp(collision->otherObj->m_pTag, L"MiniGamePig"))
	{
		StopSound(SOUND_EFFECT_GIMMICK);
		PlaySound_Effect(L"75.wav", SOUND_EFFECT_GIMMICK, 1.f);
		//µ¹·ÁÁÜ.
	}
	else if (!lstrcmp(collision->otherObj->m_pTag, L"Toodee"))
	{
		//Åõµð Æ÷Å»¿¬Ãâ¸¸
		StopSound(SOUND_EFFECT_GIMMICK);
		PlaySound_Effect(L"75.wav", SOUND_EFFECT_GIMMICK, 1.f);
		//µ¹·ÁÁÜ.
		dynamic_cast<CToodee*>(collision->otherObj)->SetDoStop_Mini(m_pTransform);
	}
}

void CMiniGame1Portal::OnCollisionExit(const Collision * collision)
{
}

HRESULT CMiniGame1Portal::Add_Component(void)
{
	CComponent* pComponent = nullptr;

	pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(Engine::Clone_Proto(L"RcTex", this));
	NULL_CHECK_RETURN(m_pBufferCom, E_FAIL);
	m_vecComponent[ID_STATIC].push_back({ L"RcTex" , pComponent });

	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Portal_Texture", this));
	NULL_CHECK_RETURN(m_pTextureCom, E_FAIL);
	m_vecComponent[ID_STATIC].push_back({ L"Texture",pComponent });
	m_pTextureCom->Add_Anim(L"Idle_Portal", 0, 10, 1.f, true);
	m_pTextureCom->Switch_Anim(L"Idle_Portal");
	m_pTextureCom->m_bUseFrameAnimation = true;

	pComponent = m_pCollider = dynamic_cast<CCollider*>(Engine::Clone_Proto(L"Collider", this));
	NULL_CHECK_RETURN(m_pCollider, E_FAIL);
	m_vecComponent[ID_DYNAMIC].push_back({ L"Collider",pComponent });
	m_pCollider->Set_BoundingBox({ 5.f, 5.f, 2.f });

	pComponent = m_pShadow = dynamic_cast<CShadow*>(Engine::Clone_Proto(L"Shadow", this));
	NULL_CHECK_RETURN(m_pShadow, E_FAIL);
	m_vecComponent[ID_STATIC].push_back({ L"Shadow", pComponent });


	return S_OK;
}

CMiniGame1Portal * CMiniGame1Portal::Create(LPDIRECT3DDEVICE9 pGraphicDev, _vec3& vPos)
{
	CMiniGame1Portal* pInstance = new CMiniGame1Portal(pGraphicDev);

	if (FAILED(pInstance->Ready_GameObject(vPos)))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CMiniGame1Portal::Free(void)
{
	__super::Free();
}
