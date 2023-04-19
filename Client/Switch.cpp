#include "stdafx.h"
#include "Switch.h"

#include "Export_Function.h"

bool CSwitch::m_bSwtichON = false;
_int CSwitch::m_iCnt = 0;
CSwitch::CSwitch(LPDIRECT3DDEVICE9 pGraphicDev)
	:CGameObject(pGraphicDev),
	m_pSwitchCube(nullptr),
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
	m_pTransform->m_vInfo[INFO_POS].z = 8.999f;
	m_pTransform->m_vScale = { 0.9f, 0.9f, 1.f };
	m_pTransform->m_bIsStatic = false;

	m_pCollider->Set_BoundingBox({ 1.f, 1.5f, 1.1f }, {0.f, 0.5f, 0.f});

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
	m_pShadow->Render_Shadow(m_pBufferCom);

	m_pBufferCom->Render_Buffer();

	__super::Render_GameObject();
}

void CSwitch::OnCollisionEnter(const Collision * collision)
{
	COL_DIR	tToodeeDir = const_cast<Collision*>(collision)->Get_ColDir();

	if (!lstrcmp(L"Toodee", collision->otherObj->m_pTag) || 
		!lstrcmp(L"Pig", collision->otherObj->m_pTag) ||
		!lstrcmp(L"Boss2", collision->otherObj->m_pTag) ||
		!lstrcmp(L"Boss2Hand", collision->otherObj->m_pTag)||
		!lstrcmp(L"Tookee", collision->otherObj->m_pTag))
	{
		if (DIR_UP == tToodeeDir || DIR_BACK == tToodeeDir || DIR_LEFT == tToodeeDir || DIR_RIGHT == tToodeeDir)
		{
			m_bSwtichON = true;
			m_iTextureIndex = 1;
			CSwitch::m_iCnt++;
		}			
	}
}

void CSwitch::OnCollisionExit(const Collision * collision)
{
	CSwitch::m_iCnt--;
	m_iTextureIndex = 0;
	if (m_iCnt == 0)
		m_bSwtichON = false;
		
}

HRESULT CSwitch::Add_Component(void)
{
	CComponent* pComponent = nullptr;

	pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(Engine::Clone_Proto(L"RcTex", this));
	NULL_CHECK_RETURN(m_pBufferCom, E_FAIL);
	m_vecComponent[ID_STATIC].push_back({ L"RcTex", pComponent });

	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Switch_Texture", this));
	NULL_CHECK_RETURN(m_pTextureCom, E_FAIL);
	m_vecComponent[ID_STATIC].push_back({ L"Switch_Texture", pComponent });

	pComponent = m_pCollider = dynamic_cast<CCollider*>(Engine::Clone_Proto(L"Collider", this));
	NULL_CHECK_RETURN(m_pCollider, E_FAIL);
	m_vecComponent[ID_DYNAMIC].push_back({ L"Collider", pComponent });

	pComponent = m_pShadow = dynamic_cast<CShadow*>(Engine::Clone_Proto(L"Shadow", this));
	NULL_CHECK_RETURN(m_pShadow, E_FAIL);
	m_vecComponent[ID_STATIC].push_back({ L"Shadow", pComponent });


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
