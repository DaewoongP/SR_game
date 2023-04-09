#include "stdafx.h"
#include "CrackCube.h"

#include"Export_Function.h"

CCrackCube::CCrackCube(LPDIRECT3DDEVICE9 pGraphicDev):CCube(pGraphicDev)
{
	
}

CCrackCube::~CCrackCube()
{
}

HRESULT CCrackCube::Ready_GameObject(_vec3& vPos)
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);
	m_pTransform->m_vScale = { 1.f,1.f,1.f };
	m_pTransform->m_vInfo[INFO_POS] = vPos;
	m_fBlockTime = CRACKTIME;
	m_pCollider->Set_BoundingBox({ 1.999f,1.999f,2.f });
	return S_OK;
}

_int CCrackCube::Update_GameObject(const _float& fTimeDelta)
{
	if (m_bDead)
		return OBJ_DEAD;
	__super::Update_GameObject(fTimeDelta);
	Engine::Add_RenderGroup(RENDER_NONALPHA, this);
	return OBJ_NOEVENT;
}

_int CCrackCube::Update_Too(const _float& fTimeDelta)
{
	// 레이충돌필요함 철민출동 십자로 쏴야댐 
	// 태그 안에서 레이충돌 시전 -> 최적화
	CGameObject* pCrackBox = Get_GameObject(L"Layer_GameLogic", L"CrackCube");

	if (nullptr != pCrackBox && dynamic_cast<CCrackCube*>(pCrackBox)->m_bCrackDead)
	{
		m_fBlockTime -= fTimeDelta;
		_vec3 ShakePos;
		ShakePos = m_pTransform->m_vInfo[INFO_POS];
		Shaking(ShakePos, fTimeDelta);
		if (m_fBlockTime <= 0.f)
		{
			m_bDead = true;
			return OBJ_DEAD;
		}
	}
	return 0;
}

_int CCrackCube::Update_Top(const _float& fTimeDelta)
{
	if (m_fBlockTime <= 0.f)
		return OBJ_DEAD;

	return 0;
}

void CCrackCube::LateUpdate_GameObject(void)
{
	__super::LateUpdate_GameObject();
}

void CCrackCube::Render_GameObject(void)
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransform->Get_WorldMatrixPointer());

	m_pTextureCom->Set_Texture();

	m_pBufferCom->Render_Buffer();
	CGameObject::Render_GameObject();
}

void CCrackCube::OnCollisionEnter(const Collision* collision)
{
	if (!lstrcmp(collision->otherObj->m_pTag, L"Toodee"))
		m_bCrackDead = true;
	if (!lstrcmp(collision->otherObj->m_pTag, L"CrackCube"))
		m_bCrackDead = true;
	__super::OnCollisionEnter(collision);

}

HRESULT CCrackCube::Add_Component(void)
{
	CComponent* pComponent = nullptr;

	pComponent = m_pBufferCom = dynamic_cast<CCubeTex*>(Engine::Clone_Proto(L"CubeTex", this));
	NULL_CHECK_RETURN(m_pBufferCom, E_FAIL);
	m_uMapComponent[ID_STATIC].insert({ L"CubeTex", pComponent });

	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"CrackBlock_Cube", this));
	NULL_CHECK_RETURN(m_pTextureCom, E_FAIL);
	m_uMapComponent[ID_STATIC].insert({ L"CrackBlock_Cube", pComponent });

	pComponent = m_pCollider = dynamic_cast<CCollider*>(Engine::Clone_Proto(L"Collider", this));
	NULL_CHECK_RETURN(m_pCollider, E_FAIL);
	m_uMapComponent[ID_DYNAMIC].insert({ L"Collider", pComponent });
	return S_OK;
}

void CCrackCube::Shaking(_vec3& vPos, const _float& fTimeDelta)
{
	static _float fTime = 0.f;
	fTime += fTimeDelta;

	if (fTime > 0.01f)
	{
		fTime = 0.f;

		_float fRandomX = (_float)(rand() % 100) / 100.f;
		
		_float fRandomY = (_float)(rand() % 100) / 100.f;
		//y값랜덤주게되면 점프가 잘 안됨
		_vec3 ShakePos = {vPos.x + 0.075f * (fRandomX - 0.5f),
					vPos.y /*+0.05f*(fRandomY-0.5f)*/ ,  10.f};
		m_pTransform->Set_Pos(ShakePos.x,ShakePos.y,10.f);
	}
}

CCrackCube* CCrackCube::Create(LPDIRECT3DDEVICE9 pGraphicDev, _vec3& vPos)
{
	CCrackCube* pInstance = new CCrackCube(pGraphicDev);
	if (FAILED(pInstance->Ready_GameObject(vPos)))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CCrackCube::Free(void)
{
	__super::Free();
}