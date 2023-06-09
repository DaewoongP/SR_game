#include "stdafx.h"
#include "TopdeeParts.h"

CTopdeeParts::CTopdeeParts(LPDIRECT3DDEVICE9 pGraphicDev)
	:CGameObject(pGraphicDev)
{
	m_RenderState = true;
}

CTopdeeParts::~CTopdeeParts()
{
}

HRESULT CTopdeeParts::Ready_GameObject(_vec3 & vPos, _tchar * texturename, _int idx, _bool setAnim)
{
	m_TextureName = texturename;
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);
	m_pTransform->m_vInfo[INFO_POS] = vPos;
	m_pTextureCom->m_bUseFrameAnimation = setAnim;
	
	return S_OK;
}

_int CTopdeeParts::Update_GameObject(const _float & fTimeDelta)
{
	if (m_bDead)
		return 1;

	__super::Update_GameObject(fTimeDelta);
	m_pTextureCom->Update_Anim(fTimeDelta);
	Engine::Add_RenderGroup(RENDER_ALPHA, this);
	return 0;
}

_int CTopdeeParts::Update_Too(const _float & fTimeDelta)
{
	return _int();
}

_int CTopdeeParts::Update_Top(const _float & fTimeDelta)
{
	return _int();
}

void CTopdeeParts::LateUpdate_GameObject(void)
{
	__super::LateUpdate_GameObject();
}

void CTopdeeParts::Render_GameObject(void)
{
	if (m_bDead)
		return;
	if (m_RenderState)
	{
		_matrix matView, matProj;
		m_pGraphicDev->GetTransform(D3DTS_VIEW, &matView);
		m_pGraphicDev->GetTransform(D3DTS_PROJECTION, &matProj);

		m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransform->Get_WorldMatrixPointer());
		m_pTextureCom->Set_TextureOnShader(m_pShader, "g_Texture", m_TextureIdx);

		m_pShader->Set_Matrix("g_WorldMatrix", m_pTransform->Get_WorldMatrixPointer());
		m_pShader->Set_Matrix("g_ViewMatrix", &matView);
		m_pShader->Set_Matrix("g_ProjMatrix", &matProj);
		m_pShader->Set_Bool("g_Is2D", &g_Is2D);

		m_pShader->Begin(1);
		m_pBufferCom->Render_Buffer();
		m_pShader->End();
		__super::Render_GameObject();
	}	
}

void CTopdeeParts::Render_Too(void)
{
}

void CTopdeeParts::Render_Top(void)
{
}

HRESULT CTopdeeParts::Add_Component(void)
{
	CComponent*		pComponent = nullptr;

	pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(Engine::Clone_Proto(L"RcTex", this));
	NULL_CHECK_RETURN(m_pBufferCom, E_FAIL);
	m_vecComponent[ID_STATIC].push_back({ L"RcTex", pComponent });

	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Engine::Clone_Proto(m_TextureName, this));
	NULL_CHECK_RETURN(m_pTextureCom, E_FAIL);
	m_vecComponent[ID_STATIC].push_back({ L"Texture", pComponent });

	pComponent = m_pShader = dynamic_cast<CShader*>(Engine::Clone_Proto(L"Shader_Rect", this));
	NULL_CHECK_RETURN(m_pShader, E_FAIL);
	m_vecComponent[ID_STATIC].push_back({ L"Shader_Rect", pComponent });

	return S_OK;
}

CTopdeeParts * CTopdeeParts::Create(LPDIRECT3DDEVICE9 pGraphicDev,  _vec3 & vPos,CTransform * trans, _tchar * texturename, _int idx, _bool setAnim)
{
	CTopdeeParts*		pInstance = new CTopdeeParts(pGraphicDev);

	if (FAILED(pInstance->Ready_GameObject(vPos, texturename, idx, setAnim)))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CTopdeeParts::Free(void)
{
	__super::Free();
}
