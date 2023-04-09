#include "stdafx.h"
#include "StageBG.h"

#include "Export_Function.h"

CStage1BG::CStage1BG(LPDIRECT3DDEVICE9 pGraphicDev)
	: CGameObject(pGraphicDev)
{
}

CStage1BG::~CStage1BG()
{
}

HRESULT CStage1BG::Ready_GameObject(void)
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);
	m_pTransform->m_vScale = { CUBEX,CUBEY,1 };
	m_pTransform->Set_Pos(CUBEX, CUBEY, 11.101);
	return S_OK;
}
_int CStage1BG::Update_GameObject(const _float& fTimeDelta)
{

	__super::Update_GameObject(fTimeDelta);

	Engine::Add_RenderGroup(RENDER_PRIORITY, this);

	return 0;
}
void CStage1BG::LateUpdate_GameObject(void)
{
	__super::LateUpdate_GameObject();
}

void CStage1BG::Render_GameObject(void)
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransform->Get_WorldMatrixPointer());
	m_pTextureCom->Set_Texture(0);

	m_pBufferCom->Render_Buffer();
}

HRESULT CStage1BG::Add_Component(void)
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



CStage1BG* CStage1BG::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CStage1BG*		pInstance = new CStage1BG(pGraphicDev);

	if (FAILED(pInstance->Ready_GameObject()))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CStage1BG::Free(void)
{
	__super::Free();
}
