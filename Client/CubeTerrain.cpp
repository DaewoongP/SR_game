#include "stdafx.h"
#include "CubeTerrain.h"

#include "Export_Function.h"

CCubeTerrain::CCubeTerrain(LPDIRECT3DDEVICE9 pGraphicDev)
	:CGameObject(pGraphicDev),
	m_iCubeIndex(1)
{
}

CCubeTerrain::~CCubeTerrain()
{
}

HRESULT CCubeTerrain::Ready_GameObject(void)
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	m_pTransform->m_vInfo[INFO_POS] = _vec3(0.f, 0.f, 0.f);

	return S_OK;
}

_int CCubeTerrain::Update_GameObject(const _float & fTimeDelta)
{
	Key_Input();

	__super::Update_GameObject(fTimeDelta);

	Engine::Add_RenderGroup(RENDER_ALPHA, this);

	return 0;
}

void CCubeTerrain::LateUpdate_GameObject(void)
{
	__super::LateUpdate_GameObject();
}

void CCubeTerrain::Render_GameObject(void)
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransform->Get_WorldMatrixPointer());

	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	m_pGraphicDev->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);

	//m_pGraphicDev->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);

	//m_pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	//m_pGraphicDev->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	//m_pGraphicDev->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	//m_pGraphicDev->SetRenderState(D3DRS_ALPHATESTENABLE,TRUE);
	//m_pGraphicDev->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);
	//m_pGraphicDev->SetRenderState(D3DRS_ALPHAREF, 0xc0);

	m_pTextureCom->Set_Texture(m_iCubeIndex);

	m_pBufferCom->Render_Buffer();

	//m_pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
	//m_pGraphicDev->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
	//m_pGraphicDev->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);

	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}

HRESULT CCubeTerrain::Add_Component(void)
{
	CComponent*		pComponent = nullptr;

	pComponent = m_pBufferCom = dynamic_cast<CCubeTex*>(Engine::Clone_Proto(L"CubeTex", this));
	NULL_CHECK_RETURN(m_pBufferCom, E_FAIL);
	m_uMapComponent[ID_STATIC].insert({ L"CubeTex", pComponent });

	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Plant_Texture", this));
	NULL_CHECK_RETURN(m_pTextureCom, E_FAIL);
	m_uMapComponent[ID_STATIC].insert({ L"Plant_Texture", pComponent });

	return S_OK;
}

void CCubeTerrain::Key_Input()
{
	_vec3		vDir;
	_vec3		vRight;
	m_pTransform->Get_Info(INFO_LOOK, &vDir);
	m_pTransform->Get_Info(INFO_RIGHT, &vRight);

	if (GetAsyncKeyState(VK_UP))	m_pTransform->Move_Pos(&vDir, 1, 1.f);
	if (GetAsyncKeyState(VK_DOWN))	m_pTransform->Move_Pos(&vDir, 1, -1.f);
	if (GetAsyncKeyState(VK_LEFT))	m_pTransform->Move_Pos(&vRight, 1, -1.f);
	if (GetAsyncKeyState(VK_RIGHT))	m_pTransform->Move_Pos(&vRight,1, 1.f);
}

CCubeTerrain * CCubeTerrain::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CCubeTerrain*		pInstance = new CCubeTerrain(pGraphicDev);

	if (FAILED(pInstance->Ready_GameObject()))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CCubeTerrain::Free(void)
{
	__super::Free();
}
