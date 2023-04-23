#include "stdafx.h"
#include "LaserTurret.h"
#include "Export_Function.h"
#include "AbstractFactory.h"

#include "Boss3.h"

CLaserTurret::CLaserTurret(LPDIRECT3DDEVICE9 pGraphicDev)
	:CGameObject(pGraphicDev),
	m_iIndex(0),
	m_fCoolDown(0.f), m_fColdist(0.f)
{
}

CLaserTurret::~CLaserTurret()
{
}

HRESULT CLaserTurret::Ready_GameObject(_vec3 & vPos, _int iIndex)
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);
	m_pTransform->m_vInfo[INFO_POS] = vPos;
	m_pTransform->m_vScale = { 1.f,1.f,1.f };
	m_pTransform->m_bIsStatic = false;

	if (1 == iIndex || 0 == iIndex)
		m_iIndex = iIndex;

	else
		m_iIndex = 1;

	m_pRedLine->Set_Width(10.f);
	m_pWhiteLine->Set_Width(8.f);

	return S_OK;
}

_int CLaserTurret::Update_GameObject(const _float & fTimeDelta)
{
	m_pBoss3 = Engine::Get_GameObject(L"Layer_GameLogic", L"Boss3");
	m_fCoolDown += fTimeDelta;

	if (m_bDead)
		return OBJ_DEAD;

	Engine::Add_RenderGroup(RENDER_ALPHA, this);

	if (0.1f < m_fCoolDown)
	{
		Shoot_Laser(fTimeDelta);
		m_fCoolDown = 0.f;
	}		

	m_vPos = m_pTransform->m_vInfo[INFO_POS] + _vec3(0.f, 0.f, 0.1f);

	if (0 == m_iIndex)
		m_vEnd = m_vPos + _vec3(m_fColdist, 0.f, 0.f);
	else
		m_vEnd = m_vPos + _vec3(-m_fColdist, 0.f, 0.f);

	__super::Update_GameObject(fTimeDelta);

	return 0;
}

void CLaserTurret::LateUpdate_GameObject(void)
{
	__super::LateUpdate_GameObject();
}

void CLaserTurret::Render_GameObject(void)
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransform->Get_WorldMatrixPointer());

	m_pTextureCom->Set_Texture(m_iIndex);
	m_pShadow->Render_Shadow(m_pBufferCom);

	m_pBufferCom->Render_Buffer();

	_matrix matWorld, matView, matProj;
	D3DXMatrixIdentity(&matWorld);
	m_pGraphicDev->GetTransform(D3DTS_VIEW, &matView);
	m_pGraphicDev->GetTransform(D3DTS_PROJECTION, &matProj);

	m_pRedLine->Set_Line(m_vPos, m_vEnd, D3DXCOLOR(1.f, 0.f, 0.f, 1.f));
	m_pRedLine->Draw_Line(matWorld, matView, matProj);
	m_pWhiteLine->Set_Line(m_vPos, m_vEnd, D3DXCOLOR(1.f, 1.f, 1.f, 1.f));
	m_pWhiteLine->Draw_Line(matWorld, matView, matProj);

	__super::Render_GameObject();
}

HRESULT CLaserTurret::Add_Component(void)
{
	CComponent* pComponent = nullptr;

	pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(Engine::Clone_Proto(L"RcTex", this));
	NULL_CHECK_RETURN(m_pBufferCom, E_FAIL);
	m_vecComponent[ID_STATIC].push_back({ L"RcTex" , pComponent });

	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Laser_Turret", this));
	NULL_CHECK_RETURN(m_pTextureCom, E_FAIL);
	m_vecComponent[ID_STATIC].push_back({ L"Laser_Turret",pComponent });

	pComponent = m_pShadow = dynamic_cast<CShadow*>(Engine::Clone_Proto(L"Shadow", this));
	NULL_CHECK_RETURN(m_pShadow, E_FAIL);
	m_vecComponent[ID_STATIC].push_back({ L"Shadow", pComponent });

	pComponent = m_pCollider = dynamic_cast<CCollider*>(Engine::Clone_Proto(L"Collider", this));
	NULL_CHECK_RETURN(m_pCollider, E_FAIL);
	m_vecComponent[ID_DYNAMIC].push_back({ L"Collider", pComponent });

	pComponent = m_pRedLine = dynamic_cast<CLine*>(Engine::Clone_Proto(L"Line", this));
	NULL_CHECK_RETURN(m_pRedLine, E_FAIL);
	m_vecComponent[ID_STATIC].push_back({ L"Line", pComponent });

	pComponent = m_pWhiteLine = dynamic_cast<CLine*>(Engine::Clone_Proto(L"Line", this));
	NULL_CHECK_RETURN(m_pWhiteLine, E_FAIL);
	m_vecComponent[ID_STATIC].push_back({ L"Line", pComponent });

	return S_OK;
}

void CLaserTurret::Shoot_Laser(const _float & fTimeDelta)
{
	// ¿ì ÁÂ ¼ø¼­ÀÓ
	_vec3 vDir[2] = { {1.f, 0.f, 0.f},{ -1.f, 0.f, 0.f } };
	_vec3 vPos = m_pTransform->m_vInfo[INFO_POS];
	vector<_tchar*> tagName;
	tagName.push_back(L"MapCube");
	tagName.push_back(L"MoveCube");
	tagName.push_back(L"GravityCube");
	tagName.push_back(L"InstallCube");
	tagName.push_back(L"SwitchCube");
	tagName.push_back(L"CrackCube");
	tagName.push_back(L"PortalCube");
	tagName.push_back(L"KeyCube");
	tagName.push_back(L"Toodee");
	tagName.push_back(L"Topdee");
	tagName.push_back(L"Boss3");
	tagName.push_back(L"Boss3Left");
	tagName.push_back(L"Boss3Right");
	vector<RayCollision> _detectedCOL;

	_detectedCOL = Engine::Check_Collision_Ray(RAYCAST(vPos, vDir[m_iIndex], 60.f), m_pCollider, tagName);

	if (_detectedCOL.size() >= 1)
	{
		if (!lstrcmp(_detectedCOL[0].tag, L"MapCube") ||
			!lstrcmp(_detectedCOL[0].tag, L"MoveCube") ||
			!lstrcmp(_detectedCOL[0].tag, L"GravityCube") ||
			!lstrcmp(_detectedCOL[0].tag, L"InstallCube") ||
			!lstrcmp(_detectedCOL[0].tag, L"SwitchCube") ||
			!lstrcmp(_detectedCOL[0].tag, L"CrackCube") ||
			!lstrcmp(_detectedCOL[0].tag, L"KeyCube") ||
			!lstrcmp(_detectedCOL[0].tag, L"PortalCube")
			)
		{
			m_fColdist = _detectedCOL[0].dist;
		}			

		else if (!lstrcmp(_detectedCOL[0].tag, L"Toodee") ||
			!lstrcmp(_detectedCOL[0].tag, L"Topdee")
			)
		{
			Engine::Get_GameObject(L"Layer_GameLogic", _detectedCOL[0].tag)->m_bDead = true;
		}

		else if (!lstrcmp(_detectedCOL[0].tag, L"Boss3"))
		{
			m_fColdist = _detectedCOL[0].dist + 10.f;
			dynamic_cast<CBoss3*>(m_pBoss3)->Set_Damage();
		}

		else if (!lstrcmp(_detectedCOL[0].tag, L"Boss3Left") ||
			!lstrcmp(_detectedCOL[0].tag, L"Boss3Right"))
		{
   			m_fColdist = _detectedCOL[0].dist + 2.f;
			dynamic_cast<CBoss3*>(m_pBoss3)->Set_Damage();
		}
	}
}	

CLaserTurret * CLaserTurret::Create(LPDIRECT3DDEVICE9 pGraphicDev, _vec3 & vPos, _int iIndex)
{
	CLaserTurret* pInstance = new CLaserTurret(pGraphicDev);

	if (FAILED(pInstance->Ready_GameObject(vPos, iIndex)))
	{
		Safe_Release(pInstance);
		return nullptr;
	}
	return pInstance;
}

void CLaserTurret::Free(void)
{
	__super::Free();
}