#include "stdafx.h"
#include "LaserTurret.h"
#include "Export_Function.h"
#include "AbstractFactory.h"

#include "Laser.h"

CLaserTurret::CLaserTurret(LPDIRECT3DDEVICE9 pGraphicDev)
	:CGameObject(pGraphicDev),
	m_iIndex(0),
	m_fCoolDown(0.f)
{
}

CLaserTurret::~CLaserTurret()
{
}

HRESULT CLaserTurret::Ready_GameObject(_vec3 & vPos, _int iIndex)
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);
	m_pTransform->m_vInfo[INFO_POS] = vPos;
	m_pTransform->m_vScale = { 1.f,1.f,1.f };
	m_pTransform->m_bIsStatic = false;

	m_pBulletPool->Ready_BulletPool();

	m_iIndex = iIndex;

	return S_OK;
}

_int CLaserTurret::Update_GameObject(const _float & fTimeDelta)
{
	if (m_bDead)
		return OBJ_DEAD;

	Engine::Add_RenderGroup(RENDER_ALPHA, this);

	__super::Update_GameObject(fTimeDelta);

	Shoot_Laser(fTimeDelta);

	return 0;
}

void CLaserTurret::LateUpdate_GameObject(void)
{
	m_pBulletPool->UnUse_Bullet();

	__super::LateUpdate_GameObject();
}

void CLaserTurret::Render_GameObject(void)
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransform->Get_WorldMatrixPointer());

	m_pTextureCom->Set_Texture(m_iIndex);
	m_pShadow->Render_Shadow(m_pBufferCom);

	m_pBufferCom->Render_Buffer();

	__super::Render_GameObject();
}

HRESULT CLaserTurret::Add_Component(void)
{
	CComponent* pComponent = nullptr;

	pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(Engine::Clone_Proto(L"RcTex", this));
	NULL_CHECK_RETURN(m_pBufferCom, E_FAIL);
	m_vecComponent[ID_STATIC].push_back({ L"RcTex" , pComponent });

	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Laser_Turret", this));
	NULL_CHECK_RETURN(m_pTextureCom, E_FAIL);
	m_vecComponent[ID_STATIC].push_back({ L"Laser_Turret",pComponent });

	pComponent = m_pShadow = dynamic_cast<CShadow*>(Engine::Clone_Proto(L"Shadow", this));
	NULL_CHECK_RETURN(m_pShadow, E_FAIL);
	m_vecComponent[ID_STATIC].push_back({ L"Shadow", pComponent });

	pComponent = m_pBulletPool = dynamic_cast<CBulletPool*>(Engine::Clone_Proto(L"BulletPool", this));
	NULL_CHECK_RETURN(m_pBulletPool, E_FAIL);
	m_vecComponent[ID_STATIC].push_back({ L"BulletPool", pComponent });

	return S_OK;
}

void CLaserTurret::Shoot_Laser(const _float & fTimeDelta)
{
	m_fCoolDown += fTimeDelta;

	if (0.2f < m_fCoolDown)
	{
		_vec3 vPos = m_pTransform->m_vInfo[INFO_POS];
		CGameObject* pGameObject;

		if(0 == m_iIndex)
			pGameObject = m_pBulletPool->Use_Bullet(_vec3{vPos.x + 1.f, vPos.y, vPos.z}, !m_iIndex);

		else if (1 == m_iIndex)
			pGameObject = m_pBulletPool->Use_Bullet(_vec3{ vPos.x - 1.f, vPos.y, vPos.z }, !m_iIndex);

		m_fCoolDown = 0.f;
	}
}

CLaserTurret * CLaserTurret::Create(LPDIRECT3DDEVICE9 pGraphicDev, _vec3 & vPos, _int iIndex)
{
	CLaserTurret* pInstance = new CLaserTurret(pGraphicDev);

	if (FAILED(pInstance->Ready_GameObject(vPos, iIndex)))
	{
		Safe_Release(pInstance);
		return nullptr;
	}
	return pInstance;
}

void CLaserTurret::Free(void)
{
	__super::Free();
}
