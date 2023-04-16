#include "stdafx.h"
#include "Boss2TailBody.h"
#include "Boss2Tail.h"

#include "AbstractFactory.h"


CBoss2TailBody::CBoss2TailBody(LPDIRECT3DDEVICE9 pGraphicDev)
	:CGameObject(pGraphicDev)
{
}

CBoss2TailBody::~CBoss2TailBody()
{
}

HRESULT CBoss2TailBody::Ready_GameObject(_vec3 & vPos)
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);
	m_pTransform->m_vInfo[INFO_POS] = vPos;

	

	m_bInit = false;


	return S_OK;
}

_int CBoss2TailBody::Update_GameObject(const _float & fTimeDelta)
{
	if (!m_bInit)
	{
		CLayer* pStageLayer = dynamic_cast<CLayer*>(Engine::Get_Layer(L"Layer_GameLogic"));
		NULL_CHECK_RETURN(pStageLayer, E_FAIL);

		FAILED_CHECK_RETURN(FACTORY<CBoss2Tail>::Create(L"Boss2Tail", pStageLayer, _vec3(3.0f, 0.f, 0.05f), m_pTransform), E_FAIL);
		
		m_pTransform->GetChild(0)->m_vScale = _vec3(0.1f, 0.1f, 0.1f);
		m_pTransform->GetChild(0)->m_vAngle = _vec3(0, 0, D3DXToRadian(0.f));

		FAILED_CHECK_RETURN(FACTORY<CBoss2Tail>::Create(L"Boss2Tail_0", pStageLayer, _vec3(0.0f, 0.0f, 10.0f + 0.1f)), E_FAIL);
		dynamic_cast<CBoss2Tail*>(Engine::Get_GameObject(L"Layer_GameLogic", L"Boss2Tail_0"))->
			Set_PreTail(m_pTransform->GetChild(0));

		FAILED_CHECK_RETURN(FACTORY<CBoss2Tail>::Create(L"Boss2Tail_1", pStageLayer, _vec3(0.0f, 0.0f, 10.0f + 0.15f)), E_FAIL);

		dynamic_cast<CBoss2Tail*>(Engine::Get_GameObject(L"Layer_GameLogic", L"Boss2Tail_1"))->
			Set_PreTail(Engine::Get_GameObject(L"Layer_GameLogic", L"Boss2Tail_0")->m_pTransform);

		FAILED_CHECK_RETURN(FACTORY<CBoss2Tail>::Create(L"Boss2Tail_2", pStageLayer, _vec3(0.0f, 0.0f, 10.0f + 0.2f)), E_FAIL);

		dynamic_cast<CBoss2Tail*>(Engine::Get_GameObject(L"Layer_GameLogic", L"Boss2Tail_2"))->
			Set_PreTail(Engine::Get_GameObject(L"Layer_GameLogic", L"Boss2Tail_1")->m_pTransform);

		FAILED_CHECK_RETURN(FACTORY<CBoss2Tail>::Create(L"Boss2Tail_3", pStageLayer, _vec3(0.0f, 0.0f, 10.0f + 0.25f)), E_FAIL);

		dynamic_cast<CBoss2Tail*>(Engine::Get_GameObject(L"Layer_GameLogic", L"Boss2Tail_3"))->
			Set_PreTail(Engine::Get_GameObject(L"Layer_GameLogic", L"Boss2Tail_2")->m_pTransform);
		

		FAILED_CHECK_RETURN(FACTORY<CBoss2Tail>::Create(L"Boss2Tail_4", pStageLayer, _vec3(0.0f, 0.0f, 10.0f + 0.3f)), E_FAIL);

		dynamic_cast<CBoss2Tail*>(Engine::Get_GameObject(L"Layer_GameLogic", L"Boss2Tail_4"))->
			Set_PreTail(Engine::Get_GameObject(L"Layer_GameLogic", L"Boss2Tail_3")->m_pTransform);


		FAILED_CHECK_RETURN(FACTORY<CBoss2Tail>::Create(L"Boss2Tail_5", pStageLayer, _vec3(0.0f, 0.0f, 10.0f + 0.35f)), E_FAIL);

		dynamic_cast<CBoss2Tail*>(Engine::Get_GameObject(L"Layer_GameLogic", L"Boss2Tail_5"))->
			Set_PreTail(Engine::Get_GameObject(L"Layer_GameLogic", L"Boss2Tail_4")->m_pTransform);

		FAILED_CHECK_RETURN(FACTORY<CBoss2Tail>::Create(L"Boss2Tail_6", pStageLayer, _vec3(0.0f, 0.0f, 10.0f + 0.4f)), E_FAIL);

		dynamic_cast<CBoss2Tail*>(Engine::Get_GameObject(L"Layer_GameLogic", L"Boss2Tail_6"))->
			Set_PreTail(Engine::Get_GameObject(L"Layer_GameLogic", L"Boss2Tail_5")->m_pTransform);


		FAILED_CHECK_RETURN(FACTORY<CBoss2Tail>::Create(L"Boss2Tail_7", pStageLayer, _vec3(0.0f, 0.0f, 10.0f + 0.45f)), E_FAIL);

		dynamic_cast<CBoss2Tail*>(Engine::Get_GameObject(L"Layer_GameLogic", L"Boss2Tail_7"))->
			Set_PreTail(Engine::Get_GameObject(L"Layer_GameLogic", L"Boss2Tail_6")->m_pTransform);

		FAILED_CHECK_RETURN(FACTORY<CBoss2Tail>::Create(L"Boss2Tail_8", pStageLayer, _vec3(0.0f, 0.0f, 10.0f + 0.5f)), E_FAIL);

		dynamic_cast<CBoss2Tail*>(Engine::Get_GameObject(L"Layer_GameLogic", L"Boss2Tail_8"))->
			Set_PreTail(Engine::Get_GameObject(L"Layer_GameLogic", L"Boss2Tail_7")->m_pTransform);

		FAILED_CHECK_RETURN(FACTORY<CBoss2Tail>::Create(L"Boss2Tail_9", pStageLayer, _vec3(0.0f, 0.0f, 10.0f + 0.55f)), E_FAIL);

		dynamic_cast<CBoss2Tail*>(Engine::Get_GameObject(L"Layer_GameLogic", L"Boss2Tail_9"))->
			Set_PreTail(Engine::Get_GameObject(L"Layer_GameLogic", L"Boss2Tail_8")->m_pTransform);




		FAILED_CHECK_RETURN(FACTORY<CBoss2Tail>::Create(L"Boss2Tail_10", pStageLayer, _vec3(0.0f, 0.0f, 10.0f + 0.6f)), E_FAIL);

		dynamic_cast<CBoss2Tail*>(Engine::Get_GameObject(L"Layer_GameLogic", L"Boss2Tail_10"))->
			Set_PreTail(Engine::Get_GameObject(L"Layer_GameLogic", L"Boss2Tail_9")->m_pTransform);

		FAILED_CHECK_RETURN(FACTORY<CBoss2Tail>::Create(L"Boss2Tail_11", pStageLayer, _vec3(0.0f, 0.0f, 10.0f + 0.65f)), E_FAIL);

		dynamic_cast<CBoss2Tail*>(Engine::Get_GameObject(L"Layer_GameLogic", L"Boss2Tail_11"))->
			Set_PreTail(Engine::Get_GameObject(L"Layer_GameLogic", L"Boss2Tail_10")->m_pTransform);

		FAILED_CHECK_RETURN(FACTORY<CBoss2Tail>::Create(L"Boss2Tail_12", pStageLayer, _vec3(0.0f, 0.0f, 10.0f + 0.7f)), E_FAIL);

		dynamic_cast<CBoss2Tail*>(Engine::Get_GameObject(L"Layer_GameLogic", L"Boss2Tail_12"))->
			Set_PreTail(Engine::Get_GameObject(L"Layer_GameLogic", L"Boss2Tail_11")->m_pTransform);


		FAILED_CHECK_RETURN(FACTORY<CBoss2Tail>::Create(L"Boss2Tail_13", pStageLayer, _vec3(0.0f, 0.0f, 10.0f + 0.75f)), E_FAIL);

		dynamic_cast<CBoss2Tail*>(Engine::Get_GameObject(L"Layer_GameLogic", L"Boss2Tail_13"))->
			Set_PreTail(Engine::Get_GameObject(L"Layer_GameLogic", L"Boss2Tail_12")->m_pTransform);


		FAILED_CHECK_RETURN(FACTORY<CBoss2Tail>::Create(L"Boss2Tail_14", pStageLayer, _vec3(0.0f, 0.0f, 10.0f + 0.8f)), E_FAIL);

		dynamic_cast<CBoss2Tail*>(Engine::Get_GameObject(L"Layer_GameLogic", L"Boss2Tail_14"))->
			Set_PreTail(Engine::Get_GameObject(L"Layer_GameLogic", L"Boss2Tail_13")->m_pTransform);

		FAILED_CHECK_RETURN(FACTORY<CBoss2Tail>::Create(L"Boss2Tail_15", pStageLayer, _vec3(0.0f, 0.0f, 10.0f + 0.85f)), E_FAIL);

		dynamic_cast<CBoss2Tail*>(Engine::Get_GameObject(L"Layer_GameLogic", L"Boss2Tail_15"))->
			Set_PreTail(Engine::Get_GameObject(L"Layer_GameLogic", L"Boss2Tail_14")->m_pTransform);


		FAILED_CHECK_RETURN(FACTORY<CBoss2Tail>::Create(L"Boss2Tail_16", pStageLayer, _vec3(0.0f, 0.0f, 10.0f + 0.9f)), E_FAIL);

		dynamic_cast<CBoss2Tail*>(Engine::Get_GameObject(L"Layer_GameLogic", L"Boss2Tail_16"))->
			Set_PreTail(Engine::Get_GameObject(L"Layer_GameLogic", L"Boss2Tail_15")->m_pTransform);

		FAILED_CHECK_RETURN(FACTORY<CBoss2Tail>::Create(L"Boss2Tail_17", pStageLayer, _vec3(0.0f, 0.0f, 10.0f + 0.95f)), E_FAIL);

		dynamic_cast<CBoss2Tail*>(Engine::Get_GameObject(L"Layer_GameLogic", L"Boss2Tail_17"))->
			Set_PreTail(Engine::Get_GameObject(L"Layer_GameLogic", L"Boss2Tail_16")->m_pTransform);

		FAILED_CHECK_RETURN(FACTORY<CBoss2Tail>::Create(L"Boss2Tail_18", pStageLayer, _vec3(0.0f, 0.0f, 10.0f + 1.0f)), E_FAIL);

		dynamic_cast<CBoss2Tail*>(Engine::Get_GameObject(L"Layer_GameLogic", L"Boss2Tail_18"))->
			Set_PreTail(Engine::Get_GameObject(L"Layer_GameLogic", L"Boss2Tail_17")->m_pTransform);

		m_bInit = true;
	}

	if (m_bMetronome)
	{
		if (1.0f > m_fTimer)
		{
			m_fTimer += fTimeDelta * 2.0f;

			if (1.0f < m_fTimer)
				m_fTimer = 1.0f;

			m_pTransform->m_vAngle.z = D3DXToRadian(Lerp(BOSETAILWAVESTART, BOSETAILWAVEEND, m_fTimer));

			if (1.0f<= m_fTimer)
			{
				m_fTimer = 0.0f;

				m_bMetronome = false;
			}
			
		}
	}
	else if(!m_bMetronome)
	{
		if (1.0f > m_fTimer)
		{
			m_fTimer += fTimeDelta * 2.0f;

			if (1.0f < m_fTimer)
				m_fTimer = 1.0f;

			m_pTransform->m_vAngle.z = D3DXToRadian(Lerp(BOSETAILWAVEEND, BOSETAILWAVESTART, m_fTimer));

			if (1.0f <= m_fTimer)
			{
				m_fTimer = 0.0f;

				m_bMetronome = true;
			}

			
		}
	}

	__super::Update_GameObject(fTimeDelta);
	Engine::Add_RenderGroup(RENDER_ALPHA, this);
	return 0;
}

_int CBoss2TailBody::Update_Too(const _float & fTimeDelta)
{
	return _int();
}

_int CBoss2TailBody::Update_Top(const _float & fTimeDelta)
{
	return _int();
}

void CBoss2TailBody::LateUpdate_GameObject(void)
{
}

void CBoss2TailBody::Render_GameObject(void)
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransform->Get_WorldMatrixPointer());
	m_pTextureCom->Set_Texture(0);
	m_pBufferCom->Render_Buffer();
	__super::Render_GameObject();
}

void CBoss2TailBody::Render_Too(void)
{
}

void CBoss2TailBody::Render_Top(void)
{
}

HRESULT CBoss2TailBody::Add_Component(void)
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

CBoss2TailBody * CBoss2TailBody::Create(LPDIRECT3DDEVICE9 pGraphicDev, _vec3 & vPos)
{
	CBoss2TailBody*		pInstance = new CBoss2TailBody(pGraphicDev);

	if (FAILED(pInstance->Ready_GameObject(vPos)))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CBoss2TailBody::Free(void)
{
	__super::Free();
}
