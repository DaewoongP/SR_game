#include "stdafx.h"
#include "MiniTopdee.h"
#include "MoveCube.h"
#include "GravityCube.h"
#include "PortalCube.h"
#include "CrackCube.h"
#include "Spike.h"
#include "Key.h"
#include "LaserTurret.h"
#include "KeyCube.h"
#include "PinkCloud.h"

CMiniTopdee::CMiniTopdee(LPDIRECT3DDEVICE9 pGraphicDev)
	:CGameObject(pGraphicDev), m_fSpeed(5.f), m_pLayer(nullptr), m_fCubeSpeed(10.f), m_iChkPortalCube(0)
{
}


CMiniTopdee::~CMiniTopdee()
{
}

HRESULT CMiniTopdee::Ready_GameObject(CLayer* pLayer, _vec3& vPos)
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);
	m_pTransform->m_vScale *= 2.f;
	m_pTransform->m_vInfo[INFO_POS] = vPos;
	m_pLayer = pLayer;

	Engine::Ready_Frame(L"ShootCubeTime", _float(1 / SHOOTCUBETIME));
	return S_OK;
}

_int CMiniTopdee::Update_GameObject(const _float & fTimeDelta)
{
	Engine::Add_RenderGroup(RENDER_ALPHA, this);
	MoveY(fTimeDelta);
	ShootCube(fTimeDelta);
	__super::Update_GameObject(fTimeDelta);
	return 0;
}

void CMiniTopdee::LateUpdate_GameObject(void)
{
	__super::LateUpdate_GameObject();
}

void CMiniTopdee::Render_GameObject(void)
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransform->Get_WorldMatrixPointer());

	m_pTex->Set_Texture(0);
	m_pBuf->Render_Buffer();

	__super::Render_GameObject();
}

void CMiniTopdee::MoveY(const _float& fTimeDelta)
{
	if (m_pTransform->m_vInfo[INFO_POS].y <= BOTY)
	{
		m_pTransform->m_vInfo[INFO_POS].y = BOTY;
		m_fSpeed *= -1;
	}
	else if (m_pTransform->m_vInfo[INFO_POS].y >= TOPY)
	{
		m_pTransform->m_vInfo[INFO_POS].y = TOPY;
		m_fSpeed *= -1;
	}

	m_pTransform->m_vInfo[INFO_POS].y += m_fSpeed * fTimeDelta;
}

void CMiniTopdee::ShootCube(const _float& fTimeDelta)
{
	if (Engine::IsPermit_Call(L"ShootCubeTime", fTimeDelta))
	{
		_int iRandCube = rand() % 9;

		switch (iRandCube)
		{
		case 0:
			CreateCube<CMoveCube>(L"MoveCube");
			break;
		case 1:
			CreateCube<CGravityCube>(L"GravityCube");
			break;
		case 2:
			if (m_iChkPortalCube < 2)
			{
				CreateCubewithDir<CPortalCube>(L"PortalCube", 0);
				++m_iChkPortalCube;
			}
			else
			{
				CreateCube<CMoveCube>(L"MoveCube");
			}
			break;
		case 3:
			CreateCube<CCrackCube>(L"CrackCube");
			break;
		case 4:
			CreateCube<CSpike>(L"Spike");
			break;
		case 5:
			CreateCube<CKey>(L"Key");
			break;
		case 6:
			CreateCube<CCrackCube>(L"CrackCube");
			break;
		case 7:
			if (0 < CKey::iKeyCnt)
				CreateCube<CKeyCube>(L"KeyCube");
			else
				CreateCube<CMoveCube>(L"MoveCube");
			break;
		case 8:
			CreateCube<CPinkCloud>(L"PinkCloud");
			break;
		}
	}

	for (auto iter = m_vecCube.begin(); iter != m_vecCube.end();)
	{
		if ((*iter)->m_bDead)
		{
			iter = m_vecCube.erase(iter);
			continue;
		}
		(*iter)->m_pTransform->m_vInfo[INFO_POS].x += m_fCubeSpeed * fTimeDelta;

		if ((*iter)->m_pTransform->m_vInfo[INFO_POS].x >= 59.f)
		{
			if (!lstrcmp((*iter)->m_pTag, L"PortalCube"))
				--m_iChkPortalCube;
			else if (!lstrcmp((*iter)->m_pTag, L"Key"))
				(*iter)->OnCollisionEnter(nullptr);
			Engine::Delete_Collider(*iter);
			(*iter)->m_bDead = true;
			iter = m_vecCube.erase(iter);
		}
		else
			++iter;
	}
}

template<typename T>
inline void CMiniTopdee::CreateCube(const _tchar* pTag)
{
	_vec3 vPos = m_pTransform->m_vInfo[INFO_POS];
	vPos.x += 2.f;
	CGameObject* pGameObject = T::Create(m_pGraphicDev, vPos);
	pGameObject->Sort_Component();
	m_pLayer->Add_GameObject(pTag, pGameObject);
	m_vecCube.push_back(pGameObject);
}

template<typename T>
void CMiniTopdee::CreateCubewithDir(const _tchar * pTag, _int iDir)
{
	_vec3 vPos = m_pTransform->m_vInfo[INFO_POS];
	vPos.x += 2.f;
	CGameObject* pGameObject = T::Create(m_pGraphicDev, vPos, iDir);
	pGameObject->Sort_Component();
	m_pLayer->Add_GameObject(pTag, pGameObject);
	m_vecCube.push_back(pGameObject);
}

HRESULT CMiniTopdee::Add_Component(void)
{
	CComponent*		pComponent = nullptr;

	pComponent = m_pBuf = dynamic_cast<CRcTex*>(Engine::Clone_Proto(L"RcTex", this));
	NULL_CHECK_RETURN(m_pBuf, E_FAIL);
	m_vecComponent[ID_STATIC].push_back({ L"RcTex" , pComponent });

	pComponent = m_pTex = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"MiniTopdee", this));
	NULL_CHECK_RETURN(m_pTex, E_FAIL);
	m_vecComponent[ID_STATIC].push_back({ L"MiniTopdee", pComponent });

	return S_OK;
}

CMiniTopdee * CMiniTopdee::Create(LPDIRECT3DDEVICE9 pGraphicDev, _vec3& vPos, CLayer* pLayer)
{
	CMiniTopdee*	pInstance = new CMiniTopdee(pGraphicDev);

	if (FAILED(pInstance->Ready_GameObject(pLayer, vPos)))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CMiniTopdee::Free()
{
	__super::Free();
}
