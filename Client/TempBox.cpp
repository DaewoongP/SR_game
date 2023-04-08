#include "stdafx.h"
#include "TempBox.h"
#include "MoveBox.h"
#include "Export_Function.h"

CTempBox::CTempBox(LPDIRECT3DDEVICE9 pGraphicDev) :CCube(pGraphicDev)
{
}

CTempBox::~CTempBox()
{
}

HRESULT CTempBox::Ready_GameObject(_vec3 & vPos)
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);
	m_pTransform->m_vInfo[INFO_POS] = vPos;
	m_pTransform->m_vScale = { 1.f,1.f,1.f };
	m_pCollider->m_bIsTrigger = true;

	return S_OK;
}

_int CTempBox::Update_GameObject(const _float & fTimeDelta)
{
	if (m_bDead)
		return OBJ_DEAD;
	__super::Update_GameObject(fTimeDelta);
	return S_OK;
}

void CTempBox::LateUpdate_GameObject(void)
{
	__super::LateUpdate_GameObject();
}

void CTempBox::Render_GameObject(void)
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransform->Get_WorldMatrixPointer());

	m_pTextureCom->Set_Texture();

	m_pBufferCom->Render_Buffer();
	CGameObject::Render_GameObject();
}

_int CTempBox::Update_Too(const _float & fTimeDelta)
{
	return 0;
}

void CTempBox::OnCollisionEnter(const Collision * collision)
{
}

void CTempBox::OnCollisionStay(const Collision * collision)
{
	if (!lstrcmp(collision->otherObj->m_pTag, L"MoveCube"))
	{
		//내 위치랑 같다면.
		if (collision->otherObj->m_pTransform->m_vInfo[INFO_POS] == m_pTransform->m_vInfo[INFO_POS])
		{
			//그 친구는 돌멩이가 되고
			dynamic_cast<CMoveBox*>(collision->otherObj)->m_bIsStone = true;
			for (int i = 0; i < DIR_END;i++)
				dynamic_cast<CMoveBox*>(collision->otherObj)->m_bIsCol[i] = true;

			//낙하를 트루로 바까준다.
			dynamic_cast<CMoveBox*>(collision->otherObj)->DoFallingStart();
			//나는 죽는다.
			m_bDead = true;
		}
	}
}

void CTempBox::OnCollisionExit(const Collision * collision)
{
}

HRESULT CTempBox::Add_Component(void)
{
	CComponent* pComponent = nullptr;

	pComponent = m_pBufferCom = dynamic_cast<CCubeTex*>(Engine::Clone_Proto(L"CubeTex", this));
	NULL_CHECK_RETURN(m_pBufferCom, E_FAIL);
	m_uMapComponent[ID_STATIC].insert({ L"CubeTex", pComponent });

	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"KeyBox_Cube", this));
	NULL_CHECK_RETURN(m_pTextureCom, E_FAIL);
	m_uMapComponent[ID_STATIC].insert({ L"KeyBox_Cube", pComponent });

	pComponent = m_pCollider = dynamic_cast<CCollider*>(Engine::Clone_Proto(L"Collider", this));
	NULL_CHECK_RETURN(m_pCollider, E_FAIL);
	m_uMapComponent[ID_DYNAMIC].insert({ L"Collider", pComponent });
	m_pCollider->Set_BoundingBox({ 2.f, 2.f,2.f });
	m_pCollider->Set_Group(COL_ENV);

	return S_OK;
}

CTempBox * CTempBox::Create(LPDIRECT3DDEVICE9 pGraphicDev, _vec3 & vPos)
{
	CTempBox* pInstance = new CTempBox(pGraphicDev);
	if (FAILED(pInstance->Ready_GameObject(vPos)))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CTempBox::Free()
{
	__super::Free();
}
