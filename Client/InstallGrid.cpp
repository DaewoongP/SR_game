#include "stdafx.h"
#include "InstallGrid.h"
#include "MoveCube.h"
#include "Export_Function.h"

CInstallGrid::CInstallGrid(LPDIRECT3DDEVICE9 pGraphicDev)
	:CGrid(pGraphicDev),
	m_iStageTileIndex(0)
{
}

CInstallGrid::~CInstallGrid()
{
}

HRESULT CInstallGrid::Ready_GameObject(_vec3 & vPos)
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);
	m_pTransform->m_vInfo[INFO_POS] = vPos;
	m_pTransform->m_vInfo[INFO_POS].z = 11.1f;
	m_pTransform->m_vScale = { 1.f,1.f,1.f };
	m_pCollider->m_bIsTrigger = true;
	return S_OK;
}

_int CInstallGrid::Update_GameObject(const _float & fTimeDelta)
{
	if (m_bDead)
		return OBJ_DEAD;

	CGameObject::Update_GameObject(fTimeDelta);

	Engine::Add_RenderGroup(RENDER_NONALPHA, this);

	return 0;
}

void CInstallGrid::LateUpdate_GameObject(void)
{
	CGameObject::LateUpdate_GameObject();
}

void CInstallGrid::Render_GameObject(void)
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransform->Get_WorldMatrixPointer());

	m_pTextureCom->Set_Texture(m_iStageTileIndex);

	m_pBufferCom->Render_Buffer();
}

void CInstallGrid::OnCollisionEnter(const Collision * collision)
{
}

void CInstallGrid::OnCollisionStay(const Collision * collision)
{
	if (!lstrcmp(collision->otherObj->m_pTag, L"MoveCube"))
	{
		//�� ��ġ�� ���ٸ�.
		if (collision->otherObj->m_pTransform->m_vInfo[INFO_POS].x == m_pTransform->m_vInfo[INFO_POS].x&&
			collision->otherObj->m_pTransform->m_vInfo[INFO_POS].y == m_pTransform->m_vInfo[INFO_POS].y)
		{
			//�� ģ���� �����̰� �ǰ�
			dynamic_cast<CMoveCube*>(collision->otherObj)->m_bIsStone = true;
			for (int i = 0; i < DIR_END; i++)
				dynamic_cast<CMoveCube*>(collision->otherObj)->m_bIsCol[i] = true;

			//���ϸ� Ʈ��� �ٱ��ش�.
			dynamic_cast<CMoveCube*>(collision->otherObj)->DoFallingStart();
			//���� �״´�.
			m_bDead = true;
		}
	}
}

void CInstallGrid::OnCollisionExit(const Collision * collision)
{
}

HRESULT CInstallGrid::Add_Component(void)
{
	CComponent*		pComponent = nullptr;

	pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(Engine::Clone_Proto(L"RcTex", this));
	NULL_CHECK_RETURN(m_pBufferCom, E_FAIL);
	m_uMapComponent[ID_STATIC].insert({ L"RcTex", pComponent });

	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Stage1_Tile_Texture", this));
	NULL_CHECK_RETURN(m_pTextureCom, E_FAIL);
	m_uMapComponent[ID_STATIC].insert({ L"Stage1_Tile_Texture", pComponent });

	pComponent = m_pCollider = dynamic_cast<CCollider*>(Engine::Clone_Proto(L"Collider", this));
	NULL_CHECK_RETURN(m_pCollider, E_FAIL);
	m_uMapComponent[ID_DYNAMIC].insert({ L"Collider", pComponent });
	m_pCollider->Set_BoundingBox({ 2.f, 2.f,2.f });
	m_pCollider->Set_Group(COL_ENV);

	return S_OK;
}

CInstallGrid * CInstallGrid::Create(LPDIRECT3DDEVICE9 pGraphicDev, _vec3 & vPos)
{
	CInstallGrid* pInstance = new CInstallGrid(pGraphicDev);

	if (FAILED(pInstance->Ready_GameObject(vPos)))
	{
		Safe_Release(pInstance);
		pInstance = nullptr;
	}

	return pInstance;
}

void CInstallGrid::Free(void)
{
	CGameObject::Free();
}