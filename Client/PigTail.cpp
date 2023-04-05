#include "stdafx.h"
#include "PigTail.h"

#include "Export_Function.h"


CPigTail::CPigTail(LPDIRECT3DDEVICE9 pGraphicDev)
	:
	CGameObject(pGraphicDev)
{

}

CPigTail::~CPigTail()
{
}

HRESULT CPigTail::Ready_GameObject(void)
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	m_pTransform->m_vScale = { 0.5f, 0.5f, 0.0f };

	m_pTransform->m_vInfo[INFO_POS] = _vec3(-1.5f, 0.0f, 0.0f);

	return S_OK;
}

_int CPigTail::Update_GameObject(const _float & fTimeDelta)
{
	if (m_pTransform->m_pParent)
	{
		//앞뒤
		if (m_pTransform->m_pParent->Get_WorldMatrixPointer()->_42 > m_pTransform->Get_WorldMatrixPointer()->_42)
		{
			m_pTransform->m_vInfo[INFO_POS].z = -0.1f;
		}
		else 
			m_pTransform->m_vInfo[INFO_POS].z = 0.5f;
		//좌우
		if (m_pTransform->m_pParent->Get_WorldMatrixPointer()->_41 > m_pTransform->Get_WorldMatrixPointer()->_41)
		{
			if(m_pTransform->m_vAngle.y >= D3DXToRadian(0.0f))
			m_pTransform->m_vAngle.y -= D3DXToRadian(5.0f);
		}
		else
		{
			if (m_pTransform->m_vAngle.y <= D3DXToRadian(180.0f))
				m_pTransform->m_vAngle.y += D3DXToRadian(5.0f);
		}


		//m_pTransform->m_vAngle.y = m_pTransform->m_pParent->m_vAngle.y;

		//역회전을 걸려면...부모의 회전의 반대만큼 각도를...
		m_pTransform->m_vAngle.z = 2 * D3DX_PI - m_pTransform->m_pParent->m_vAngle.z;

		__super::Update_GameObject(fTimeDelta);


		Engine::Add_RenderGroup(RENDER_ALPHA, this);
	}
	
	return 0;
}

void CPigTail::LateUpdate_GameObject(void)
{
	__super::LateUpdate_GameObject();
}

void CPigTail::Render_GameObject(void)
{
	if (m_pTransform->m_pParent)
	{
		m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransform->Get_WorldMatrixPointer());

		m_pTextureCom->Set_Texture(PIG_TAIL);

		m_pBufferCom->Render_Buffer();

	}
	__super::Render_GameObject();
}

HRESULT CPigTail::Add_Component(void)
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

CPigTail * CPigTail::Create(LPDIRECT3DDEVICE9 pGraphicDev, CTransform* pParentTrans)
{
	CPigTail*		pInstance = new CPigTail(pGraphicDev);

	if (FAILED(pInstance->Ready_GameObject()))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CPigTail::Free(void)
{
	__super::Free();
}
