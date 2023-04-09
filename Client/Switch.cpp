#include "stdafx.h"
#include "Switch.h"

#include "Export_Function.h"

CSwitch::CSwitch(LPDIRECT3DDEVICE9 pGraphicDev)
	:CGameObject(pGraphicDev),
	m_pToodee(nullptr),
	m_iTextureIndex(0)
{
}

CSwitch::~CSwitch()
{
}

HRESULT CSwitch::Ready_GameObject(_vec3 & vPos)
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);
	m_pTransform->m_vInfo[INFO_POS] = vPos;

	m_pCollider->Set_BoundingBox({ 2.f, 2.f,2.f });
	m_pCollider->Set_Group(COL_OBJ);
	m_pTransform->m_bIsStatic = true;	

	return S_OK;
}

_int CSwitch::Update_GameObject(const _float & fTimeDelta)
{
	if (m_bDead)
		return OBJ_DEAD;

	__super::Update_GameObject(fTimeDelta);

	Engine::Add_RenderGroup(RENDER_ALPHA, this);

	return S_OK;
}

void CSwitch::LateUpdate_GameObject(void)
{
	__super::LateUpdate_GameObject();
}

void CSwitch::Render_GameObject(void)
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransform->Get_WorldMatrixPointer());

	m_pTextureCom->Set_Texture(m_iTextureIndex);

	m_pBufferCom->Render_Buffer();

	__super::Render_GameObject();
}

void CSwitch::OnCollisionEnter(const Collision * collision)
{
	m_pToodee = Engine::Get_GameObject(L"Layer_GameLogic", L"Toodee");
	NULL_CHECK_RETURN(m_pToodee, );

	if (m_pToodee == collision->otherObj)
		m_iTextureIndex = 1;
}

void CSwitch::OnCollisionStay(const Collision * collision)
{
}

void CSwitch::OnCollisionExit(const Collision * collision)
{
	m_iTextureIndex = 0;
}

HRESULT CSwitch::Add_Component(void)
{
	CComponent* pComponent = nullptr;

	pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(Engine::Clone_Proto(L"RcTex", this));
	NULL_CHECK_RETURN(m_pBufferCom, E_FAIL);
	m_uMapComponent[ID_STATIC].insert({ L"RcTex", pComponent });

	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Switch_Texture", this));
	NULL_CHECK_RETURN(m_pTextureCom, E_FAIL);
	m_uMapComponent[ID_STATIC].insert({ L"Switch_Texture", pComponent });

	pComponent = m_pCollider = dynamic_cast<CCollider*>(Engine::Clone_Proto(L"Collider", this));
	NULL_CHECK_RETURN(m_pCollider, E_FAIL);
	m_uMapComponent[ID_DYNAMIC].insert({ L"Collider", pComponent });

	return S_OK;
}

CSwitch * CSwitch::Create(LPDIRECT3DDEVICE9 pGraphicDev, _vec3 & vPos)
{
	CSwitch* pInstance = new CSwitch(pGraphicDev);
	if (FAILED(pInstance->Ready_GameObject(vPos)))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CSwitch::Free()
{
	__super::Free();
}
