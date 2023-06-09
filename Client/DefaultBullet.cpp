#include "stdafx.h"
#include "DefaultBullet.h"

CDefaultBullet::CDefaultBullet(LPDIRECT3DDEVICE9 pGraphicDev)
	:CBullet(pGraphicDev)
{
}

CDefaultBullet::~CDefaultBullet()
{
}

HRESULT CDefaultBullet::Ready_Bullet(_vec3 & vPos, _vec3 & vDir)
{
	__super::Ready_Bullet(vPos, vDir);
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);
	m_pTex->Add_Anim(L"Idle", 0, 3, 1.f, true);
	m_pTex->Switch_Anim(L"Idle");
	m_pTex->m_bUseFrameAnimation = true;
	m_fSpeed = 50.f;

	return S_OK;
}

void CDefaultBullet::Ready_Pool(_vec3 & vPos, _vec3 & vDir)
{
	__super::Ready_Pool(vPos, vDir);
	m_pTex->Add_Anim(L"Idle", 0, 3, 1.f, true);
	m_pTex->Switch_Anim(L"Idle");
	m_pTex->m_bUseFrameAnimation = true;
	m_fSpeed = 50.f;
}

_int CDefaultBullet::Update_GameObject(const _float & fTimeDelta)
{
	_int iResult = 0;
	_matrix mat;
	//m_pGraphicDev->GetTransform(D3DTS_VIEW, &mat);
	//m_pTransform->Set_BillboardY(&mat);
	m_pTransform->m_vInfo[INFO_POS].z +=  m_fSpeed * fTimeDelta;
	m_pTex->Update_Anim(fTimeDelta);
	iResult = __super::Update_GameObject(fTimeDelta);
	return iResult;
}

void CDefaultBullet::LateUpdate_GameObject(void)
{
	__super::LateUpdate_GameObject();
}

void CDefaultBullet::Render_GameObject(void)
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransform->Get_WorldMatrixPointer());

	m_pTex->Set_Texture();
	m_pBuf->Render_Buffer();

	__super::Render_GameObject();
}

HRESULT CDefaultBullet::Add_Component(void)
{
	CComponent*		pComponent = nullptr;

	pComponent = m_pTex = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Bullet", this));
	NULL_CHECK_RETURN(m_pTex, E_FAIL);
	m_vecComponent[ID_STATIC].push_back({ L"Bullet", pComponent });
	return S_OK;
}

CDefaultBullet * CDefaultBullet::Create(LPDIRECT3DDEVICE9 pGraphicDev, _vec3 & vPos, _vec3 & vDir)
{
	CDefaultBullet* pInstance = new CDefaultBullet(pGraphicDev);
	if (FAILED(pInstance->Ready_Bullet(vPos, vDir)))
	{
		Safe_Release(pInstance);
		return nullptr;
	}
	return pInstance;
}

void CDefaultBullet::Free(void)
{
	__super::Free();
}
