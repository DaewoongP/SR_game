#include "stdafx.h"
#include "Portal.h"
#include "AbstractFactory.h"
#include "Export_Function.h"
#include "SwallowPortal.h"
#include "Toodee.h"
#include "Topdee.h"

CPortal::CPortal(LPDIRECT3DDEVICE9 pGraphicDev)
	:CGameObject(pGraphicDev),
	m_bTooCol(false), m_bTopCol(false), m_bCreateSwallowPortal(true)
{
}

CPortal::~CPortal()
{
}

HRESULT CPortal::Ready_GameObject(_vec3& vPos)
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);
	m_dwPortalTimer = 1;
	m_pTransform->m_vScale = { 2.f, 2.f, 1.f };
	m_pTransform->m_vInfo[INFO_POS] = vPos;
	m_pTransform->m_bIsStatic = false;
	m_pCollider->Set_Group(COL_ENV);
	m_pCollider->m_bIsTrigger = true;

	return S_OK;
}

_int CPortal::Update_GameObject(const _float & fTimeDelta)
{
	if (m_bDead)
		return OBJ_DEAD;

	__super::Update_GameObject(fTimeDelta);

	Engine::Add_RenderGroup(RENDER_ALPHA, this);
	m_pTextureCom->Update_Anim(fTimeDelta);

	if (!m_bCreateSwallowPortal)
		m_dwPortalTimer -= fTimeDelta;

	if (m_dwPortalTimer <= 0)
	{
		CLayer* pStageLayer = dynamic_cast<CLayer*>(Engine::Get_Layer(L"Layer_GameLogic"));
		if (pStageLayer == nullptr) return 0;

		StopSound(SOUND_EFFECT_GIMMICK);
		PlaySound_Effect(L"75.wav", SOUND_EFFECT_GIMMICK, 1.f);
		FACTORY<CSwallowPortal>::Create(L"SwallowPortal", pStageLayer, m_pTransform->m_vInfo[INFO_POS]+_vec3(0,0,-2.1f));
		dynamic_cast<CToodee*>(m_pPlayer1)->SetRenderONOFF(false);
		dynamic_cast<CTopdee*>(m_pPlayer2)->SetRenderONOFF(false);
		m_dwPortalTimer = 1000;
	}

	//debug
	if (Engine::Get_DIKeyState(DIK_F7) == Engine::KEYDOWN)
	{
		CLayer* pStageLayer = dynamic_cast<CLayer*>(Engine::Get_Layer(L"Layer_GameLogic"));
		FACTORY<CSwallowPortal>::Create(L"SwallowPortal", pStageLayer, m_pTransform->m_vInfo[INFO_POS] + _vec3(0, 0, -2.1f));
	}
		
	return 0;
}

void CPortal::LateUpdate_GameObject(void)
{
	__super::LateUpdate_GameObject();
}

void CPortal::Render_GameObject(void)
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransform->Get_WorldMatrixPointer());

	m_pTextureCom->Set_Texture(0);
	m_pShadow->Render_Shadow(m_pBufferCom);

	m_pBufferCom->Render_Buffer();

	__super::Render_GameObject();
}

void CPortal::OnCollisionEnter(const Collision * collision)
{
	m_pPlayer1 = Engine::Get_GameObject(L"Layer_GameLogic", L"Toodee");
	FAILED_CHECK_RETURN(m_pPlayer1, );
	m_pPlayer2 = Engine::Get_GameObject(L"Layer_GameLogic", L"Topdee");
	FAILED_CHECK_RETURN(m_pPlayer2, );

	if (m_pPlayer1 == collision->otherObj)
		m_bTooCol = true;

	if (m_pPlayer2 == collision->otherObj)
		m_bTopCol = true;

	if (m_bTooCol && m_bTopCol && m_bCreateSwallowPortal)
	{
		dynamic_cast<CToodee*>(m_pPlayer1)->SetDoStop(m_pTransform->m_vInfo[INFO_POS] + _vec3(-4, 0, -2.1f));
		dynamic_cast<CTopdee*>(m_pPlayer2)->SetDoStop(m_pTransform->m_vInfo[INFO_POS] + _vec3(4, 0, -2.1f));
		m_bCreateSwallowPortal = false;
	}
}

void CPortal::OnCollisionExit(const Collision * collision)
{
	if (m_pPlayer1 == collision->otherObj)
		m_bTooCol = false;

	if (m_pPlayer2 == collision->otherObj)
		m_bTopCol = false;
}

HRESULT CPortal::Add_Component(void)
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

CPortal * CPortal::Create(LPDIRECT3DDEVICE9 pGraphicDev, _vec3& vPos)
{
	CPortal* pInstance = new CPortal(pGraphicDev);

	if (FAILED(pInstance->Ready_GameObject(vPos)))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CPortal::Free(void)
{
	__super::Free();
}
