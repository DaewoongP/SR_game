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
	m_pTransform->Set_Pos(CUBEX, CUBEY, _float(11.101));

	BoundingBox box;
	box._min = { 0.f, 0.f, 3.f };
	box._max = { 70.f, 40.f, 11.f };
	m_pLeafParticle0->Set_BoundingBox(box);
	m_pLeafParticle0->Set_AnimSpeed(0.01f);
	m_pLeafParticle0->Start_Particle();
	m_pLeafParticle1->Set_BoundingBox(box);
	m_pLeafParticle1->Set_AnimSpeed(0.01f);
	m_pLeafParticle1->Start_Particle();
	m_pLeafParticle2->Set_BoundingBox(box);
	m_pLeafParticle2->Set_AnimSpeed(0.01f);
	m_pLeafParticle2->Start_Particle();

	return S_OK;
}
_int CStage1BG::Update_GameObject(const _float& fTimeDelta)
{

	__super::Update_GameObject(fTimeDelta);

	Engine::Add_RenderGroup(RENDER_ALPHABLEND, this);

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
	Render_Particle();
}

HRESULT CStage1BG::Add_Component(void)
{
	CComponent*		pComponent = nullptr;

	pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(Engine::Clone_Proto(L"RcTex", this));
	NULL_CHECK_RETURN(m_pBufferCom, E_FAIL);
	m_vecComponent[ID_STATIC].push_back({ L"RcTex", pComponent });

	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Stage_Texture", this));
	NULL_CHECK_RETURN(m_pTextureCom, E_FAIL);
	m_vecComponent[ID_STATIC].push_back({ L"Stage_Texture", pComponent });

	pComponent = m_pLeafParticle0 = dynamic_cast<CLeafParticle*>(Engine::Clone_Proto(L"LeafParticle0", this));
	NULL_CHECK_RETURN(m_pLeafParticle0, E_FAIL);
	m_vecComponent[ID_STATIC].push_back({ L"LeafParticle0", pComponent });

	pComponent = m_pLeafParticle1 = dynamic_cast<CLeafParticle*>(Engine::Clone_Proto(L"LeafParticle1", this));
	NULL_CHECK_RETURN(m_pLeafParticle1, E_FAIL);
	m_vecComponent[ID_STATIC].push_back({ L"LeafParticle1", pComponent });

	pComponent = m_pLeafParticle2 = dynamic_cast<CLeafParticle*>(Engine::Clone_Proto(L"LeafParticle2", this));
	NULL_CHECK_RETURN(m_pLeafParticle2, E_FAIL);
	m_vecComponent[ID_STATIC].push_back({ L"LeafParticle2", pComponent });

	return S_OK;
}

void CStage1BG::Render_Particle()
{
	m_pLeafParticle0->Update_Particle();
	m_pLeafParticle1->Update_Particle();
	m_pLeafParticle2->Update_Particle();
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
