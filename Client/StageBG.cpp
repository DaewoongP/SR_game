#include "stdafx.h"
#include "StageBG.h"

#include "Export_Function.h"

CStageBG::CStageBG(LPDIRECT3DDEVICE9 pGraphicDev)
	: CGameObject(pGraphicDev)
{
}

CStageBG::~CStageBG()
{
}

HRESULT CStageBG::Ready_GameObject(void)
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);
	m_pTransform->m_vScale = { CUBEX,CUBEY,1 };
	m_pTransform->Set_Pos(CUBEX, CUBEY, 11.101);
	return S_OK;
}
_int CStageBG::Update_GameObject(const _float& fTimeDelta)
{

	__super::Update_GameObject(fTimeDelta);

	Engine::Add_RenderGroup(RENDER_PRIORITY, this);

	return 0;
}
void CStageBG::LateUpdate_GameObject(void)
{
	__super::LateUpdate_GameObject();
}

void CStageBG::Render_GameObject(void)
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransform->Get_WorldMatrixPointer());
	m_pTextureCom->Set_Texture(0);

	m_pBufferCom->Render_Buffer();
}

HRESULT CStageBG::Add_Component(void)
{
	CComponent*		pComponent = nullptr;

	pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(Engine::Clone_Proto(L"RcTex", this));
	NULL_CHECK_RETURN(m_pBufferCom, E_FAIL);
	m_uMapComponent[ID_STATIC].insert({ L"RcTex", pComponent });


	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Stage_Texture", this));
	NULL_CHECK_RETURN(m_pTextureCom, E_FAIL);
	m_uMapComponent[ID_STATIC].insert({ L"Stage_Texture", pComponent });

	return S_OK;
}



CStageBG* CStageBG::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CStageBG*		pInstance = new CStageBG(pGraphicDev);

	if (FAILED(pInstance->Ready_GameObject()))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CStageBG::Free(void)
{
	__super::Free();
}
