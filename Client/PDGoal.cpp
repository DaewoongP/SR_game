#include "stdafx.h"
#include "PDGoal.h"
CPDGoal::CPDGoal(LPDIRECT3DDEVICE9 pGraphicDev)
	:
	CProduceObject(pGraphicDev), iTextureIndex(0), m_bPlaySound(false)
{
}

CPDGoal::~CPDGoal()
{
}

HRESULT CPDGoal::Ready_GameObject(void)
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	m_pTransform->m_vInfo[INFO_POS].x = 0.0f;
	m_pTransform->m_vInfo[INFO_POS].z = 30.0f;
	m_pTransform->m_vScale = { WINCY,WINCY,0.0f };

	m_vecLerpList.push_back(_vec3(0.0f, 0.0f, 1.0f));
	m_vecLerpList.push_back(_vec3(-500.0f, 0.0f, 0.2f));
	m_vecLerpList.push_back(_vec3(-1500.0f, 0.0f, 1.0f));
	m_vecLerpList.push_back(_vec3(-1500.0f, 0.0f, 1.0f));
	m_vecLerpList.push_back(_vec3(-1500.0f, 0.0f, 1.0f));
	m_vecLerpList.push_back(_vec3(-1500.0f, 0.0f, 1.0f));
	m_vecLerpList.push_back(_vec3(-1500.0f, 0.0f, 1.0f));

	return S_OK;
}

_int CPDGoal::Update_GameObject(const _float & fTimeDelta)
{
	if (m_bDead)
		return OBJ_DEAD;

	Update_Produce(fTimeDelta);
	On_Next(4);


	CGameObject::Update_GameObject(fTimeDelta);
	Engine::Add_RenderGroup(RENDER_UI, this);

	return 0;
}

void CPDGoal::Render_GameObject(void)
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransform->Get_WorldMatrixPointer());

	m_pTextureCom->Set_Texture(iTextureIndex);

	m_pBufferCom->Render_Buffer();
}

CPDGoal * CPDGoal::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CPDGoal*		pInstance = new CPDGoal(pGraphicDev);

	if (FAILED(pInstance->Ready_GameObject()))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

HRESULT CPDGoal::Add_Component()
{
	CComponent*		pComponent = nullptr;

	pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(Engine::Clone_Proto(L"RcTex", this));
	NULL_CHECK_RETURN(m_pBufferCom, E_FAIL);
	m_vecComponent[ID_STATIC].push_back({ L"RcTex", pComponent });

	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"GoalThirdee", this));
	NULL_CHECK_RETURN(m_pTextureCom, E_FAIL);
	m_vecComponent[ID_STATIC].push_back({ L"GoalThirdee", pComponent });

	return S_OK;
}

void CPDGoal::Free()
{
	__super::Free();
}