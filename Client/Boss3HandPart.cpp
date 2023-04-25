#include "stdafx.h"
#include "Boss3HandPart.h"
#include"..\Engine\Export_Function.h"

CBoss3HandPart::CBoss3HandPart(LPDIRECT3DDEVICE9 pGraphicDev)
	: CGameObject(pGraphicDev),
	m_bSparkON(false),
	m_fDamagedTime(0.f),
	m_bDamaged(false),
	m_bWhiteIndex(0)
{
	m_pBoss3 = nullptr;
	m_pBoss3LPart = nullptr;
	m_pBoss3RPart = nullptr;
}

CBoss3HandPart::~CBoss3HandPart()
{
}

HRESULT CBoss3HandPart::Ready_GameObject(_vec3& vPos,_int iIndex)
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);
	m_pTransform->m_vInfo[INFO_POS] = vPos;
	m_iIndex = iIndex;
	m_pTransform->m_vScale = { 1.f,1.f,1.f };
	m_pTransform->Rotation(ROT_Y, D3DXToRadian(45.f));

	m_pTransform->m_bIsStatic = true;
	m_pBoss3 = Engine::Get_GameObject(L"Layer_GameLogic", L"Boss3");
	NULL_CHECK_RETURN(m_pBoss3, E_FAIL);

	m_pSparkAnimation->Add_Anim(L"Spark", 0, 5, 0.1f, true);
	m_pSparkAnimation->Switch_Anim(L"Spark");
	m_pSparkAnimation->m_bUseFrameAnimation = true;

	m_bWhiteIndex = m_iIndex + 1;

	return S_OK;
}

_int CBoss3HandPart::Update_GameObject(const _float& fTimeDelta)
{
	if (m_bDead)
		return OBJ_DEAD;

	if (m_bDamaged)
		DamagedBoss3(fTimeDelta);

	m_pBoss3LPart = Engine::Get_GameObject(L"Layer_GameLogic", L"Boss3Left");
	NULL_CHECK_RETURN(m_pBoss3LPart, E_FAIL);

	m_pBoss3RPart = Engine::Get_GameObject(L"Layer_GameLogic", L"Boss3Right");
	NULL_CHECK_RETURN(m_pBoss3RPart, E_FAIL);

	_vec3 Lerp;

	__super::Update_GameObject(fTimeDelta);

		if (!lstrcmp(m_pTag, L"Boss3LPart")||!lstrcmp(m_pTag, L"Boss3LPartShadow"))
		{
			D3DXVec3Lerp(&Lerp, &m_pBoss3->m_pTransform->m_vInfo[INFO_POS], &m_pBoss3LPart->m_pTransform->m_vInfo[INFO_POS], 0.3f);
			m_pTransform->m_vInfo[INFO_POS] = { Lerp.x,Lerp.y,Lerp.z };
			if(!lstrcmp(m_pTag, L"Boss3LPartShadow"))
			m_pTransform->m_vInfo[INFO_POS] = { Lerp.x,Lerp.y,10.f };
		}

		else if (!lstrcmp(m_pTag, L"Boss3RPart")||!lstrcmp(m_pTag, L"Boss3RPartShadow"))
		{
			D3DXVec3Lerp(&Lerp, &m_pBoss3->m_pTransform->m_vInfo[INFO_POS], &m_pBoss3RPart->m_pTransform->m_vInfo[INFO_POS], 0.3f);
			m_pTransform->m_vInfo[INFO_POS] = { Lerp.x,Lerp.y,Lerp.z };
			if(!lstrcmp(m_pTag, L"Boss3RPartShadow"))
				m_pTransform->m_vInfo[INFO_POS] = { Lerp.x,Lerp.y,10.f };

		}

		else if (!lstrcmp(m_pTag, L"Boss3LPart1") || !lstrcmp(m_pTag, L"Boss3LPart1Shadow"))
		{
			D3DXVec3Lerp(&Lerp, &m_pBoss3->m_pTransform->m_vInfo[INFO_POS], &m_pBoss3LPart->m_pTransform->m_vInfo[INFO_POS], 0.45f);
			m_pTransform->m_vInfo[INFO_POS] = { Lerp.x,Lerp.y,Lerp.z };
			m_pTransform->m_vScale = { 1.4f,0.7f,0.7f };
			if (!lstrcmp(m_pTag, L"Boss3LPart1Shadow"))
				m_pTransform->m_vInfo[INFO_POS] = { Lerp.x,Lerp.y,10.f };

		}

		else if (!lstrcmp(m_pTag, L"Boss3RPart1") || !lstrcmp(m_pTag, L"Boss3RPart1Shadow"))
		{
			D3DXVec3Lerp(&Lerp, &m_pBoss3->m_pTransform->m_vInfo[INFO_POS], &m_pBoss3RPart->m_pTransform->m_vInfo[INFO_POS], 0.45f);
			m_pTransform->m_vInfo[INFO_POS] = { Lerp.x,Lerp.y,Lerp.z };
			m_pTransform->m_vScale = { 1.4f,0.7f,0.7f };
			if (!lstrcmp(m_pTag, L"Boss3RPart1Shadow"))
				m_pTransform->m_vInfo[INFO_POS] = { Lerp.x,Lerp.y,10.f };
		}

		else if (!lstrcmp(m_pTag, L"Boss3LPart2") || !lstrcmp(m_pTag, L"Boss3LPart2Shadow"))
		{
			D3DXVec3Lerp(&Lerp, &m_pBoss3->m_pTransform->m_vInfo[INFO_POS], &m_pBoss3LPart->m_pTransform->m_vInfo[INFO_POS], 0.6f);
			m_pTransform->m_vInfo[INFO_POS] = { Lerp.x,Lerp.y,Lerp.z };
			if (!lstrcmp(m_pTag, L"Boss3LPart2Shadow"))
				m_pTransform->m_vInfo[INFO_POS] = { Lerp.x,Lerp.y,10.f };
		}

		else if (!lstrcmp(m_pTag, L"Boss3RPart2") || !lstrcmp(m_pTag, L"Boss3RPart2Shadow"))
		{
			D3DXVec3Lerp(&Lerp, &m_pBoss3->m_pTransform->m_vInfo[INFO_POS], &m_pBoss3RPart->m_pTransform->m_vInfo[INFO_POS], 0.6f);
			m_pTransform->m_vInfo[INFO_POS] = { Lerp.x,Lerp.y,Lerp.z };
			if (!lstrcmp(m_pTag, L"Boss3RPart2Shadow"))
				m_pTransform->m_vInfo[INFO_POS] = { Lerp.x,Lerp.y,10.f };
		}

		else if (!lstrcmp(m_pTag, L"Boss3LPart3") || !lstrcmp(m_pTag, L"Boss3LPart3Shadow"))
		{
			D3DXVec3Lerp(&Lerp, &m_pBoss3->m_pTransform->m_vInfo[INFO_POS], &m_pBoss3LPart->m_pTransform->m_vInfo[INFO_POS], 0.8f);
			m_pTransform->m_vInfo[INFO_POS] = { Lerp.x,Lerp.y,Lerp.z };
			m_pTransform->m_vScale = { 1.4f,0.7f,0.7f };
			if (!lstrcmp(m_pTag, L"Boss3LPart3Shadow"))
				m_pTransform->m_vInfo[INFO_POS] = { Lerp.x,Lerp.y,10.f };
		}

		else if (!lstrcmp(m_pTag, L"Boss3RPart3") || !lstrcmp(m_pTag, L"Boss3RPart3Shadow"))
		{
			D3DXVec3Lerp(&Lerp, &m_pBoss3->m_pTransform->m_vInfo[INFO_POS], &m_pBoss3RPart->m_pTransform->m_vInfo[INFO_POS], 0.8f);
			m_pTransform->m_vInfo[INFO_POS] = { Lerp.x,Lerp.y,Lerp.z };
			m_pTransform->m_vScale = { 1.4f,0.7f,0.7f };
			if (!lstrcmp(m_pTag, L"Boss3RPart3Shadow"))
				m_pTransform->m_vInfo[INFO_POS] = { Lerp.x,Lerp.y,10.f };
		}

	Engine::Add_RenderGroup(RENDER_ALPHA, this);

	m_pSparkAnimation->Update_Anim(fTimeDelta);

	return 0;
}

_int CBoss3HandPart::Update_Too(const _float& fTimeDelta)
{
	__super::Update_Too(fTimeDelta);
	return 0;
}

_int CBoss3HandPart::Update_Top(const _float& fTimeDelta)
{
	__super::Update_Top(fTimeDelta);
	return 0;
}

void CBoss3HandPart::LateUpdate_GameObject(void)
{
	__super::LateUpdate_GameObject();
}

void CBoss3HandPart::Render_GameObject(void)
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransform->Get_WorldMatrixPointer());
	
	if (m_bDamaged)
	{
		if (0 == (_int)(m_fDamagedTime * 250) % 2)
			m_pTextureCom->Set_Texture(m_iIndex);

		else
			m_pTextureCom->Set_Texture(m_bWhiteIndex);
	}

	else
		m_pTextureCom->Set_Texture(m_iIndex);

	if (m_bSparkON)
		m_pSparkAnimation->Set_Texture(0);	

	if (!lstrcmp(m_pTag, L"Boss3LPartShadow") || !lstrcmp(m_pTag, L"Boss3RPartShadow")||
		!lstrcmp(m_pTag, L"Boss3LPart1Shadow") || !lstrcmp(m_pTag, L"Boss3RPart1Shadow")||
		!lstrcmp(m_pTag, L"Boss3LPart2Shadow") || !lstrcmp(m_pTag, L"Boss3RPart2Shadow")||
		!lstrcmp(m_pTag, L"Boss3LPart3Shadow") || !lstrcmp(m_pTag, L"Boss3RPart3Shadow"))
		m_pTextureCom2->Set_Texture(m_iIndex);

	m_pBufferCom->Render_Buffer();

	__super::Render_GameObject();
}

HRESULT CBoss3HandPart::Add_Component(void)
{
	CComponent* pComponent = nullptr;

	pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(Engine::Clone_Proto(L"RcTex", this));
	NULL_CHECK_RETURN(m_pBufferCom, E_FAIL);
	m_vecComponent[ID_STATIC].push_back({ L"RcTex", pComponent });

	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Boss3_HandPart", this));
	NULL_CHECK_RETURN(m_pTextureCom, E_FAIL);
	m_vecComponent[ID_STATIC].push_back({ L"Boss3_HandPart", pComponent });

	pComponent = m_pSparkAnimation = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Boss3_Spark_Animation", this));
	NULL_CHECK_RETURN(m_pTextureCom, E_FAIL);
	m_vecComponent[ID_STATIC].push_back({ L"Boss3_Spark_Animation", pComponent });

	pComponent = m_pTextureCom2 = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Boss3_HandPart_Shadow", this));
	NULL_CHECK_RETURN(m_pTextureCom2, E_FAIL);
	m_vecComponent[ID_STATIC].push_back({ L"Boss3_HandPart_Shadow", pComponent });

	return S_OK;
}

void CBoss3HandPart::DamagedBoss3(const _float & fTimeDelta)
{
	m_fDamagedTime += fTimeDelta * 0.01f;

	if (3.f <= m_fDamagedTime * 100.f)
	{
		m_bDamaged = false;
		m_fDamagedTime = 0.f;
	}
}

CBoss3HandPart* CBoss3HandPart::Create(LPDIRECT3DDEVICE9 pGraphicDev, _vec3& vPos,_int iIndex)
{
	CBoss3HandPart* pInstance = new CBoss3HandPart(pGraphicDev);

	if (FAILED(pInstance->Ready_GameObject(vPos,iIndex)))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CBoss3HandPart::Free(void)
{
	__super::Free();
}
