#include "stdafx.h"
#include "Boss2Body.h"

CBoss2Body::CBoss2Body(LPDIRECT3DDEVICE9 pGraphicDev)
	:CBoss2Parts(pGraphicDev)
{
}

CBoss2Body::~CBoss2Body()
{
}

HRESULT CBoss2Body::Ready_GameObject(_vec3 & vPos)
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);
	m_bIsMoveable = true;
	m_BounusAngle_z = 0;
	m_pTransform->m_vInfo[INFO_POS] = vPos;
	m_OriginTexture = 0;
	m_WhiteTexture = 5;
	m_CurrentTexture = m_OriginTexture;
	return S_OK;
}

_int CBoss2Body::Update_GameObject(const _float & fTimeDelta)
{
	if (m_bIsMoveable)
	{
		if (m_bDir)
		{
			m_pTransform->m_vAngle.z = Lerp(m_pTransform->m_vAngle.z, D3DXToRadian(m_fMin), 0.03f);
			if (m_pTransform->m_vAngle.z < D3DXToRadian(m_fMin + m_fMin*0.1f))
				m_bDir = false;	
		}
		else
		{
			m_pTransform->m_vAngle.z = Lerp(m_pTransform->m_vAngle.z, D3DXToRadian(m_fMax), 0.03f);
			if (m_pTransform->m_vAngle.z > D3DXToRadian(m_fMax- m_fMax*0.1f))
				m_bDir = true;
		}
	}

	__super::Update_GameObject(fTimeDelta);
	Engine::Add_RenderGroup(RENDER_ALPHA, this);
	return 0;
}

_int CBoss2Body::Update_Too(const _float & fTimeDelta)
{
	return _int();
}

_int CBoss2Body::Update_Top(const _float & fTimeDelta)
{
	return _int();
}

void CBoss2Body::LateUpdate_GameObject(void)
{
}

void CBoss2Body::Render_GameObject(void)
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransform->Get_WorldMatrixPointer());
	m_pTextureCom->Set_Texture(m_CurrentTexture);
	m_pBufferCom->Render_Buffer();
	__super::Render_GameObject();
}

void CBoss2Body::Render_Too(void)
{
}

void CBoss2Body::Render_Top(void)
{
}

HRESULT CBoss2Body::Add_Component(void)
{
	CComponent*		pComponent = nullptr;

	pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(Engine::Clone_Proto(L"RcTex", this));
	NULL_CHECK_RETURN(m_pBufferCom, E_FAIL);
	m_vecComponent[ID_STATIC].push_back({ L"RcTex", pComponent });

	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Boss2_Head", this));
	NULL_CHECK_RETURN(m_pTextureCom, E_FAIL);
	m_vecComponent[ID_STATIC].push_back({ L"Texture", pComponent });

	return S_OK;
}

CBoss2Body * CBoss2Body::Create(LPDIRECT3DDEVICE9 pGraphicDev, _vec3 & vPos)
{
	CBoss2Body*		pInstance = new CBoss2Body(pGraphicDev);

	if (FAILED(pInstance->Ready_GameObject(vPos)))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CBoss2Body::Free(void)
{
	__super::Free();
}
