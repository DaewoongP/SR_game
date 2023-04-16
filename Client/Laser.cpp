#include "stdafx.h"
#include "Laser.h"
#include"Export_Function.h"

CLaser::CLaser(LPDIRECT3DDEVICE9 pGraphicDev)
	:CGameObject(pGraphicDev),
	m_bUse(false),
	m_fSpeed(15.f),
	m_eLaserDir(LASER_DIR_END)
{
}

CLaser::~CLaser()
{
}

HRESULT CLaser::Ready_GameObject(_vec3 & vPos, _int eDir)
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);
	m_pTransform->m_vInfo[INFO_POS] = vPos;
	m_pTransform->m_vScale = { 2.f,2.f,1.f };
	m_pTransform->m_bIsStatic = false;

	m_pCollider->Set_Options({ 3.f, 1.f, 0.7f }, COL_OBJ, true);

	m_eLaserDir = (LASERDIR)eDir;

	return S_OK;
}

_int CLaser::Update_GameObject(const _float & fTimeDelta)
{
	if (m_bDead)
		return OBJ_DEAD;

	if(LASER_RIGHT == m_eLaserDir)
		m_pTransform->m_vInfo[INFO_POS].x += m_fSpeed * fTimeDelta;

	else
		m_pTransform->m_vInfo[INFO_POS].x -= m_fSpeed * fTimeDelta;

	Engine::Add_RenderGroup(RENDER_ALPHA, this);

	__super::Update_GameObject(fTimeDelta);

	return 0;
}

void CLaser::LateUpdate_GameObject(void)
{
	__super::LateUpdate_GameObject();
}

void CLaser::Render_GameObject(void)
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransform->Get_WorldMatrixPointer());

	m_pTextureCom->Set_Texture();

	m_pBufferCom->Render_Buffer();

	__super::Render_GameObject();
}

void CLaser::OnCollisionEnter(const Collision * collision)
{
	//m_bDead = true;
	m_bUse = false;
}

HRESULT CLaser::Add_Component(void)
{
	CComponent* pComponent = nullptr;

	pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(Engine::Clone_Proto(L"RcTex", this));
	NULL_CHECK_RETURN(m_pBufferCom, E_FAIL);
	m_vecComponent[ID_STATIC].push_back({ L"RcTex" , pComponent });

	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Laser", this));
	NULL_CHECK_RETURN(m_pTextureCom, E_FAIL);
	m_vecComponent[ID_STATIC].push_back({ L"Laser",pComponent });

	pComponent = m_pCollider = dynamic_cast<CCollider*>(Engine::Clone_Proto(L"Collider", this));
	NULL_CHECK_RETURN(m_pCollider, E_FAIL);
	m_vecComponent[ID_DYNAMIC].push_back({ L"Collider",pComponent });

	return S_OK;
}

CLaser * CLaser::Create(LPDIRECT3DDEVICE9 pGraphicDev, _vec3 & vPos, _int eDir)
{
	CLaser* pInstance = new CLaser(pGraphicDev);

	if (FAILED(pInstance->Ready_GameObject(vPos, eDir)))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CLaser::Free(void)
{
	__super::Free();
}
