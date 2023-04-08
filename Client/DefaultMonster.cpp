#include "stdafx.h"
#include "DefaultMonster.h"
#include "Export_Function.h"

CDefaultMonster::CDefaultMonster(PDIRECT3DDEVICE9 pGraphicDev)
	:CMonster(pGraphicDev)
{
}

CDefaultMonster::~CDefaultMonster()
{
}

HRESULT CDefaultMonster::Ready_GameObject(_vec3& vPos)
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	m_pTransform->m_vInfo[INFO_POS] = vPos;
	m_pTransform->m_vScale = { 1.f, 1.f, 1.f };
	return S_OK;
}

_int CDefaultMonster::Update_GameObject(const _float & fTimeDelta)
{
	if (m_bDead)
		return OBJ_DEAD;

	Mouse_Move();

	CGameObject::Update_GameObject(fTimeDelta);

	Engine::Add_RenderGroup(RENDER_NONALPHA, this);

	return 0;
}

void CDefaultMonster::LateUpdate_GameObject(void)
{
	CGameObject::LateUpdate_GameObject();
}

void CDefaultMonster::Render_GameObject(void)
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransform->Get_WorldMatrixPointer());

	m_pTextureCom->Set_Texture();

	m_pBufferCom->Render_Buffer();

	CGameObject::Render_GameObject();
}

HRESULT CDefaultMonster::Add_Component(void)
{
	CComponent*		pComponent = nullptr;

	pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(Engine::Clone_Proto(L"RcTex", this));
	NULL_CHECK_RETURN(m_pBufferCom, E_FAIL);
	m_uMapComponent[ID_STATIC].insert({ L"RcTex", pComponent });

	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Default_Monster_Texture", this));
	NULL_CHECK_RETURN(m_pTextureCom, E_FAIL);
	m_uMapComponent[ID_STATIC].insert({ L"Default_Monster_Texture", pComponent });

	return S_OK;
}

void CDefaultMonster::Mouse_Move()
{
	POINT	pt{};
	GetCursorPos(&pt);
	ScreenToClient(g_hWnd, &pt);

	int x = 1366 / CUBEX * 2;
	int y = 768 / CUBEY * 2;

	int nx = 1366 / x;
	int ny = 768 / y;

	float Gridx = pt.x / nx / (1.3f) - 1.f;
	float Gridy = (768 / ny - pt.y / ny) / (2.35f) - 1.f;

	m_pTransform->m_vInfo[INFO_POS].x = Gridx;
	m_pTransform->m_vInfo[INFO_POS].y = Gridy;
	m_pTransform->m_vInfo[INFO_POS].z = 10.f;
}

CDefaultMonster * CDefaultMonster::Create(LPDIRECT3DDEVICE9 pGraphicDev, _vec3& vPos)
{
	CDefaultMonster*		pInstance = new CDefaultMonster(pGraphicDev);

	if (FAILED(pInstance->Ready_GameObject(vPos)))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CDefaultMonster::Free(void)
{
	CGameObject::Free();
}
