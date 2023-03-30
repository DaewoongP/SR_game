#include "stdafx.h"
#include "Terrain.h"

#include "Export_Function.h"

CTerrain::CTerrain(LPDIRECT3DDEVICE9 pGraphicDev)
	: CGameObject(pGraphicDev)
{
}

CTerrain::~CTerrain()
{
}

HRESULT CTerrain::Ready_GameObject(void)
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	return S_OK;
}
_int CTerrain::Update_GameObject(const _float& fTimeDelta)
{
	Key_Input(fTimeDelta);

	__super::Update_GameObject(fTimeDelta);

	//m_pBufferCom->m_PlaneVec;

	Engine::Add_RenderGroup(RENDER_NONALPHA, this);

	return 0;
}
void CTerrain::LateUpdate_GameObject(void)
{
	__super::LateUpdate_GameObject();

	
}

void CTerrain::Render_GameObject(void)
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransform->Get_WorldMatrixPointer());

	m_pTextureCom->Set_Texture(1);

	m_pBufferCom->Render_Buffer();

}

HRESULT CTerrain::Add_Component(void)
{
	CComponent*		pComponent = nullptr;

	pComponent = m_pBufferCom = dynamic_cast<CTerrainTex*>(Engine::Clone_Proto(L"TerrainTex",this));
	NULL_CHECK_RETURN(m_pBufferCom, E_FAIL);
	m_uMapComponent[ID_STATIC].insert({ L"TerrainTex", pComponent });

	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Terrain_Texture",this));
	NULL_CHECK_RETURN(m_pTextureCom, E_FAIL);
	m_uMapComponent[ID_STATIC].insert({ L"Terrain_Texture", pComponent });

	return S_OK;
}



CTerrain* CTerrain::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CTerrain*		pInstance = new CTerrain(pGraphicDev);

	if (FAILED(pInstance->Ready_GameObject()))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CTerrain::Free(void)
{
	__super::Free();
}

void CTerrain::Key_Input(const _float & fTimeDelta)
{
	//_vec3		vDir;
	//m_pTransform->Get_Info(INFO_UP, &vDir);

	/*if (GetAsyncKeyState(VK_UP))	m_pTransform->Rotation(ROT_Y, D3DXToRadian(180.f * fTimeDelta));
	if (GetAsyncKeyState(VK_DOWN))	m_pTransform->Rotation(ROT_Y, D3DXToRadian(-180.f * fTimeDelta));*/

}