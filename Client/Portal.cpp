#include "stdafx.h"
#include "Portal.h"

#include"Export_Function.h"
#include "SwallowPortal.h"
#include "Player.h"
#include "Player02.h"

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

	m_pTransform->m_vScale = { 2.f, 2.f, 1.f };
	m_pTransform->m_vInfo[INFO_POS] = vPos;
	m_pTransform->m_bIsStatic	 = false;

	return S_OK;
}

_int CPortal::Update_GameObject(const _float & fTimeDelta)
{
	if (m_bDead)
		return OBJ_DEAD;

	m_pPlayer2 = Engine::Get_GameObject(L"Layer_GameLogic", L"Player02");
	m_pPlayer1 = Engine::Get_GameObject(L"Layer_GameLogic", L"Player");

	__super::Update_GameObject(fTimeDelta);

	Engine::Add_RenderGroup(RENDER_ALPHA, this);
	m_pTextureCom->Update_Anim(fTimeDelta);

	return 0;
}

_int CPortal::Update_Too(const _float & fTimeDelta)
{
	return 0;
}

_int CPortal::Update_Top(const _float & fTimeDelta)
{
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

	m_pBufferCom->Render_Buffer();

	__super::Render_GameObject();
}

void CPortal::OnCollisionEnter(const Collision * collision)
{
	if (m_pPlayer1 == collision->otherObj)
		m_bTooCol = true;

	if (m_pPlayer2 == collision->otherObj)
	{
		this->m_pCollider->m_bIsTrigger = true;
		m_bTopCol = true;
	}		
}

void CPortal::OnCollisionStay(const Collision * collision)
{
	if (m_bTooCol && m_bTopCol && m_bCreateSwallowPortal)
	{
		CLayer* pStageLayer = dynamic_cast<CLayer*>(Engine::Get_Layer(L"Layer_GameLogic"));
		NULL_CHECK_RETURN(pStageLayer, );

		CGameObject* pGameObject = nullptr;

		pGameObject = CSwallowPortal::Create(m_pGraphicDev, m_pTransform->m_vInfo[INFO_POS]);
		NULL_CHECK_RETURN(pGameObject, );
		FAILED_CHECK_RETURN(pStageLayer->Add_GameObject(L"Swallow_Portal", pGameObject), );

		m_bCreateSwallowPortal = false;
	}
}

HRESULT CPortal::Add_Component(void)
{
	CComponent* pComponent = nullptr;

	pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(Engine::Clone_Proto(L"RcTex", this));
	NULL_CHECK_RETURN(m_pBufferCom, E_FAIL);
	m_uMapComponent[ID_STATIC].insert({ L"RcTex" , pComponent });

	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Portal_Texture", this));
	NULL_CHECK_RETURN(m_pTextureCom, E_FAIL);
	m_uMapComponent[ID_STATIC].insert({ L"Texture",pComponent });
	m_pTextureCom->Add_Anim(L"Idle_Portal", 0, 10, 1.f, true);
	m_pTextureCom->Switch_Anim(L"Idle_Portal");
	m_pTextureCom->m_bUseFrameAnimation = true;

	pComponent = m_pCollider = dynamic_cast<CCollider*>(Engine::Clone_Proto(L"Collider", this));
	NULL_CHECK_RETURN(m_pCollider, E_FAIL);
	m_uMapComponent[ID_DYNAMIC].insert({ L"Collider",pComponent });
	m_pCollider->Set_BoundingBox({ 5.f, 5.f, 2.f });

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
