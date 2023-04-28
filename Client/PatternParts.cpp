#include "stdafx.h"
#include "PatternParts.h"
#include "Export_Function.h"

CPatternParts::CPatternParts(LPDIRECT3DDEVICE9 pGraphicDev) :CGameObject(pGraphicDev)
{
}

CPatternParts::~CPatternParts()
{
}

HRESULT CPatternParts::Ready_GameObject(_vec3 & vPos, _tchar * texturename, _int idx, _bool setAnim)
{
	m_TextureName = texturename;
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);
	m_pTransform->m_vInfo[INFO_POS] = vPos;
	m_TextureIdx = idx;
	return S_OK;
}

_int CPatternParts::Update_GameObject(const _float & fTimeDelta)
{
	Engine::Add_RenderGroup(RENDER_ALPHA, this);
	__super::Update_GameObject(fTimeDelta);
	return 0;
}

void CPatternParts::LateUpdate_GameObject(void)
{
	__super::LateUpdate_GameObject();
}

void CPatternParts::Render_GameObject(void)
{
	__super::Render_GameObject();
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransform->Get_WorldMatrixPointer());
	m_pTextureCom->Set_Texture(m_TextureIdx);
	m_pBufferCom->Render_Buffer();
}

HRESULT CPatternParts::Add_Component(void)
{
	CComponent*		pComponent = nullptr;

	pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(Engine::Clone_Proto(L"RcTex", this));
	NULL_CHECK_RETURN(m_pBufferCom, E_FAIL);
	m_vecComponent[ID_STATIC].push_back({ L"RcTex", pComponent });

	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Engine::Clone_Proto(m_TextureName, this));
	NULL_CHECK_RETURN(m_pTextureCom, E_FAIL);
	m_vecComponent[ID_STATIC].push_back({ L"Texture", pComponent });

	return S_OK;
}

CPatternParts * CPatternParts::Create(LPDIRECT3DDEVICE9 pGraphicDev, _vec3 & vPos, CTransform * trans, _tchar * texturename, _int idx, _bool setAnim)
{
	CPatternParts*		pInstance = new CPatternParts(pGraphicDev);

	if (FAILED(pInstance->Ready_GameObject(vPos, texturename, idx, setAnim)))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CPatternParts::Free(void)
{
	__super::Free();
}
