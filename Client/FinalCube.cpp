#include "stdafx.h"
#include "FinalCube.h"
#include "Export_Function.h"
#include "Item.h"
#include "AbstractFactory.h"

CFinalCube::CFinalCube(LPDIRECT3DDEVICE9 pGraphicDev)
	:CCube(pGraphicDev),
	m_iCubeIndex(0)
{
}

CFinalCube::~CFinalCube()
{
}

HRESULT CFinalCube::Ready_GameObject(_vec3 & vPos, _int iIndex)
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);
	m_pTransform->m_vInfo[INFO_POS] = vPos;
	m_pTransform->m_vScale = { 1.f,1.f,1.f };
	m_pTransform->m_bIsStatic = true;

	m_pCollider->Set_BoundingBox({ 2.f, 2.f,2.f });
	m_pCollider->Set_Group(COL_ENV);

	m_iCubeIndex = iIndex;

	return S_OK;
}

_int CFinalCube::Update_GameObject(const _float & fTimeDelta)
{
	if (m_bDead)
		return OBJ_DEAD;

	__super::Update_GameObject(fTimeDelta);

	return S_OK;
}

void CFinalCube::LateUpdate_GameObject(void)
{
	__super::LateUpdate_GameObject();
}

void CFinalCube::Render_GameObject(void)
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransform->Get_WorldMatrixPointer());

	m_pTextureCom->Set_Texture(m_iCubeIndex);

	m_pBufferCom->Render_Buffer();

	//__super::Render_GameObject();
}

void CFinalCube::OnCollisionEnter(const Collision * collision)
{
	if (!lstrcmp(collision->otherObj->m_pTag, L"Bullet") ||
		!lstrcmp(collision->otherObj->m_pTag, L"SwordBullet") ||
		!lstrcmp(collision->otherObj->m_pTag, L"FireBullet"))
	{
		//collision->otherObj->m_bDead = true;

		// 아이템을 확률적으로 생성 (25%)
		_int iRandValue = rand() % 4;

		if (0 == iRandValue)
		{
			int i = 0;
			// 4가지 아이템 중 하나를 무작위로 생성
			_int iRandItem = rand() % 4;

			CLayer* pLayer = Engine::Get_Layer(L"Layer_GameLogic");
			FAILED_CHECK_RETURN(FACTORY<CItem>::Create(L"Item", pLayer, m_pTransform->m_vInfo[INFO_POS], iRandItem), );
		}

		m_bDead = true;
	}
}

HRESULT CFinalCube::Add_Component(void)
{
	CComponent* pComponent = nullptr;

	pComponent = m_pBufferCom = dynamic_cast<CCubeTex*>(Engine::Clone_Proto(L"CubeTex", this));
	NULL_CHECK_RETURN(m_pBufferCom, E_FAIL);
	m_vecComponent[ID_STATIC].push_back({ L"CubeTex", pComponent });

	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Final_Cube", this));
	NULL_CHECK_RETURN(m_pTextureCom, E_FAIL);
	m_vecComponent[ID_STATIC].push_back({ L"Final_Cube", pComponent });

	pComponent = m_pCollider = dynamic_cast<CCollider*>(Engine::Clone_Proto(L"Collider", this));
	NULL_CHECK_RETURN(m_pCollider, E_FAIL);
	m_vecComponent[ID_DYNAMIC].push_back({ L"Collider", pComponent });

	return S_OK;
}

CFinalCube * CFinalCube::Create(LPDIRECT3DDEVICE9 pGraphicDev, _vec3 & vPos, _int iIndex)
{
	CFinalCube* pInstance = new CFinalCube(pGraphicDev);
	if (FAILED(pInstance->Ready_GameObject(vPos, iIndex)))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CFinalCube::Free()
{
	__super::Free();
}
