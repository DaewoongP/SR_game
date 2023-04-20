#include "stdafx.h"
#include "DefaultBG.h"

CDefaultBG::CDefaultBG(PDIRECT3DDEVICE9 pGraphicdev):CBackGroundBase(pGraphicdev)
{
}

CDefaultBG::~CDefaultBG()
{
}

HRESULT CDefaultBG::Ready_GameObject(_vec3& vPos)
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	m_pTransform->m_vInfo[INFO_POS] = vPos;
	m_pTransform->m_vScale = { 1.f, 1.f, 1.f };

	return S_OK;
}

_int CDefaultBG::Update_GameObject(const _float& fTimeDelta)
{
	Mouse_Move();

	CGameObject::Update_GameObject(fTimeDelta);


	Engine::Add_RenderGroup(RENDER_PRIORITY, this);

	return 0;
}

void CDefaultBG::LateUpdate_GameObject(void)
{
	CGameObject::LateUpdate_GameObject();

}

void CDefaultBG::Render_GameObject(void)
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransform->Get_WorldMatrixPointer());
	m_pTextureCom->Set_Texture();
	m_pBufferCom->Render_Buffer();

	CGameObject::Render_GameObject();
}

HRESULT CDefaultBG::Add_Component(void)
{
	CComponent* pComponent = nullptr;

	pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(Engine::Clone_Proto(L"RcTex", this));
	NULL_CHECK_RETURN(m_pBufferCom, E_FAIL);
	m_vecComponent[ID_STATIC].push_back({ L"RcTex", pComponent });

	// 디폴트 몬스터
	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Default_BG_Texture", this));
	NULL_CHECK_RETURN(m_pTextureCom, E_FAIL);
	m_vecComponent[ID_STATIC].push_back({ L"Default_BG_Texture", pComponent });
	return S_OK;
}

void CDefaultBG::Mouse_Move()
{
	POINT	pt{};
	GetCursorPos(&pt);
	ScreenToClient(g_hWnd, &pt);

	_int x = 1366 / CUBEX * 2;
	_int y = 768 / CUBEY * 2;

	_int nx = 1366 / x;
	_int ny = 768 / y;

	_float Gridx = pt.x / nx / (1.3f) - 1.f;
	_float Gridy = (768 / ny - pt.y / ny) / (2.35f) - 1.f;

	_int installx = _int(Gridx / 2);
	_int instally = _int(Gridy / 2);

	m_pTransform->m_vInfo[INFO_POS].x = installx * 2.f;
	m_pTransform->m_vInfo[INFO_POS].y = instally * 2.f;
	m_pTransform->m_vInfo[INFO_POS].z = 10.f;
}

CDefaultBG* CDefaultBG::Create(LPDIRECT3DDEVICE9 pGraphicDev, _vec3& vPos)
{
	CDefaultBG* pInstance = new CDefaultBG(pGraphicDev);

	if (FAILED(pInstance->Ready_GameObject(vPos)))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CDefaultBG::Free(void)
{
	__super::Free();
}
