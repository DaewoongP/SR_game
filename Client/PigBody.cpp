#include "stdafx.h"
#include "PigBody.h"

#include "Export_Function.h"


CPigBody::CPigBody(LPDIRECT3DDEVICE9 pGraphicDev)
	:
	CGameObject(pGraphicDev)
{

}

CPigBody::~CPigBody()
{
}

HRESULT CPigBody::Ready_GameObject(void)
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	m_pTransform->m_vScale = { 2.0f, 2.0f, 0.0f };

	m_pTransform->m_vInfo[INFO_POS] = _vec3(0.0f, 0.0f, 0.0f);

	//m_pTransform->m_vAngle.x = 90.0f;


	return S_OK;
}

_int CPigBody::Update_GameObject(const _float & fTimeDelta)
{


	return 0;
}

void CPigBody::LateUpdate_GameObject(void)
{

}

void CPigBody::Render_GameObject(void)
{

}

_int CPigBody::Update_Top(const _float & fTimeDelta)
{
	if (m_pTransform->m_pParent)
	{
		//ÁÂ¿ì
		if (m_pTransform->m_pParent->Get_WorldMatrixPointer()->_41 > m_pTransform->Get_WorldMatrixPointer()->_41)
		{
			m_pTransform->m_vAngle.y = D3DXToRadian(0.0f);
		}
		else
		{
			m_pTransform->m_vAngle.y = D3DXToRadian(180.0f);
		}

		//¿ªÈ¸Àü
		m_pTransform->m_vAngle.z = 2 * D3DX_PI - m_pTransform->m_pParent->m_vAngle.z;

		CGameObject::Update_GameObject(fTimeDelta);

		Engine::Add_RenderGroup(RENDER_ALPHA, this);
	}

	return _int();
}

void CPigBody::Render_Top()
{
	if (m_pTransform->m_pParent)
	{
		m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
		m_pGraphicDev->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);

		m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransform->Get_WorldMatrixPointer());

		m_pTextureCom->Set_Texture(PIG_BODY);

		m_pBufferCom->Render_Buffer();

		m_pGraphicDev->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
		m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	}

	CGameObject::Render_GameObject();
}

HRESULT CPigBody::Add_Component(void)
{
	CComponent*		pComponent = nullptr;

	pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(Engine::Clone_Proto(L"RcTex", this));
	NULL_CHECK_RETURN(m_pBufferCom, E_FAIL);
	m_uMapComponent[ID_STATIC].insert({ L"RcTex", pComponent });

	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Pig_Parts_Texture", this));
	NULL_CHECK_RETURN(m_pTextureCom, E_FAIL);
	m_uMapComponent[ID_STATIC].insert({ L"Pig_Parts_Texture", pComponent });

	return S_OK;
}

CPigBody * CPigBody::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CPigBody*		pInstance = new CPigBody(pGraphicDev);

	if (FAILED(pInstance->Ready_GameObject()))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CPigBody::Free(void)
{
	__super::Free();
}
