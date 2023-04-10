#include "stdafx.h"
#include "Switch.h"

#include "Export_Function.h"

bool CSwitch::m_bSwtichON = false;

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
	m_pTransform->m_bIsStatic = false;

	// ���� ť�꿡 z�ప ���� ���� �տ� �־�� �ϱ� ������ �ٿ�� �ڽ� ���� ����
	m_pCollider->Set_BoundingBox({ 1.5f, 2.5f, 2.1f });

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
	m_pSwitchCube = Engine::Get_GameObject(L"Layer_GameLogic", L"SwitchCube");
	NULL_CHECK_RETURN(m_pSwitchCube, );

	// �Ͻ������� const ����ȭ�ؼ� �÷��̾��� �浹 ������ �о��. �̰� �´����� �𸣰���
	COL_DIR	tToodeeDir = const_cast<Collision*>(collision)->Get_ColDir();

	if (!lstrcmp(L"Toodee", collision->otherObj->m_pTag) || !lstrcmp(L"Pig", collision->otherObj->m_pTag))
	{
		// �������, ������ ���� �ϴ°� ������ ������ �ڵ� ������ �ڵ� �־���, 
		// ������ ����ġ ��ü�� ť��� ������ Pos�� �ֱ� ������ ���������
		if (DIR_UP == tToodeeDir || DIR_BACK == tToodeeDir)
		{
			m_bSwtichON = true;
			m_iTextureIndex = 1;
		}			
	}
}

void CSwitch::OnCollisionExit(const Collision * collision)
{
	m_bSwtichON = false;
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
