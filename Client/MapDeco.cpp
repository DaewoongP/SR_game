#include "stdafx.h"
#include "MapDeco.h"

CMapDeco::CMapDeco(LPDIRECT3DDEVICE9 pGraphicDev) :CBackGroundBase(pGraphicDev)
{
}

CMapDeco::~CMapDeco()
{
}

HRESULT CMapDeco::Ready_GameObject(_vec3& vPos)
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);


	m_pTransform->m_vScale = { 1.f, 1.f, 1.f };

	m_pTransform->m_vInfo[INFO_POS] = vPos;
	
	switch (m_eDir)
	{
	case Engine::CD_UP:
		break;
	case Engine::CD_LEFT:
		m_pTransform->Rotation(ROT_Z, D3DXToRadian(90));
		break;
	case Engine::CD_DOWN:
		m_pTransform->Rotation(ROT_Z, D3DXToRadian(180));
		break;
	case Engine::CD_RIGHT:
		m_pTransform->Rotation(ROT_Z, D3DXToRadian(270));
		break;
	}
	return S_OK;
}

_int CMapDeco::Update_GameObject(const _float& fTimeDelta)
{
	__super::Update_GameObject(fTimeDelta);

	Engine::Add_RenderGroup(RENDER_ALPHA, this);
	return 0;
}

void CMapDeco::Render_GameObject(void)
{
	m_pGraphicDev->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);

	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransform->Get_WorldMatrixPointer());

	m_pTextureCom->Set_Texture(0);

	m_pBufferCom->Render_Buffer();

	m_pGraphicDev->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
}

HRESULT CMapDeco::Add_Component(void)
{
	CComponent* pComponent = nullptr;

	pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(Engine::Clone_Proto(L"RcTex", this));
	NULL_CHECK_RETURN(m_pBufferCom, E_FAIL);
	m_vecComponent[ID_STATIC].push_back({ L"RcTex", pComponent });

	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Map_Deco", this));
	NULL_CHECK_RETURN(m_pTextureCom, E_FAIL);
	m_vecComponent[ID_STATIC].push_back({ L"Map_Deco", pComponent });

	return S_OK;
}
void CMapDeco::Set_CubeDir(CUBE_DIR dir)
{
	m_eDir = dir;
	_vec3 dirvec[CD_END] = { {0,1,0},{0,-1,0},{-1,0,0},{1,0,0} };
	m_DirVec = dirvec[dir];
}
CMapDeco* CMapDeco::Create(LPDIRECT3DDEVICE9 pGraphicDev, _vec3& vPos,int eDir)
{
	CMapDeco* pInstance = new CMapDeco(pGraphicDev);
	pInstance->Set_CubeDir((CUBE_DIR)eDir);


	if (FAILED(pInstance->Ready_GameObject(vPos)))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CMapDeco::Free(void)
{
	__super::Free();
}
