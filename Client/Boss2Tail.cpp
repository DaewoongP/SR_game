#include "stdafx.h"
#include "Boss2Tail.h"
#include "Boss2.h"

CBoss2Tail::CBoss2Tail(LPDIRECT3DDEVICE9 pGraphicDev)
	:CBoss2Parts(pGraphicDev), m_pPreTail(nullptr), m_fStartZ(0.0f), m_fZRatio(0.0f)
{
}

CBoss2Tail::~CBoss2Tail()
{
}

HRESULT CBoss2Tail::Ready_GameObject(_vec3 & vPos)
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);
	m_pTransform->m_vInfo[INFO_POS] = vPos;

	m_pRigid->m_bUseGrivaty = false;
	m_pRigid->m_bUseLimitVelocity = true;
	m_pRigid->m_fLimitVelocity = 8.0f;
	m_pRigid->m_bFreezePos_Z = true;
	m_pShadow->m_bUseShadow = false;
	m_pShadow->m_fOutLineScale = 2.0f;
	m_pShadow->m_fOutLineHeight = 0.02f;
	m_OriginTexture = 0;
	m_WhiteTexture = 5;
	m_CurrentTexture = m_OriginTexture;
	m_fStartZ = vPos.z;

	return S_OK;
}

_int CBoss2Tail::Update_GameObject(const _float & fTimeDelta)
{
	if (m_pPreTail)
	{
		_matrix matBoss;
		CGameObject* pObj = dynamic_cast<CBoss2*>(Engine::Get_GameObject(L"Layer_GameLogic", L"Boss2"));
		if (pObj == nullptr)
			return -1;
		matBoss = pObj->m_pTransform->m_matWorld;
		_vec3 vBossX = { matBoss._11,matBoss._12,matBoss._13 };
		_float fYDis = D3DXVec3Length(&vBossX);
		vBossX /= fYDis;

		_vec3 vPrePos = {
			m_pPreTail->Get_WorldMatrixPointer()->_41 + vBossX.x * 0.5f,//offset 생각을 좀 해봐야 할듯...
			m_pPreTail->Get_WorldMatrixPointer()->_42,
			m_pPreTail->Get_WorldMatrixPointer()->_43 };
		//리지드 이동 방향 및 거리 계산용
		_vec3 vDis = vPrePos - m_pTransform->m_vInfo[INFO_POS];
		//거리 저장
		_float fLength = D3DXVec3Length(&vDis);
		//방향으로 사용
		D3DXVec3Normalize(&vDis, &vDis);
		
		m_pRigid->AddForce(vDis, 1200.f, FORCE, fTimeDelta);
		
		vDis.z = 0.0f;

		if (0.7f < fLength)
		{
			_float fZ = matBoss._43 + 0.35f + m_fStartZ - 10.0f;

			_vec3 vRePosition = vPrePos - vDis;

			m_pTransform->Set_Pos(vRePosition.x, vRePosition.y, fZ);
		}
	}
	

__super::Update_GameObject(fTimeDelta);
	Engine::Add_RenderGroup(RENDER_ALPHA, this);

	/*if (m_bMetronome)
	{
		if (1.0f > m_fTimer)
		{
			m_fTimer += fTimeDelta;

			if (1.0f < m_fTimer)
				m_fTimer = 1.0f;

			m_pTransform->m_vAngle.z = D3DXToRadian(Lerp(BOSETAILPARTWAVESTART, BOSETAILPARTWAVEEND, m_fTimer));

			if (1.0f <= m_fTimer)
			{
				m_fTimer = 0.0f;

				m_bMetronome = false;
			}

		}
	}
	else if (!m_bMetronome)
	{
		if (1.0f > m_fTimer)
		{
			m_fTimer += fTimeDelta;

			if (1.0f < m_fTimer)
				m_fTimer = 1.0f;

			m_pTransform->m_vAngle.z = D3DXToRadian(Lerp(BOSETAILPARTWAVEEND, BOSETAILPARTWAVESTART, m_fTimer));

			if (1.0f <= m_fTimer)
			{
				m_fTimer = 0.0f;

				m_bMetronome = true;
			}
		}
	}*/
	if (!g_Is2D)
	{
		if (0.8f < m_fZRatio)
		{
			m_fTimer += fTimeDelta*4.0f;
			if (1.0f < m_fTimer)
			{
				m_fTimer = 1.0f;
			}
			m_fZRatio = Lerp(1.0f, 0.8f, m_fTimer);
		}
	}
	if (g_Is2D)
	{
		if (1.0f > m_fZRatio)
		{
			m_fTimer += fTimeDelta*4.0f;
			if (1.0f < m_fTimer)
			{
				m_fTimer = 1.0f;
			}
			m_fZRatio = Lerp(0.8f, 1.0f, m_fTimer);
		}
	}
	return 0;
}

_int CBoss2Tail::Update_Too(const _float & fTimeDelta)
{
	return _int();
}

_int CBoss2Tail::Update_Top(const _float & fTimeDelta)
{
	return _int();
}

void CBoss2Tail::LateUpdate_GameObject(void)
{
	if (m_pPreTail)
	{
		m_pTransform->m_matWorld._42 *= m_fZRatio;
		if (dynamic_cast<CBoss2*>(Engine::Get_GameObject(L"Layer_GameLogic", L"Boss2"))->Get_Flip_Y())
		{
			m_pTransform->m_matWorld._43 = m_pPreTail->m_matWorld._43 - 0.01f;
		}
	}
	__super::LateUpdate_GameObject();
}

void CBoss2Tail::Render_GameObject(void)
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransform->Get_WorldMatrixPointer());
	m_pTextureCom->Set_Texture(m_CurrentTexture);
	m_pShadow->Render_Shadow(m_pBufferCom);
	m_pBufferCom->Render_Buffer();
	__super::Render_GameObject();
}

void CBoss2Tail::Render_Too(void)
{
}

void CBoss2Tail::Render_Top(void)
{
}

HRESULT CBoss2Tail::Add_Component(void)
{
	CComponent*		pComponent = nullptr;

	pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(Engine::Clone_Proto(L"RcTex", this));
	NULL_CHECK_RETURN(m_pBufferCom, E_FAIL);
	m_vecComponent[ID_STATIC].push_back({ L"RcTex", pComponent });

	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Boss2_Head", this));
	NULL_CHECK_RETURN(m_pTextureCom, E_FAIL);
	m_vecComponent[ID_STATIC].push_back({ L"Texture", pComponent });

	pComponent = m_pRigid = dynamic_cast<CRigidbody*>(Engine::Clone_Proto(L"Rigidbody", this));
	NULL_CHECK_RETURN(m_pRigid, E_FAIL);
	m_vecComponent[ID_DYNAMIC].push_back({ L"Rigidbody", pComponent });

	pComponent = m_pShadow = dynamic_cast<CShadow*>(Engine::Clone_Proto(L"Shadow", this));
	NULL_CHECK_RETURN(m_pShadow, E_FAIL);
	m_vecComponent[ID_DYNAMIC].push_back({ L"Shadow", pComponent });

	return S_OK;
}

CBoss2Tail * CBoss2Tail::Create(LPDIRECT3DDEVICE9 pGraphicDev, _vec3 & vPos)
{
	CBoss2Tail*		pInstance = new CBoss2Tail(pGraphicDev);

	if (FAILED(pInstance->Ready_GameObject(vPos)))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CBoss2Tail::Free(void)
{
	__super::Free();
}
