#include "stdafx.h"
#include "ShiningStar.h"

CShiningStar::CShiningStar(LPDIRECT3DDEVICE9 pGraphicDev)
	: CGameObject(pGraphicDev)
{
}

CShiningStar::~CShiningStar()
{
}

HRESULT CShiningStar::Ready_GameObject(_vec3 & vPos)
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	_float fRandScale = rand() % 10;

	m_pTransform->m_vScale = { fRandScale * 5.0f, fRandScale * 5.0f, 1.f };

	m_pTransform->m_vInfo[INFO_POS] = vPos;

	m_pTextureCom->Add_Anim(L"Idle", 0, 9, fRandScale + 1.f, true);
	m_pTextureCom->Switch_Anim(L"Idle");
	m_pTextureCom->m_bUseFrameAnimation = true;

	return S_OK;
}

_int CShiningStar::Update_GameObject(const _float & fTimeDelta)
{
	m_pTextureCom->Update_Anim(fTimeDelta);

	__super::Update_GameObject(fTimeDelta);

	Engine::Add_RenderGroup(RENDER_ALPHA, this);

	return 0;
}

void CShiningStar::Render_GameObject(void)
{
	m_pGraphicDev->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);

	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransform->Get_WorldMatrixPointer());

	m_pTextureCom->Set_Texture(0);

	m_pBufferCom->Render_Buffer();

	m_pGraphicDev->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
}

HRESULT CShiningStar::Add_Component(void)
{
	CComponent*		pComponent = nullptr;

	pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(Engine::Clone_Proto(L"RcTex", this));
	NULL_CHECK_RETURN(m_pBufferCom, E_FAIL);
	m_vecComponent[ID_STATIC].push_back({ L"RcTex", pComponent });

	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Spark_Texture", this));
	NULL_CHECK_RETURN(m_pTextureCom, E_FAIL);
	m_vecComponent[ID_STATIC].push_back({ L"Spark_Texture", pComponent });

	return S_OK;
}

CShiningStar * CShiningStar::Create(LPDIRECT3DDEVICE9 pGraphicDev, _vec3 & vPos)
{
	CShiningStar*		pInstance = new CShiningStar(pGraphicDev);

	if (FAILED(pInstance->Ready_GameObject(vPos)))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CShiningStar::Free(void)
{
	__super::Free();
}
