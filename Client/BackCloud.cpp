#include "stdafx.h"
#include "BackCloud.h"

CBackCloud::CBackCloud(LPDIRECT3DDEVICE9 pGraphicDev) :CBackGroundBase(pGraphicDev)
{
}

CBackCloud::~CBackCloud()
{
}

HRESULT CBackCloud::Ready_GameObject(_vec3& vPos)
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);


	m_pTransform->m_vScale = { 15.f, 15.f, 1.f };



	m_pTransform->m_vInfo[INFO_POS] = vPos;

	return S_OK;
}

_int CBackCloud::Update_GameObject(const _float& fTimeDelta)
{
	m_pTransform->Rotation(ROT_Z, D3DXToRadian(0.3f));
	__super::Update_GameObject(fTimeDelta);

	Engine::Add_RenderGroup(RENDER_ALPHA, this);
	return 0;
}

void CBackCloud::Render_GameObject(void)
{
	m_pGraphicDev->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);

	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransform->Get_WorldMatrixPointer());

	m_pTextureCom->Set_Texture(0);

	m_pBufferCom->Render_Buffer();

	m_pGraphicDev->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
}

HRESULT CBackCloud::Add_Component(void)
{
	CComponent* pComponent = nullptr;

	pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(Engine::Clone_Proto(L"RcTex", this));
	NULL_CHECK_RETURN(m_pBufferCom, E_FAIL);
	m_vecComponent[ID_STATIC].push_back({ L"RcTex", pComponent });

	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"BackCloud", this));
	NULL_CHECK_RETURN(m_pTextureCom, E_FAIL);
	m_vecComponent[ID_STATIC].push_back({ L"BackCloud", pComponent });

	return S_OK;
}

CBackCloud* CBackCloud::Create(LPDIRECT3DDEVICE9 pGraphicDev, _vec3& vPos)
{
	CBackCloud* pInstance = new CBackCloud(pGraphicDev);

	if (FAILED(pInstance->Ready_GameObject(vPos)))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CBackCloud::Free(void)
{
	__super::Free();
}
