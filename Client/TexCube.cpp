#include "stdafx.h"
#include "TexCube.h"
#include "Export_Function.h"
CTexCube::CTexCube(LPDIRECT3DDEVICE9 pGraphicDev)
	:CGameObject(pGraphicDev)
{
}

CTexCube::~CTexCube()
{
}

HRESULT CTexCube::Ready_GameObject(_vec3& vPos)
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);
	m_pTransform->m_vInfo[INFO_POS] = vPos;
	m_pTransform->m_vScale = { 1.f, 1.f, 1.f };

	return S_OK;
}
_int CTexCube::Update_GameObject(const _float & fTimeDelta)
{
	// DeadÃ³¸® ¤¤¤¤
	int iExit = __super::Update_GameObject(fTimeDelta);

	Engine::Add_RenderGroup(RENDER_NONALPHA, this);

	return iExit;
}

void CTexCube::LateUpdate_GameObject(void)
{
	__super::LateUpdate_GameObject();
}

void CTexCube::Render_GameObject(void)
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransform->Get_WorldMatrixPointer());
	m_pTextureCom->Set_Texture(m_iCubeTexNum);
	m_pBufferCom->Render_Buffer();
	__super::Render_GameObject();
}

HRESULT CTexCube::Add_Component(void)
{
	CComponent*		pComponent = nullptr;

	pComponent = m_pBufferCom = dynamic_cast<CCubeTex*>(Engine::Clone_Proto(L"CubeTex", this));
	NULL_CHECK_RETURN(m_pBufferCom, E_FAIL);
	m_vecComponent[ID_STATIC].push_back({ L"CubeTex", pComponent });

	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Stage_Cube", this));
	NULL_CHECK_RETURN(m_pTextureCom, E_FAIL);
	m_vecComponent[ID_STATIC].push_back({ L"Stage_Cube", pComponent });

	return S_OK;
}

CTexCube * CTexCube::Create(LPDIRECT3DDEVICE9 pGraphicDev, _vec3& vPos, _int iIndex)
{
	CTexCube*		pInstance = new CTexCube(pGraphicDev);

	if (FAILED(pInstance->Ready_GameObject(vPos)))
	{
		Safe_Release(pInstance);
		return nullptr;
	}
	pInstance->Set_CubeTexNum(iIndex);
	return pInstance;
}

void CTexCube::Free(void)
{
	__super::Free();
}
