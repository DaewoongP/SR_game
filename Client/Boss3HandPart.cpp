#include "stdafx.h"
#include "Boss3HandPart.h"
#include"..\Engine\Export_Function.h"

CBoss3HandPart::CBoss3HandPart(LPDIRECT3DDEVICE9 pGraphicDev) : CGameObject(pGraphicDev)
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
	m_pTransform->m_vScale = { 2.f,1.f,1.f };
	m_pTransform->Rotation(ROT_Y, D3DXToRadian(45.f));

	m_pTransform->m_bIsStatic = true;

	m_pBoss3 = Engine::Get_GameObject(L"Layer_GameLogic", L"Boss3");
	NULL_CHECK_RETURN(m_pBoss3, E_FAIL);

	
	return S_OK;
}

_int CBoss3HandPart::Update_GameObject(const _float& fTimeDelta)
{
	if (m_bDead)
		return OBJ_DEAD;

	m_pBoss3LPart = Engine::Get_GameObject(L"Layer_GameLogic", L"Boss3Left");
	NULL_CHECK_RETURN(m_pBoss3LPart, E_FAIL);

	m_pBoss3RPart = Engine::Get_GameObject(L"Layer_GameLogic", L"Boss3Right");
	NULL_CHECK_RETURN(m_pBoss3RPart, E_FAIL);
	_vec3 Lerp;

	//D3DXVec3Lerp(&LerpL, &m_pBoss3->m_pTransform->m_vInfo[INFO_POS], &m_pBoss3LPart->m_pTransform->m_vInfo[INFO_POS], 0.5f);
	//m_pTransform->m_vInfo[INFO_POS] = Lerp;



	__super::Update_GameObject(fTimeDelta);

		if (!lstrcmp(m_pTag, L"Boss3LPart"))
		{
		
			D3DXVec3Lerp(&Lerp, &m_pBoss3->m_pTransform->m_vInfo[INFO_POS], &m_pBoss3LPart->m_pTransform->m_vInfo[INFO_POS], 0.3f);
			m_pTransform->m_vInfo[INFO_POS] = { Lerp.x,Lerp.y,Lerp.z };
			//Rot2Hand();
			//m_pTransform->Set_ParentTransform(m_pBoss3, 0.f, 0.f,5.f);


		}
		else if (!lstrcmp(m_pTag, L"Boss3RPart"))
		{
			D3DXVec3Lerp(&Lerp, &m_pBoss3->m_pTransform->m_vInfo[INFO_POS], &m_pBoss3RPart->m_pTransform->m_vInfo[INFO_POS], 0.3f);
			m_pTransform->m_vInfo[INFO_POS] = { Lerp.x,Lerp.y,Lerp.z };
			//Rot2Hand();

			//m_pTransform->Set_ParentTransform(m_pBoss3, 0.f, 0.f, -5.f);

		}

	//	else if(!lstrcmp(m_pTag, L"Boss3RPart1"))

	

	
	Engine::Add_RenderGroup(RENDER_ALPHA, this);

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
	m_pTextureCom->Set_Texture(m_iIndex);
	m_pBufferCom->Render_Buffer();

	__super::Render_GameObject();
}

void CBoss3HandPart::Rot2Hand()
{
	_vec3 vDir,vRight;


	if (!lstrcmp(m_pTag, L"Boss3LPart"))
	{
		D3DXVec3Normalize(&vRight, &m_pTransform->m_vInfo[INFO_RIGHT]);
		D3DXVec3Normalize(&vDir,&( m_pBoss3LPart->m_pTransform->m_vInfo[INFO_POS] - m_pTransform->m_vInfo[INFO_POS]));
		_float fAngle = acosf(D3DXVec3Dot(&vRight, &vDir));
		
		m_pTransform->Rotation(ROT_Z, fAngle);


	}
	if (!lstrcmp(m_pTag, L"Boss3RPart"))
	{
		D3DXVec3Normalize(&vRight, &m_pTransform->m_vInfo[INFO_RIGHT]);
		D3DXVec3Normalize(&vDir, &(m_pBoss3RPart->m_pTransform->m_vInfo[INFO_POS] - m_pTransform->m_vInfo[INFO_POS]));
		_float fAngle = acosf(D3DXVec3Dot(&vRight, &vDir));

		m_pTransform->Rotation(ROT_Z, fAngle);


	}



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

	pComponent = m_pShadowCom = dynamic_cast<CShadow*>(Engine::Clone_Proto(L"Shadow", this));
	NULL_CHECK_RETURN(m_pShadowCom, E_FAIL);
	m_vecComponent[ID_DYNAMIC].push_back({ L"Shadow",pComponent });


	return S_OK;
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
