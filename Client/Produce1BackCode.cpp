#include "stdafx.h"
#include "Produce1BackCode.h"


CProduce1BackCode::CProduce1BackCode(LPDIRECT3DDEVICE9 pGraphicDev)
	:
	CProduceObject(pGraphicDev), iTextureIndex(0)
{
}

CProduce1BackCode::~CProduce1BackCode()
{
}

HRESULT CProduce1BackCode::Ready_GameObject(void)
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);
	
	m_pTransform->m_vScale = {WINCX*0.4f,WINCY*0.4f,0.0f};

		m_pTransform->m_vInfo[INFO_POS].z = 50.0f;

		m_vecLerpList.push_back(_vec3(0.0f, 0.0f, 0.0f));
		m_vecLerpList.push_back(_vec3(0.0f, 0.0f, 0.2f));
		m_vecLerpList.push_back(_vec3(0.0f, 0.0f, 1.0f));
		m_vecLerpList.push_back(_vec3(0.0f, 0.0f, 1.0f));
		m_vecLerpList.push_back(_vec3(0.0f, 0.0f, 1.0f));
		m_vecLerpList.push_back(_vec3(0.0f, 0.0f, 1.0f));
		m_vecLerpList.push_back(_vec3(0.0f, 0.0f, 1.0f));
		m_vecLerpList.push_back(_vec3(0.0f, 0.0f, 1.0f));
		m_vecLerpList.push_back(_vec3(0.0f, 0.0f, 1.0f));
		m_vecLerpList.push_back(_vec3(0.0f, 0.0f, 1.0f));

	return S_OK;
}

_int CProduce1BackCode::Update_GameObject(const _float & fTimeDelta)
{
	if (m_bDead)
		return OBJ_DEAD;

	
	Update_Produce(fTimeDelta);
		On_Next(2);

		if (8 == m_iIndex)
		{
			iTextureIndex = 1;
		}
	

	CGameObject::Update_GameObject(fTimeDelta);
	Engine::Add_RenderGroup(RENDER_UI, this);

	return 0;
}

void CProduce1BackCode::Render_GameObject(void)
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransform->Get_WorldMatrixPointer());
	
	m_pTextureCom->Set_Texture(iTextureIndex);

	m_pBufferCom->Render_Buffer();
}

CProduce1BackCode * CProduce1BackCode::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CProduce1BackCode*		pInstance = new CProduce1BackCode(pGraphicDev);

	if (FAILED(pInstance->Ready_GameObject()))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

HRESULT CProduce1BackCode::Add_Component()
{
	CComponent*		pComponent = nullptr;

	pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(Engine::Clone_Proto(L"RcTex", this));
	NULL_CHECK_RETURN(m_pBufferCom, E_FAIL);
	m_vecComponent[ID_STATIC].push_back({ L"RcTex", pComponent });

	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"BackCode", this));
	NULL_CHECK_RETURN(m_pTextureCom, E_FAIL);
	m_vecComponent[ID_STATIC].push_back({ L"BackCode", pComponent });

	return S_OK;
}

void CProduce1BackCode::Free()
{
	CGameObject::Free();
}
